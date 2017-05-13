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
		value(bool v) {
			if(v) ZVAL_TRUE(&value_);
			else ZVAL_FALSE(&value_);
		}
		value(int v) {
			ZVAL_LONG(&value_, v);
		}
		value(std::int64_t v) {
			ZVAL_LONG(&value_, v);
		}
		value(zend_ulong v) {
			ZVAL_LONG(&value_, v);
		}
		value(double v) {
			ZVAL_DOUBLE(&value_, v);
		}
		// 字符串
		value(zend_string* s, bool create = false) {
			ZVAL_STR(&value_, s);
			if(!create) Z_ADDREF(value_);
		}
		value(const std::string& s) {
			ZVAL_NEW_STR(&value_, zend_string_init(s.c_str(), s.length(), false));
		}
		value(const char* str, std::size_t len, bool persistent = false) {
			ZVAL_NEW_STR(&value_, zend_string_init(str, len, persistent));
		}
		value(buffer&& b);
		// 数组
		// ---------------------------------------------------------------------
		value(zend_array* a, bool create = false) {
			ZVAL_ARR(&value_, a);
			if(!create) Z_ADDREF(value_);
		}
		// 对象
		// ---------------------------------------------------------------------
		value(zend_object* o, bool create = false) {
			ZVAL_OBJ(&value_, o);
			if(!create) Z_ADDREF(value_);
		}
		value(class_base* base);
		// 闭包
		// ---------------------------------------------------------------------
		value(std::function<value (parameters&)> fn);
		// ---------------------------------------------------------------------
		inline zend_uchar type() const {
			return Z_TYPE(value_);
		}
		bool is_empty() const;
		bool is_null() const;
		inline bool is_bool() const {
			return Z_TYPE(value_) == IS_TRUE || Z_TYPE(value_) == IS_FALSE;
		}
		bool is_true() const;
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
		inline bool is_generator() const {
			return is_object() && instanceof_function(Z_OBJCE(value_), zend_ce_generator);
		}
		inline bool is_exception() const {
			return is_object() && instanceof_function(Z_OBJCE(value_), zend_ce_exception);
		}
		inline bool is_reference() const {
			return Z_ISREF(value_);
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
		operator zval*();
		operator int() const;
		operator std::int64_t() const;
		operator std::size_t() const;
		operator double() const;
		operator zend_string*();
		operator std::string() const;
		// !!! 此处返回的是对当前字符串的引用 GC_REFCOUNT()++
		operator string&();
		operator zend_array*();
		operator array&();
		operator zend_object*();
		operator object&();
		operator callable&();
		operator zend_refcounted*();
		operator zend_generator*();
		operator generator&();
		bool         to_bool();
		long         to_long(int base = 10);
		double       to_double();
		std::string  to_string();
		// ---------------------------------------------------------------------
		value& operator =(const value& v);
		value& operator =(value&& v);
	};

}
