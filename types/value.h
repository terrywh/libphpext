#pragma once

namespace php {
	class class_base;
	template <class ClassT>
	class class_entry;
	class parameters;
	template <class T>
	struct class_wrapper;
	class string;
	class array;
	class object;
	class buffer;
	class callable;

	// @ zend_value / zval 结构参数考 zend/zend_types.h:101
	class value {
	protected:
		zval value_;
	public:
		// -------------------------------------------------------------------------
		~value();
		value();
		value(std::nullptr_t v);
		value(const zval& v);
		value(zval&& v);
		value(const value& w);
		value(value&& w);
		value(zend_string* str);
		value(const string& str);
		value(string&& str);
		value(buffer&& buf);
		value(zend_array* arr);
		value(const array& arr);
		value(array&& arr);
		value(zend_object* obj);
		value(const object& obj);
		value(object&& obj);
		value(bool v);
		value(int v);
		value(std::int64_t v);
		value(double v);
		value(const std::string& str);
		value(const char* str, std::size_t len, bool persistent = false);
		value(class_base* base);
		// 闭包 closure
		value(std::function<value (parameters&)> fn);
		// -------------------------------------------------------------------------
		inline zend_uchar type() const {
			return Z_TYPE(value_);
		}
		bool is_empty() const;
		bool is_null() const;
		inline bool is_bool() const {
			return Z_TYPE(value_) == IS_TRUE || Z_TYPE(value_) == IS_FALSE;
		}
		inline bool is_long() const {
			return Z_TYPE(value_) == IS_LONG;
		}
		inline bool is_double() const {
			return Z_TYPE(value_) == IS_DOUBLE;
		}
		inline bool is_string() const {
			return Z_TYPE(value_) == IS_STRING;
		}
		inline bool is_array() const {
			return Z_TYPE(value_) == IS_ARRAY;
		}
		inline bool is_object() const {
			return Z_TYPE(value_) == IS_OBJECT;
		}
		inline bool is_callable() {
			return zend_is_callable(&value_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr);
		}
		std::size_t length() const;
		inline void addref() {
			Z_TRY_ADDREF(value_);
		}
		inline void delref() {
			Z_TRY_DELREF(value_);
		}
		inline operator zval*() {
			return &value_;
		}
		// bool is_true() const;
		// inline bool is_false() const {
		// 	return !is_true();
		// }
		bool is_true() const;
		operator int() const;
		operator std::int64_t() const;
		operator double() const;
		operator zend_string*();
		operator std::string() const;
		// !!! 此处返回的是对当前字符串的引用 GC_REFCOUNT()++
		operator string();
		operator zend_array*();
		// !!! 此处返回的是对当前数组的引用 GC_REFCOUNT()++
		operator array();
		operator zend_object*();
		// !!! 此处返回的是对当前对象的引用 GC_REFCOUNT()++
		operator object();
		// !!! 次数返回的是对当前对象的引用 GC_REFCOUNT()++
		operator callable();
		bool to_bool();
		long to_long(int base = 10);
		double to_double();
		zend_string* to_string();
		bool is_instance_of(const std::string& class_name) const;
		template<class T>
		inline bool is_instance_of() const {
			return Z_TYPE(value_) == IS_OBJECT &&
				Z_OBJ(value_)->handlers == &class_entry<T>::handlers_;
		}
		template <class T>
		inline T* native() {
			return Z_TYPE(value_) == IS_OBJECT ? class_wrapper<T>::from_this(&value_) : nullptr;
		};
		value& operator =(const value& v);
		value& operator =(value&& v);
	};

}
