#pragma once

#include "../vendor.h"
#include "../types/value.h"
#include "method_entry.h"
#include "property_entry.h"
#include "arguments.h"

namespace php {
	enum {
		PUBLIC    = ZEND_ACC_PUBLIC,
		PROTECTED = ZEND_ACC_PROTECTED,
		PRIVATE   = ZEND_ACC_PRIVATE,
	};
	class class_entry_base {
	public:
		virtual void declare() = 0;
	};
	template <class T>
	class class_entry: public class_entry_base {
	public:
		class_entry(const char* name)
		: name_(name)
		, parent_class_entry_(nullptr) {
			init_handlers();
		}
		class_entry(class_entry&& entry)
		: name_(entry.name_)
		, parent_class_entry_(entry.parent_class_entry_)
		// , constant_entries_(std::move(entry.constant_entries_))
		, property_entries_(std::move(entry.property_entries_))
		, method_entries_(std::move(entry.method_entries_))
		, arguments_(std::move(entry.arguments_)) {
			entry.name_ = nullptr;
			entry.parent_class_entry_ = nullptr;

		}
		// class_entry& add(const constant_entry& entry) {
		// 	constant_entries_.push_back(entry);
		// 	return *this;
		// }

		class_entry& add(property_entry&& entry) {
			property_entries_.emplace_back(std::move(entry));
			return *this;
		}
		// 方法
		template <value (T::*FUNCTION)(parameters& params) >
		class_entry& add(const char* name, int access = ZEND_ACC_PUBLIC) {
			zend_function_entry entry;
			method_entry<T,FUNCTION>::fill(&entry, name, access);
			method_entries_.push_back(entry);
		}
		// 方法
		template <value (T::*FUNCTION)(parameters& params) >
		class_entry& add(const char* name, arguments&& info, int access = ZEND_ACC_PUBLIC) {
			zend_function_entry entry;
			arguments_.emplace_back(std::move(info));
			method_entry<T,FUNCTION>::fill(&entry, name, arguments_.back(), access);
			method_entries_.push_back(entry);
		}

		class_entry& extends(const std::string& class_name) {
			zend_string* name = zend_string_init(class_name.c_str(), class_name.length(), 0);
			parent_class_entry_ = zend_lookup_class(name);
			zend_string_release(name);
			
			if(parent_class_entry_ != nullptr && parent_class_entry_->ce_flags & ZEND_ACC_INTERFACE) {
				parent_class_entry_ = nullptr;
			}
		}

		class_entry& implements(const std::string& interface_name) {
			zend_string* name = zend_string_init(interface_name.c_str(), interface_name.length(), 0);
			zend_class_entry* intf = zend_lookup_class(name);
			zend_string_release(name);
			if(intf != nullptr && (parent_class_entry_->ce_flags & ZEND_ACC_INTERFACE)) {
				interfaces_.push_back(intf);
			}
		}

		class_entry& use(const std::string& trait_name) {
			zend_string* name = zend_string_init(trait_name.c_str(), trait_name.length(), 0);
			zend_class_entry* intf = zend_lookup_class(name);
			zend_string_release(name);
			if(intf != nullptr && (parent_class_entry_->ce_flags & ZEND_ACC_TRAIT)) {
				traits_.push_back(intf);
			}
		}

		virtual void declare() override {
			method_entries_.push_back(zend_function_entry{}); // 结束数组条件
			zend_class_entry ce;
			INIT_OVERLOADED_CLASS_ENTRY_EX(ce, name_, std::strlen(name_), method_entries_.data(), nullptr, nullptr, nullptr, nullptr, nullptr);
			ce.create_object = class_entry<T>::create_object;
			zend_class_entry* ce_ = zend_register_internal_class_ex(&ce, parent_class_entry_);
			for(auto i=interfaces_.begin();i!=interfaces_.end();++i) {
				zend_class_implements(ce_, 1, *i);
			}
			interfaces_.clear();
			for(auto i=traits_.begin();i!=traits_.end();++i) {
				zend_do_implement_trait(ce_, *i);
			}
			traits_.clear();
			
			for(auto i=property_entries_.begin();i!=property_entries_.end();++i) {
				i->declare(ce_);
			}
		}
	private:
		const char*                      name_;
		zend_class_entry*                parent_class_entry_;
		// std::vector<constant_entry>      constant_entries_;
		std::vector<property_entry>      property_entries_;
		std::vector<zend_function_entry> method_entries_;
		std::vector<arguments>           arguments_;
		std::vector<zend_class_entry*>   interfaces_;
		std::vector<zend_class_entry*>   traits_;

		static zend_object_handlers handlers_;
		void init_handlers() {
			std::memcpy(&handlers_, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
			handlers_.offset    = XtOffsetOf(class_wrapper<T>, obj);
			handlers_.free_obj  = class_entry<T>::free_object;
			handlers_.clone_obj = class_entry<T>::clone_object;
		}
		static zend_object *create_object(zend_class_entry *entry) {
			auto wrapper = reinterpret_cast<class_wrapper<T>*>(emalloc(sizeof(class_wrapper<T>) + zend_object_properties_size(entry)));
			new (&wrapper->cpp)T(); // 创建 C++ 类对象
			zend_object_std_init(&wrapper->obj, entry);
			object_properties_init(&wrapper->obj, entry);
			wrapper->obj.handlers = &handlers_;
			wrapper->cpp._object_set(&wrapper->obj);
			return &wrapper->obj;
		}

		static void free_object(zend_object* object) {
			auto wrapper = reinterpret_cast<class_wrapper<T>*>((char*)object - XtOffsetOf(class_wrapper<T>, obj));
			zend_object_std_dtor(&wrapper->obj);
			wrapper->cpp.~T();
			// emalloc 的内存会被 PHP 主动释放
			efree(wrapper);
		}

		static zend_object* clone_object(zval* self) {
			class_wrapper<T>* obj1 = reinterpret_cast<class_wrapper<T>*>((char*)Z_OBJ_P(self) + XtOffsetOf(class_wrapper<T>, obj));
			class_wrapper<T>* wrapper = reinterpret_cast<class_wrapper<T>*>(emalloc(sizeof(class_wrapper<T>) + zend_object_properties_size(Z_OBJ_P(self)->ce)));
			new (&wrapper->cpp)T(obj1->cpp); // 调用拷贝构造
			zend_object_std_init(&wrapper->obj, Z_OBJ_P(self)->ce);
			zend_objects_clone_members(&wrapper->obj, &obj1->obj);
			wrapper->obj.handlers = &handlers_;
			wrapper->cpp._object_set(&wrapper->obj);
			return &wrapper->obj;
		}

		static void dtor_obj(zend_object* object) {
			auto wrapper = reinterpret_cast<class_wrapper<T>*>((char*)object + XtOffsetOf(class_wrapper<T>, obj));
		}
	};


	template <class T>
	zend_object_handlers class_entry<T>::handlers_;
	// inline T* from_this(zval* self) {
	// 	assert(Z_TYPE_P(self) == IS_OBJECT);
	// 	return (T*)((char*)Z_OBJ_P(self) - XtOffsetOf(class_wrapper<T>, obj));
	// }
}
