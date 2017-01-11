#include <cstdint>
#include <cmath>
#include <cstring>
#include <boost/pool/pool.hpp>

using std::isnan;
#include <zend.h>
#include <zend_API.h>

#include "../memory.h"

#ifndef CORE_TYPES_VALUE_H
#define CORE_TYPES_VALUE_H

namespace core
{
namespace types
{

	// @ zend_value / zval 结构参数考 zend/zend_types.h:101
class value
{
	public:
		value();
		value(std::nullptr_t v); // for null
		value(value&& v);
		value(const value& v);
		value(zval* v, bool copy = true);
		value(bool v);
		value(int v);
		value(std::int64_t v);
		value(double v);
		value(const char* v);
		virtual ~value();
		// primitive types convertor
		operator bool();
		operator std::int64_t();
		operator double();
		operator char*();
		// non-primitive types convertor provided by type construtors

		// assign
		value& operator=(const value& v);
		value& operator=(value&& v);
		// assign to other primitive values will:
		// 1. construct a new value
		// 2. assign to that value
		// by using move assign operator this almost has zero overhead
		// assign to non-primitive values will:
		// 1. convert to value
		// 2. assign to that value

		// 判断
		bool operator==(const value& v);
		bool is_empty();
		inline bool is_null() const
		{
			return Z_TYPE_P(val_) == IS_NULL;
		}
		inline bool is_string() const
		{
			return Z_TYPE_P(val_) == IS_STRING;
		}
		inline bool is_long() const
		{
			return Z_TYPE_P(val_) == IS_LONG;
		}
		inline bool is_double() const
		{
			return Z_TYPE_P(val_) == IS_DOUBLE;
		}
		inline bool is_array() const
		{
			return Z_TYPE_P(val_) == IS_ARRAY;
		}
		inline bool is_callable() const
		{
			return zend_is_callable(val_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr);
		}

		inline void addref()
		{
			Z_TRY_ADDREF_P(val_); // value may hold non-refcounted types
		}
		inline void delref()
		{
			Z_TRY_DELREF_P(val_);
		}
		inline zval* data() const
		{
			return val_;
		}
		inline zval* data(zval* v)
		{
			val_ = v;
		}
		// specify custom allocator with zend engine
		static boost::pool<user_allocator_zend> pool;
	protected:
		zval* val_;

	};
}}

#endif
