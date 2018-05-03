#pragma once

namespace php {
	class value;
	class object: public value {
	private:
		// !!! 仅允许调用类成员函数
		static value __call(zval* obj, zval* fn, int argc, zval* argv, bool silent);
		object(zend_class_entry* ce) {
			ZVAL_OBJ(&value_, zend_objects_new(ce));
		}
	public:
		using value::value;
		using value::operator =;
		using value::operator ==;
		using value::operator zval*;
		using value::operator zend_object*;
		object(): value() {}
		object(const php::value& v): value(v) {}
		object(php::value&& v): value(std::move(v)) {}
		// 构造
		template<class T>
		static object create() {
			object obj;
			ZVAL_OBJ(&obj.value_, class_entry<T>::create_object());
			return std::move(obj);
		}
		static object create(const std::string& name);
		static object create(zend_class_entry* ce);
		static object create_exception(const std::string& message, int code = 0);
		static object create();
		inline property prop(const php::string& name, bool silent = false) const {
			return php::property(*this, name, silent);
		}
		inline property prop(const char* name, std::size_t len, bool silent = false) const {
			return php::property(*this, php::string(name, len), silent);
		}
		inline property prop(const std::string& name, bool silent = false) const {
			return php::property(*this, php::string(name), silent);
		}
		// !!! 仅允许调用类成员函数
		inline value call(const std::string& name) {
			return __call(&value_, php::string(name.c_str(), name.length()), 0, nullptr, false);
		}
		inline value scall(const std::string& name) {
			return __call(&value_, php::string(name.c_str(), name.length()), 0, nullptr, true);
		}
		inline value call(const std::string& name, std::vector<php::value> argv) {
			return __call(&value_, php::string(name.c_str(), name.length()), argv.size(), (zval*)argv.data(), false);
		}
		inline value scall(const std::string& name, std::vector<php::value> argv) {
			return __call(&value_, php::string(name.c_str(), name.length()), argv.size(), (zval*)argv.data(), true);
		}
		inline value call(const php::string& name) {
			return __call(&value_, name, 0, nullptr, false);
		}
		inline value scall(const php::string& name) {
			return __call(&value_, name, 0, nullptr, true);
		}
		inline value call(const php::string& name, std::vector<php::value> argv) {
			return __call(&value_, name, argv.size(), (zval*)argv.data(), false);
		}
		inline value scall(const php::string& name, std::vector<php::value> argv) {
			return __call(&value_, name, argv.size(), (zval*)argv.data(), true);
		}
		bool is_instance_of(zend_class_entry* ce) const;
		bool is_instance_of(const std::string& class_name) const;
		template<class T>
		inline bool is_instance_of() const {
			return Z_OBJ(value_)->handlers == &class_entry<T>::handlers_;
		}
		template <class T>
		inline T* native() {
			return class_wrapper<T>::from_obj(Z_OBJ(value_));
		};
		inline operator zend_class_entry*() const {
			return Z_OBJCE(value_);
		}
	};
}
