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
		object(): value() {}
		explicit object(nullptr_t nptr): value(nptr) {}
		explicit object(zend_object* obj, bool create = false): value(obj, create) {}
		explicit object(class_base* obj): value(obj) {}
		object(const object& obj): value(obj) {}
		object(object&& obj): value(std::move(obj)) {}
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
		inline property prop(const php::string& name) const {
			return php::property(*this, name);
		}
		inline property prop(const char* name, std::size_t len) const {
			return prop(php::string(name, len));
		}
		inline property prop(const std::string& name) const {
			return prop(name.c_str(), name.length());
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
		inline operator zend_object*() const {
			return Z_OBJ(value_);
		}
		using value::operator =;
		using value::operator ==;
		inline object& operator=(const object& v) {
			value::operator=(v);
			return *this;
		}
		inline object& operator=(object&& v) {
			value::operator=(std::move(v));
			return *this;
		}
	};
}
