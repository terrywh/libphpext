#pragma once

namespace php {
	class class_base;
	class parameters;
	class value {
	protected:
		zval  value_ins;
	public:
		// ---------------------------------------------------------------------
		virtual ~value();
		// ---------------------------------------------------------------------
		value();
		value(std::nullptr_t v);
		value(class_base* v);
		value(const zval* v);
		value(const zend_string* v);
		value(const zend_object* v);
		value(zend_class_entry* e);
		value(const CLASS& c);
		value(zend_class_entry* e, std::vector<value> argv);
		value(const CLASS& c, std::vector<value> argv);
		value(const zend_array* v);
		explicit value(const void* data);
		value(const value& w);
		value(value&& w);
		// 类型检查构造
		value(const value& w, const TYPE& t); // value_ex.hpp
		value(value&& w, const TYPE& t); // value_ex.hpp
		value(const value& w, const CLASS& ce); // value_ex.hpp
		value(value&& w, const CLASS& ce); // value_ex.hpp
		// 基础类型
		// ---------------------------------------------------------------------
		value(bool v);
		value(int v);
		value(std::uint32_t v);
		value(std::int64_t v);
		value(std::size_t v);
		value(double v);
		value(const char* str);
		value(const std::string& str);
		value(std::function<value (parameters& params)> c); // value_impl.hpp
		// ---------------------------------------------------------------------
		bool empty() const;
		std::size_t length() const;
		std::size_t size() const;
		// --------------------------------------------------------------------
		TYPE typeof() const;
		bool typeof(const TYPE& t) const;
		CLASS classof() const;
		bool instanceof(const CLASS& c) const;
		// 转换
		// ---------------------------------------------------------------------
		operator bool() const;
		operator int() const;
		operator std::int64_t() const;
		operator std::size_t() const;
		operator float() const;
		operator double() const;
		operator std::string() const;
		operator zval*() const;
		operator zend_string*() const;
		operator zend_object*() const;
		operator zend_array*() const;
		operator zend_class_entry*() const;
		template <typename POINTER_TYPE>
		POINTER_TYPE* ptr() const {
			assert(typeof(TYPE::POINTER));
			return reinterpret_cast<POINTER_TYPE*>(Z_PTR(value_ins));
		}
		// (无类型检查)转换
		// ---------------------------------------------------------------------
		bool         to_boolean();
		std::int64_t to_integer(int base = 10);
		double       to_float();
		std::string  to_string();
		// 引用
		// ---------------------------------------------------------------------
		std::uint32_t addref() const;
		std::uint32_t delref();
		// 赋值
		// -------------------------------------------------------------------
		value& operator =(const value& v);
		value& operator =(value&& v);
		value& operator =(const zval* v);
		// 带类型检查
		void assign(const value& v, const TYPE& t);
		void assign(value&& v, const TYPE& t);
		void assign(const value& v, const CLASS& t);
		void assign(value&& v, const CLASS& t);
		void ptr(void* p);
		// ---------------------------------------------------------------------
		bool operator ==(const value& v) const;
		bool operator !=(const value& v) const;
		// --------------------------------------------------------------------
	};
}
