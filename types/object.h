#pragma once

namespace php {
	class value;
	class property;
	class object: public value {
	private:
		// !!! 仅允许调用类成员函数
		static value __call(zval* obj, zval* fn, int argc, zval* argv);
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
		static object create(const php::string& name);
		static object create(zend_class_entry* ce);
		static object create();
		property prop(const php::string& name) const; 
		property prop(const char* name, std::size_t len = -1) const;
		// !!! 仅允许调用类成员函数
		inline value call(const php::string& name) {
			return __call(&value_, name, 0, nullptr);
		}
		inline value call(const php::string& name, std::vector<php::value> argv) {
			return __call(&value_, name, argv.size(), (zval*)argv.data());
		}
		inline value call(const char* name, std::size_t size = -1) {
			return __call(&value_, php::string(name, (size == -1 ? std::strlen(name) : size)), 0, nullptr);
		}
		inline value call(const char* name, std::size_t size, std::vector<php::value> argv) {
			return __call(&value_, php::string(name, (size == -1 ? std::strlen(name) : size)), argv.size(), (zval*)argv.data());
		}
		bool is_instance_of(zend_class_entry* ce) const;
		bool is_instance_of(const php::string& class_name) const;
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
