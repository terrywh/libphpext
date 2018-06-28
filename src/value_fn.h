#pragma once

#include "type.h"
#include "class.h"
#include "value.h"
#include "string.h"
#include "exception.h"

namespace php {
	class value_fn {
	public:
		virtual value ptr() const = 0;
		virtual zval* raw() const = 0;
		// --------------------------------------------------------------------
		// 检查
		bool empty() const {
			return ptr().empty();
		}
		std::size_t length() const {
			return ptr().length();
		}
		std::size_t size() const {
			return ptr().size();
		}
		TYPE typeof() const {
			return ptr().typeof();
		}
		bool typeof(const TYPE& t) const {
			return ptr().typeof(t);
		}
		CLASS classof() const {
			return ptr().classof();
		}
		bool instanceof(const CLASS& c) const {
			return ptr().instanceof(c);
		}
		// 读取
		operator bool() const {
			return ptr().operator bool();
		}
		operator int() const {
			return ptr().operator int();
		}
		operator std::int64_t() const {
			return ptr().operator std::int64_t();
		}
		operator std::size_t() const {
			return ptr().operator std::size_t();
		}
		operator float() const {
			return ptr().operator float();
		}
		operator double() const {
			return ptr().operator double();
		}
		operator std::string() const {
			return ptr().operator std::string();
		}
		operator zval*() const {
			return ptr().operator zval*();
		}
		operator zend_string*() const {
			return ptr().operator zend_string*();
		}
		operator zend_object*() const {
			return ptr().operator zend_object*();
		}
		operator zend_array*() const {
			return ptr().operator zend_array*();
		}
		operator zend_class_entry*() const {
			return ptr().operator zend_class_entry*();
		}
		template <typename POINTER_TYPE>
		POINTER_TYPE* pointer() const {
			return ptr().pointer<POINTER_TYPE>();
		}
		// 强制转换
		bool to_boolean() {
			return ptr().to_boolean();
		}
		std::int64_t to_integer(int base = 10) {
			return ptr().to_integer();
		}
		double to_float() {
			return ptr().to_float();
		}
		std::string to_string() {
			return ptr().to_string();
		}
		// 判定
		bool operator ==(const value& v) const {
			return ptr() == v;
		}
		bool operator !=(const value& v) const {
			return ptr() != v;
		}
	};
}