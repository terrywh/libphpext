#pragma once

namespace php {
	class value;
	class object: public value {
	private:
		// !!! 仅允许调用类成员函数
		static value __call(zend_object* obj, const char* name, std::size_t len, int argc, zval argv[], bool silent);
		object(zend_class_entry* ce) {
			ZVAL_OBJ(&value_, zend_objects_new(ce));
		}
	public:
		object(): value() {}
		object(nullptr_t nptr): value(nptr) {}
		object(zend_object* obj): value(obj, false) {}
		object(class_base* obj): value(obj) {}
		object clone();
		// 构造
		template<class T>
		static object create() {
			object obj;
			ZVAL_OBJ(&obj.value_, class_entry<T>::create_object());
			return std::move(obj);
		}
		static object create(const std::string& name);
		static object create();
		value& prop(const char* name, std::size_t len);
		inline value& prop(const std::string& name) {
			return prop(name.c_str(), name.length());
		}
		value& sprop(const char* name, std::size_t len, php::value& val);
		inline value& sprop(const std::string& name, php::value& val) {
			sprop(name.c_str(), name.length(), val);
		}
		// !!! 仅允许调用类成员函数
		inline value call(const std::string& name) {
			return __call(Z_OBJ(value_), name.c_str(), name.length(), 0, nullptr, false);
		}
		inline value scall(const std::string& name) {
			return __call(Z_OBJ(value_), name.c_str(), name.length(), 0, nullptr, true);
		}
		template <typename ...Args>
		inline value call(const std::string& name, const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __call(Z_OBJ(value_), name.c_str(), name.length(), sizeof...(Args), (zval*)params, false);
		}
		template <typename ...Args>
		inline value scall(const std::string& name, const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __call(Z_OBJ(value_), name.c_str(), name.length(), sizeof...(Args), (zval*)params, true);
		}
		bool is_instance_of(const std::string& class_name) const;
		template<class T>
		inline bool is_instance_of() const {
			return Z_OBJ(value_)->handlers == &class_entry<T>::handlers_;
		}
		template <class T>
		inline T* native() {
			return class_wrapper<T>::from_obj(Z_OBJ(value_));
		};
		using value::operator =;
		using value::operator ==;
	};
}
