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
	class array_item_assoc;
	class array_item_index;
	class object;
	class buffer;
	class callable;
	class generator;
	
	// @ zend_value / zval 结构参数考 zend/zend_types.h:101
	// 注意：value 及 value 的子类不能出现 virutal 成员
	class value {
	protected:
		zval value_;
	public:
		// ---------------------------------------------------------------------
		~value() {
			_zval_dtor(&value_);
		}
		void reset() {
			_zval_dtor(&value_);
			ZVAL_UNDEF(&value_);
		}
		// 注意：按 php 源码判断，下面复制过程，仅能用于 IS_STRING / IS_ARRAY / IS_CONSTANT_AST
		value clone() {
			value rv;
			ZVAL_DUP(&rv.value_, &value_);
			return std::move(rv);
		}
		value() {
			ZVAL_UNDEF(&value_);
		}
		value(const zval& v) {
			ZVAL_COPY(&value_, &v);
		}
		value(zval&& v) {
			ZVAL_COPY_VALUE(&value_, &v);
			ZVAL_UNDEF(&v);
		}
		value(zval* v, bool create = false) {
			if(create) {
				ZVAL_COPY_VALUE(&value_, v);
			}else{
				ZVAL_COPY(&value_, v);
			}
		}
		value(const value& w) {
			ZVAL_COPY(&value_, &w.value_);
		}
		value(value&& w) {
			ZVAL_COPY_VALUE(&value_, &w.value_);
			ZVAL_UNDEF(&w.value_);
		}
		// 基础类型
		// ---------------------------------------------------------------------
		value(std::nullptr_t v) {
			ZVAL_NULL(&value_);
		}
		value(int v) {
			ZVAL_LONG(&value_, v);
		}
		value(std::uint32_t v) {
			ZVAL_LONG(&value_, v);
		}
		value(std::int64_t v) {
			ZVAL_LONG(&value_, v);
		}
		value(std::size_t v) {
			ZVAL_LONG(&value_, v);
		}
		value(double v) {
			ZVAL_DOUBLE(&value_, v);
		}
		// 字符串
		explicit value(zend_string* s, bool create = false) {
			ZVAL_NEW_STR(&value_, s);
			if(!create) Z_ADDREF(value_);
		}
		value(const std::string& s) {
			ZVAL_NEW_STR(&value_, zend_string_init(s.c_str(), s.length(), false));
		}
		explicit value(const char* str, std::size_t len = -1) {
			ZVAL_NEW_STR(&value_, zend_string_init(str, (len == -1 ? std::strlen(str) : len), false));
		}
		value(buffer&& b);
		// 数组
		// ---------------------------------------------------------------------
		explicit value(zend_array* a, bool create = false) {
			ZVAL_ARR(&value_, a);
			if(!create) Z_ADDREF(value_);
		}
		// 对象
		// ---------------------------------------------------------------------
		explicit value(zend_object* o, bool create = false) {
			ZVAL_OBJ(&value_, o);
			if(!create) Z_ADDREF(value_);
		}
		explicit value(class_base* base);
		explicit value(void* data) {
			ZVAL_PTR(&value_, data);
		}
		// 闭包
		// ---------------------------------------------------------------------
		value(std::function<value (parameters&)> fn);
		// ---------------------------------------------------------------------
		inline zend_uchar type() const {
			return Z_TYPE(value_);
		}
		bool is_empty() const;
		bool is_undefined() const;
		bool is_null() const;
		inline bool is_bool() const {
			return Z_TYPE(value_) == IS_TRUE || Z_TYPE(value_) == IS_FALSE;
		}
		bool is_true() const;
		bool is_false() const;
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
		inline bool is_closure() const {
			return Z_TYPE(value_) == IS_OBJECT && instanceof_function(Z_OBJCE(value_), zend_ce_closure);
		}
		inline bool is_callable() const {
			return zend_is_callable(
				const_cast<zval*>(&value_), IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr);
		}
		inline bool is_generator() const {
			return is_object() && instanceof_function(Z_OBJCE(value_), zend_ce_generator);
		}
		inline bool is_exception() const {
			return is_object() && instanceof_function(Z_OBJCE(value_), zend_ce_exception);
		}
		inline bool is_reference() const {
			return Z_ISREF(value_);
		}
		inline bool is_pointer() const {
			return Z_TYPE(value_) == IS_PTR;
		}
		std::size_t length() const;
		// ---------------------------------------------------------------------
		inline std::uint32_t addref() {
			if(Z_REFCOUNTED(value_)) {
				return Z_ADDREF(value_);
			}
			return 1;
		}
		inline std::uint32_t delref() {
			if(Z_REFCOUNTED(value_)) {
				return Z_DELREF(value_);
			}
			return 1;
		}
		// --------------------------------------------------------------------

		operator int() const;
		operator std::int64_t() const;
		operator std::size_t() const;
		operator double() const;
		operator std::string() const;

		operator zval*() const;
		operator zend_string*() const;
		operator zend_array*() const;
		operator zend_object*() const;
		operator zend_refcounted*() const;
		operator zend_generator*() const;
		template <typename T>
		T* ptr() const {
			return (T*)Z_PTR(value_);
		}
		void ptr(void* p) {
			_zval_dtor(&value_);
			ZVAL_PTR(&value_, p);
		}
		bool         to_bool();
		long         to_long(int base = 10);
		double       to_double();
		// 转换当前 value
		php::string& to_string();
		// ---------------------------------------------------------------------
		value& operator =(const value& v);
		value& operator =(value&& v);
		value& operator =(buffer&& v);
		value& operator =(std::nullptr_t np);
		value& operator =(int v);
		value& operator =(int64_t v);
		value& operator =(double v);
		value& operator =(const std::string& v);
		bool operator ==(const value& v);
	};
	
	extern value BOOL_TRUE;
	extern value BOOL_FALSE;
}
