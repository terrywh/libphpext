#include "../vendor.h"
#include "value.h"

#ifndef PHPEXT_TYPES_STRING_H
#define PHPEXT_TYPES_STRING_H

namespace phpext
{
namespace types
{

	class string: public value
	{
	public:
		string(const value& v)
		:value(v)
		{
			assert(v.is_string());
		}
		string(value&& v)
		:value(std::move(v))
		{
			assert(v.is_string());
		}
		// 构建字符串
		string()
		:value()
		{
			ZVAL_EMPTY_STRING(val_);
		}
		// 布尔类型转换需要特殊处理
		explicit string(bool v)
		:value()
		{
			zend_string* str = zend_string_alloc(1, 0);
			if(v)
			{
				ZSTR_VAL(str)[0] = '1';
			}
			else
			{
				ZSTR_VAL(str)[1] = '0';
			}
		}
		string(const char* v)
		:string(v, std::strlen(v))
		{}
		string(const char* v, std::size_t len)
		:value()
		{
			zend_string* str = zend_string_alloc(len, 0);
			ZVAL_NEW_STR(val_, str);
			std::memcpy(ZSTR_VAL(str), v, len);
		}
		// 其他类型需要显式转换成字符串
		string(const std::string& v)
		:value()
		{
			zend_string* str = zend_string_alloc(v.length(), 0);
			ZVAL_NEW_STR(val_, str);
			std::memcpy(ZSTR_VAL(str), v.c_str(), ZSTR_LEN(str));
		}
		string(const string& v)
		:value(v)
		{

		}
		string(string&& v)
		:value(std::move(v))
		{

		}
		// assign
		string& operator=(const string& v)
		{
			val_ = v.val_;
			addref();
		}
		string& operator=(string&& v)
		{
			val_ = v.val_;
			v.val_ = nullptr;
		}
		// TODO 优化，考虑使用 smart_str 已减少申请、复制次数？
		// TODO 若优化有效，考虑实现 operator+=(string&& v);
		string& operator+=(const string& v)
		{
			zend_string *target = zend_string_alloc(Z_STRLEN_P(val_) + Z_STRLEN_P(v.val_), 0),
				*origin = Z_STR_P(val_),
				*append = Z_STR_P(v.val_);

			ZVAL_NEW_STR(val_, target);
			std::memcpy(ZSTR_VAL(target), ZSTR_VAL(origin), ZSTR_LEN(origin));
			std::memcpy(ZSTR_VAL(target) + ZSTR_LEN(origin), ZSTR_VAL(append), ZSTR_LEN(append));

			zend_string_release(origin);
		}
		/**
		 * 截取字符串
		 * NOTE: 复制指定区域的内容
		 * @param `from` 可以为负值，指示从尾部截取
		 */
		string substr(std::size_t from)
		{
			if(from >= 0)
			{
				return string(Z_STRVAL_P(val_) + from, Z_STRLEN_P(val_) - from);
			}
			else
			{
				return string(Z_STRVAL_P(val_) + Z_STRLEN_P(val_) + from, -from);
			}
		}
		string substr(std::size_t from, std::size_t count)
		{
			if(from >= 0)
			{
				return string(Z_STRVAL_P(val_) + from, std::min(Z_STRLEN_P(val_) - from, count));
			}
			else
			{
				return string(Z_STRVAL_P(val_) + Z_STRLEN_P(val_) + from, std::min(-from, count));
			}
		}
		inline const char* c_str() const
		{
			return Z_STRVAL_P(val_);
		}
		inline const std::size_t length() const
		{
			return Z_STRLEN_P(val_);
		}
		inline bool is_empty() const
		{
			return !is_string() || length() == 0;
		}
	protected:

	};
}}

#endif // PHPEXT_TYPES_STRING_H
