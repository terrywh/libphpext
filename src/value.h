#pragma once

#include "type.h"
#include "class.h"

namespace php {
	class class_base;
	class parameters;
	class buffer;
	class stream_buffer;
	class value {
	protected:
		zval  val_;
		zval* ptr_;
	public:
		// ---------------------------------------------------------------------
		virtual ~value();
		// ---------------------------------------------------------------------
		value(); // undefined
		value(std::nullptr_t v);
		value(class_base* v);
		value(zval* v, bool ptr = false);
		value(const zend_string* v);
		value(smart_str* s);
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
		// value(const value& w, const TYPE& t); // value_ex.hpp
		// value(value&& w, const TYPE& t); // value_ex.hpp
		// value(const value& w, const CLASS& ce); // value_ex.hpp
		// value(value&& w, const CLASS& ce); // value_ex.hpp
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
		value(buffer&& v);
		value(stream_buffer&& v);
		value(std::function<value (parameters& params)> c); // value_impl.hpp
		// 赋值
		// -------------------------------------------------------------------
		value& operator =(const value& v);
		value& operator =(value&& v);
		// 功能项
		// ====================================================================
		// 检查
		bool empty() const;
		std::size_t length() const;
		std::size_t size() const;
		TYPE typeof() const;
		bool typeof(const TYPE& t) const;
		CLASS classof() const;
		bool instanceof(const CLASS& c) const;
		// 读取
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
		POINTER_TYPE* pointer() const {
			assert(typeof(TYPE::POINTER));
			return reinterpret_cast<POINTER_TYPE*>(Z_PTR_P(ptr_));
		}
		// 强制转换
		bool         to_boolean();
		std::int64_t to_integer(int base = 10);
		double       to_float();
		std::string  to_string();
		// 判定
		bool operator ==(const value& v) const;
		bool operator !=(const value& v) const;
		// 引用
		// ---------------------------------------------------------------------
		std::uint32_t addref() const;
		std::uint32_t delref();
		// 制作引用 (当前对象持有也会变, 但 ptr_ 对应不便)
		value make_ref();
		// --------------------------------------------------------------------
		friend std::ostream& operator << (std::ostream& os, const php::value& data);
	};
}
