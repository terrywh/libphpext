namespace phpext {
	class extension;
	enum {
		PUBLIC    = ZEND_ACC_PUBLIC,
		PROTECTED = ZEND_ACC_PROTECTED,
		PRIVATE   = ZEND_ACC_PRIVATE,
	};
	class class_entry_base {
	public:
		class_entry_base(class_entry_base&& base) {
			_wrapper = base._wrapper;
			base._wrapper = nullptr;
		}
		virtual void do_register() = 0;
	protected:
		// @ zend_class_entry 参考 Zend/zend.h:131
		void* _wrapper;

	};
	template <class T>
	class class_entry: public internal::function_adder, public class_entry_base {
	public:
		// 定义类，可以指定父类
		class_entry(const char* name, const char* parent = nullptr) {
			zend_class_entry entry;
			// @参照 INIT_OVERLOADED_CLASS_ENTRY_EX -> 参考 Zend/zend_API.h:184
			std::memset(&entry, 0, sizeof(entry));
			zend_string* pname = zend_string_init(name, strlen(name), true); // 这个不回收
			entry.name = zend_new_interned_string(pname);
			if(parent != nullptr) {
				pname = zend_string_init(parent, strlen(parent), false); // 这个要回收
				entry.parent = zend_lookup_class(pname);
				zend_string_release(pname);
			}
			void* pn = emalloc(sizeof(internal::class_wrapper<T>));
			// 类其实在这里已经注册了（对应 extension 中的 add 方法只是将类方法登记）
			if(entry.parent) {
				_wrapper = new(pn) internal::class_wrapper<T>( zend_register_internal_class_ex(&entry, entry.parent) );
			}else{
				_wrapper = new(pn) internal::class_wrapper<T>( zend_register_internal_class(&entry) );
			}
		}
		~class_entry() {
			if(_wrapper != nullptr) {
				efree(_wrapper);
			}
		}
		class_entry& final() {
			((internal::class_wrapper<T>*)_wrapper)->class_entry->ce_flags |= ZEND_ACC_FINAL;
			return *this;
		}
		// 实现接口
		class_entry& implements(const char* interf) {
			zend_string* name = zend_string_init(interf, strlen(interf), false); // 这个要回收
			zend_class_entry* intf = zend_lookup_class(name);
			if(intf == nullptr) {
				zend_error(E_WARNING, "unknown interface '%s'", intf);
			}else{
				zend_do_implement_interface(((internal::class_wrapper<T>*)_wrapper)->class_entry, intf);
			}
			zend_string_release(name);
			return *this;
		}
		// 属性
		class_entry& property(const char* name, std::nullptr_t value, int access = PUBLIC) {
			zend_declare_property_null(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), access);
			return *this;
		}
		class_entry& property(const char* name, bool value, int access = PUBLIC) {
			zend_declare_property_bool(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value, access);
			return *this;
		}
		class_entry& property(const char* name, std::int64_t value, int access = PUBLIC) {
			zend_declare_property_long(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value, access);
			return *this;
		}
		class_entry& property(const char* name, double value, int access = PUBLIC) {
			zend_declare_property_double(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value, access);
			return *this;
		}
		class_entry& property(const char* name, const char* value, int access = PUBLIC) {
			zend_declare_property_string(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value, access);
			return *this;
		}
		class_entry& contant(const char* name, bool value) {
			zend_declare_class_constant_bool(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value);
			return *this;
		}
		class_entry& contant(const char* name, std::int64_t value) {
			zend_declare_class_constant_long(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value);
			return *this;
		}
		class_entry& contant(const char* name, double value) {
			zend_declare_class_constant_double(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value);
			return *this;
		}
		class_entry& contant(const char* name, const char* value) {
			zend_declare_class_constant_string(((internal::class_wrapper<T>*)_wrapper)->class_entry, name, std::strlen(name), value);
			return *this;
		}
		template <void (T::*callback)(parameters& params)>
		class_entry& add(string&& name, std::initializer_list<internal::arg_info> init = {}) {
			function_adder::_add(_function_entries, std::move(name), internal::method_adder<T>:: template return_void<callback>, std::move(init));
			return *this;
		}
		template <value (T::*callback)(parameters& params)>
		class_entry& add(string&& name, std::initializer_list<internal::arg_info> init = {}) {
			function_adder::_add(_function_entries, std::move(name), internal::method_adder<T>:: template return_value<callback>, std::move(init));
			return *this;
		}
		virtual void do_register() override {
			internal::class_wrapper<T>* w = (internal::class_wrapper<T>*)_wrapper;
			// @参照 Zend/zend_API.c:2656
			w->class_entry->info.internal.builtin_functions = do_register_functions();
			// 由于在 zend_register_internal_class 调用时还没有 current_module 上下文
			w->class_entry->info.internal.module = EG(current_module);
			zend_register_functions(w->class_entry, w->class_entry->info.internal.builtin_functions, &w->class_entry->function_table, MODULE_PERSISTENT);
		}
	};
}
