#pragma

namespace php {
	class value;
	class object {
	private:
		zend_object* obj_;
		// !!! 仅允许调用类成员函数
		static value _call(zend_object* obj, const char* name, std::size_t len, int argc, zval argv[]);
		object(zend_object* obj);
		object(zend_class_entry* ce);
	public:
		~object();
		static object clone(const object& obj);
		object(const object& obj);
		object(object&& obj);
		// 构造
		object(const std::string& name);
		template<class T>
		static object create() {
			return object(zend_objects_new(class_entry<T>::entry()));
		}
		value& prop(const char* name, std::size_t len);
		inline value& prop(const std::string& name) {
			return prop(name.c_str(), name.length());
		}
		// !!! 仅允许调用类成员函数
		inline value call(const std::string& name) {
			return _call(obj_, name.c_str(), name.length(), 0, nullptr);
		}
		template <typename ...Args>
		inline value call(const std::string& name, const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return _call(obj_, name.c_str(), name.length(), sizeof...(Args), (zval*)params);
		}
		template <class T>
		inline T* native() {
			return class_wrapper<T>::from_this(obj_);
		};
		friend class value;
	};
}
