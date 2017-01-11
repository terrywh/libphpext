#include <cstring>
#include <zend.h>
#include "value.h"

#ifndef CORE_TYPES_STRING_H
#define CORE_TYPES_STRING_H

namespace core
{
namespace types
{

	class string
	{
	public:
		// 构建字符串
		string();
		string(const char* v);
		string(const char* v, std::size_t l);
		string(bool v);
		string(std::int64_t v);
		string(int v);
		string(double v);
		string(zend_string* v, bool copy = true);
		string(string&& v);
		string(const string& v);
		string(value&& v);
		string(const value& v);
		~string();
		inline const char* c_str() const
		{
			return _str->val;
		}
		inline const std::size_t length() const
		{
			return _str->len;
		}
		inline bool empty() const
		{
			return _str->len == 0;
		}
		// assign
		string& operator=(const string& v);
		string& operator=(string&& v);
		// append
		string& operator+=(const string& v);
		string& operator+=(string&& v);
		/**
		 * takes partial of the current string
		 * NOTE: `substr()` alway makes copy of the specified buffer
		 * @param `from` can be negative, and will be count charactors backwards
		 */
		string substr(std::size_t from);
		/**
		 * @see substr(std::size_t)
		 */
		string substr(std::size_t from, std::size_t count);

		inline void addref()
		{
			++GC_REFCOUNT(_str);
		}
		inline void delref() 
		{
			--GC_REFCOUNT(_str);
		}
		inline zend_string* data() const
		{
			return _str;
		}
	protected:
		zend_string* _str;
		size_t       _len; // 实际内存空间
	};
}}

#endif // CORE_TYPES_STRING_H
