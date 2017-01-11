#include "string.h"

namespace core
{
namespace types
{
	string::string():string("", 0)
	{

	}

	string::string(const char* v):string(v, std::strlen(v))
	{

	}

	string::string(const char* v, std::size_t l)
	{
		// 多申请一些（减少后续频繁申请）
		_len = (l + 0x3F) >> 6 << 6;
		_str = zend_string_init(v, l, false);
	}

	string::string(bool v)
	{
		_len = 0x40; // 64;
		_str = zend_string_init(v?"true":"false", v?4:5, false);
	}
	string::string(std::int64_t v)
	{
		_len = 0x40; // 64;
		_str = zend_string_alloc(_len, false);
		_str->len = zend_sprintf(_str->val, "%ld", v);
	}

	string::string(int v):string(std::int64_t(v))
	{
	}

	string::string(double v)
	{
		_len = 0x40; // 64;
		_str = zend_string_alloc(_len, false);
		_str->len = zend_sprintf(_str->val, "%f", v);
	}

	string::string(zend_string* v, bool copy)
	{
		_str = v;
		_len = v->len;
		if(copy) ++GC_REFCOUNT(_str);
	}

	string::string(string&& v)
	{
		_str = v._str;
		_len = v._len;
		
		v._str = nullptr;
		v._len = 0;
	}

	string::string(const string& v)
	{
		_str = v._str;
		_len = v._len;
		++GC_REFCOUNT(_str);
	}

	string::string(value&& v)
	{
		if(!v.is_string())
		{
			_len = 0x40; // 64;
			_str = zend_string_alloc(_len, false);
			_str->len = 0;
		}
		else
		{
			_len = Z_STRLEN_P(v.data());
			_str = Z_STR_P(v.data());
		}
	}

	string::string(const value& v)
	{
		if(!v.is_string())
		{
			_len = 0x40; // 64;
			_str = zend_string_alloc(_len, false);
			_str->len = 0;
		}
		else
		{
			_len = Z_STRLEN_P(v.data());
			_str = Z_STR_P(v.data());
			addref();
		}
	}

	string::~string()
	{
		if(_str != nullptr) {
			zend_string_release(_str); // 引用计数
		}
	}
	// 赋值
	string& string::operator=(const string& v)
	{
		_str = v._str;
		_len = v._len;
		addref();
		return *this;
	}
	string& string::operator=(string&& v)
	{
		_str = v._str;
		_len = v._len;
		v._str = nullptr;
		v._len = 0;
		return *this;
	}
	// 追加
	string& string::operator+=(const string& v)
	{
		if(_len > _str->len + v._str->len)
		{
			// 若当前 buffer 容量足够
			std::memcpy(_str->val + _str->len, v._str->val, v._str->len);
			_str->len += v._str->len;
		}
		else
		{ // 重新申请一块足够的空间
			_len = (_str->len + v._str->len + 0x3F) >> 6 << 6; 
			zend_string* buffer = zend_string_alloc(_len, false);
			std::memcpy(buffer->val, _str->val, _str->len);
			std::memcpy(buffer->val + _str->len, v._str->val, v._str->len);
			buffer->len = _str->len + v._str->len;
			// 释放当前字符串
			zend_string_release(_str);
			_str = buffer;
		}
		return *this;
	}

	string& string::operator+=(string&& v)
	{
		if(_len > _str->len + v._str->len)
		{// this buffer has enough space
			std::memcpy(_str->val + _str->len, v._str->val, v._str->len);
			_str->len += v._str->len;
		}
		else if(v._len > _str->len + v._str->len)
		{// that buffer has enough space
			std::memcpy(v._str->val + _str->len, v._str->val, v._str->len);
			std::memcpy(v._str->val, _str->val, _str->len);
			// 释放当前字符串
			zend_string_release(_str);
			_str = v._str;
			_len = v._len;
		}
		else
		{ // both sides does NOT have enough space
			_len = (_str->len + v._str->len + 0x3F) >> 6 << 6; 
			zend_string* buffer = zend_string_alloc(_len, false);
			std::memcpy(buffer->val, _str->val, _str->len);
			std::memcpy(buffer->val + _str->len, v._str->val, v._str->len);
			buffer->len = _str->len + v._str->len;
			// release current buffer
			zend_string_release(_str);
			_str = buffer;
		}
		v._str = nullptr;
		v._len = 0;
		return *this;
	}

	string string::substr(std::size_t from)
	{
		if(from >= 0)
		{
			return string(_str->val + from, _str->len - from);
		}
		else
		{
			return string(_str->val + _str->len + from, -from);
		}
	}
	string string::substr(std::size_t from, std::size_t count)
	{
		if(from >= 0)
		{
			return string(_str->val + from, std::min(_str->len - from, count));
		}
		else
		{
			return string(_str->val + _str->len + from, std::min(-from, count));
		}
	}

}}
