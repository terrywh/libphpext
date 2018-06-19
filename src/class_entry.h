#pragma once

namespace php {
	class class_entry_base {
	public:
		virtual ~class_entry_base() {}
		virtual void declare() = 0;
	};
	template <class T>
	class class_entry: public class_entry_base {
	private:
		std::string                      name_;
		zend_class_entry**               ce_parent;
		std::vector<zend_class_entry**>  ce_interface;

		std::vector<constant_entry>      entry_contants;
		std::vector<property_entry>      entry_properties;
		std::vector<zend_function_entry> entry_methods;
		std::vector<arguments>           arguments_;

		static zend_class_entry*     entry_;
		static zend_object_handlers  entry_handler;

		static zend_object* create_object(zend_class_entry *entry) {
			assert(entry == entry_);
			size_t psize = zend_object_properties_size(entry_);
			char*  pdata = (char*)
				ecalloc(1, sizeof(class_wrapper) + psize);
			class_wrapper* wrapper = reinterpret_cast<class_wrapper*>(pdata);
			// 初始化 PHP 对象
			zend_object_std_init(&wrapper->obj, entry_);
			object_properties_init(&wrapper->obj, entry_);
			wrapper->obj.handlers = &entry_handler;
			// 在指定内存位置构造 C++ 对象
			wrapper->cpp.reset(new T());
			ZVAL_OBJ(&wrapper->cpp->obj_, &wrapper->obj);
			return &wrapper->obj;
		}
		static void free_object(zend_object* obj) {
			class_wrapper* wrapper = reinterpret_cast<class_wrapper*>( ((char*)obj) - entry_handler.offset );
			wrapper->cpp.reset();
			zend_object_std_dtor(obj);
			// efree 会被 php 自行调用
		}
	public:
		class_entry(const std::string& name)
		: name_(name)
		, ce_parent(nullptr) {
			std::memcpy(&entry_handler, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
			entry_handler.offset   = XtOffsetOf(class_wrapper, obj);
			entry_handler.free_obj = class_entry::free_object;
		}
		class_entry(class_entry&& entry)
		: name_(entry.name_)
		, ce_parent(entry.ce_parent)
		, ce_interface(std::move(entry.ce_interface))
		, entry_contants(std::move(entry.entry_contants))
		, entry_properties(std::move(entry.entry_properties))
		, entry_methods(std::move(entry.entry_methods))
		, arguments_(std::move(entry.arguments_)) {

		}
		static CLASS entry() {
			assert(entry_);
			return entry_;
		}
		static zend_class_entry** entry(bool ptr) {

		}
		class_entry& extends(zend_class_entry** c) {
			ce_parent = c;
			return *this;
		}
		class_entry& implements(zend_class_entry** c) {
			ce_interface.push_back(c);
			return *this;
		}
		class_entry& constant(const constant_entry& entry) {
			entry_contants.push_back(entry);
			return *this;
		}
		class_entry& property(property_entry&& entry) {
			entry_properties.emplace_back(std::move(entry));
			return *this;
		}
		// 方法
		template <value (T::*METHOD)(parameters& params) >
		class_entry& method(const char* name, uint32_t s = PUBLIC) {
			entry_methods.emplace_back(zend_function_entry {
				name, // fname
				method_delegate<T, METHOD>, // handler
				nullptr, // arg_info,
				0, // num_args
				0,
			});
			return *this;
		}
		template <value (T::*METHOD)(parameters& params) >
		class_entry& method(const char* name, arguments&& desc, uint32_t s = PUBLIC) {
			arguments_.emplace_back(std::move(desc));
			arguments& argv = arguments_.back();
			entry_methods.emplace_back(zend_function_entry {
				name, // fname
				method_delegate<T, METHOD>, // handler
				argv, // arg_info,
				argv.size(), // num_args
				0,
			});
			return *this;
		}
		// TODO 优化内部对象定义, 使用封装后的类型?
		virtual void declare() override {
			entry_methods.push_back(zend_function_entry{}); // 结束数组条件
			zend_class_entry ce, *pce = nullptr;
			INIT_OVERLOADED_CLASS_ENTRY_EX(ce, name_.c_str(), name_.length(),
				entry_methods.data(), nullptr, nullptr, nullptr, nullptr, nullptr);
			// 在继承后再进行设置, 否则实际会执行父类的 create_object 函数
			// ce.create_object = class_entry::create_object;
			// 继承父类
			if(ce_parent) {
				class_entry::entry_ = zend_register_internal_class_ex(&ce, *ce_parent);
			}else{
				class_entry::entry_ = zend_register_internal_class_ex(&ce, nullptr);
			}
			class_entry::entry_->create_object = class_entry::create_object;
			// 实现接口
			for(auto i=ce_interface.begin();i!=ce_interface.end();++i) {
				zend_class_implements(class_entry::entry_, 1, **i);
			}
			// 常量声明
			if(!entry_contants.empty()) {
				for(auto i=entry_contants.begin();i!=entry_contants.end();++i) {
					i->declare(class_entry::entry_);
				}
				entry_contants.clear();
			}
			// 属性声明
			if(!entry_properties.empty()) {
				for(auto i=entry_properties.begin();i!=entry_properties.end();++i) {
					i->declare(class_entry::entry_);
				}
				entry_properties.clear();
			}
		}
	};

	template <class T>
	zend_object_handlers class_entry<T>::entry_handler;
	template <class T>
	zend_class_entry*    class_entry<T>::entry_;
}