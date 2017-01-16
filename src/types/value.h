#pragma once

#include "../vendor.h"

namespace php {
// @ zend_value / zval 结构参数考 zend/zend_types.h:101
class value {
public:
	// 通用
	// -------------------------------------------------------------------------
	value(zval* v, bool ref = false);
	value(std::nullptr_t);
	value(const value& w);
	value(value&& w);
	virtual ~value();
	value& operator=(const value& v);
	value& operator=(value&& v);
	inline bool operator==(const value& v) {
		return Z_PTR_P(val_) == Z_PTR_P(v.val_);
	}
	inline bool is_empty() {
		switch(Z_TYPE_P(val_)) {
			case IS_UNDEF:
			case IS_NULL:
			case IS_FALSE:
				return true;
			case IS_LONG:
			case IS_DOUBLE:
				return Z_LVAL_P(val_) == 0;
			case IS_STRING:
				return Z_STRLEN_P(val_) == 0;
			case IS_ARRAY:
				return Z_ARRVAL_P(val_)->nNumOfElements == 0;
			default: // TODO how to determine 'empty' for other types?
				return false;
		}
	}
	inline bool is_null() const {
		return Z_TYPE_P(val_) == IS_NULL;
	}
	inline void addref() {
		Z_TRY_ADDREF_P(val_); // value may hold non-refcounted types
	}
	inline void delref() {
		Z_TRY_DELREF_P(val_);
	}
	inline std::size_t length() {
		switch(Z_TYPE_P(val_)) {
			case IS_UNDEF:
			case IS_NULL:
			case IS_FALSE:
				return 0l;
			case IS_LONG:
			case IS_DOUBLE:
				return 0l;
			case IS_STRING:
				return Z_STRLEN_P(val_);
			case IS_ARRAY:
				return zend_hash_num_elements(Z_ARRVAL_P(val_));
			default: // TODO length for other types?
				return 0;
		}
	}
	inline zval* data() const {
		return val_;
	}
	// 布尔相关
	// -------------------------------------------------------------------------
	value(bool v);
	value& to_bool();
	operator bool();
	value& operator= (bool b);
	// 整数
	// -------------------------------------------------------------------------
	value(int v);
	value(std::int64_t v);
	value& to_long(int base = 10);
	operator int();
	operator std::int64_t();
	value& operator= (int i);
	value& operator= (std::int64_t l);
	inline bool is_long() const	{
		return Z_TYPE_P(val_) == IS_LONG;
	}
	// TODO 判定
	// 浮点
	// -------------------------------------------------------------------------
	value(double v);
	value& to_double();
	operator double();
	value& operator= (double d);
	inline bool is_double() const {
		return Z_TYPE_P(val_) == IS_DOUBLE;
	}
	// TODO 判定
	// 字符串
	// -------------------------------------------------------------------------
	value(const char* str);
	value(const char* str, std::size_t len);
	value(const std::string& str);
	value& to_string();
	operator const char*();
	value& operator= (const char* str);
	value& operator= (const std::string& str);
	value& operator+=(const char* str);
	value& operator+=(const std::string& str);
	// 当 from|count < 0 时，从尾部计数
	value substr(int from, int count = 0);
	inline bool is_string() const {
		return Z_TYPE_P(val_) == IS_STRING;
	}
	// TODO 判定
	// 数组
	// -------------------------------------------------------------------------
	static value array(std::size_t size);
	bool isset(const char* key);
	bool isset(const char* key, std::size_t len);
	bool isset(const std::string& key);
	void unset(const char* key);
	void unset(const char* key, std::size_t len);
	void unset(const std::string& key);
	bool isset(std::size_t idx);
	void unset(std::size_t idx);
	inline bool is_array() const {
		return Z_TYPE_P(val_) == IS_ARRAY;
	}
	// 注：若指定 index/key 对应的 key 不存在会自动创建 UNDEF 填充引用
	value operator[] (std::size_t index);
	value operator[] (const char* key);
	value operator[] (const std::string& key);
	// TODO 判定
	// 回调
	// -------------------------------------------------------------------------
	static value callable(const char* str);
	static value callable(const char* str, std::size_t len);
	static value callable(const std::string& str);
	template <typename ...Args>
	value operator() (const Args&... argv)
	{
		value argv1[] = { static_cast<value>(argv)... };
		std::vector<zval*> argv2;
		int argc = sizeof...(Args);
		for(int i=0;i<argc;++i)
		{
			argv2.push_back(argv1[i].data());
		}
		return invoke_(argc, (zval*)argv2.data());
	}
	value operator() ();
	inline bool is_callable() const	{
		return zend_is_callable(val_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr);
	}
	// 对象
	// -------------------------------------------------------------------------
	value prop(const char* name);
	value prop(const std::string& name);
	value prop(const char* name, std::size_t len);
	// returns *this;
	value& prop(const char* name, const value& val);
	value& prop(const std::string& name, const value& val);
	value& prop(const char* name, std::size_t len, const value& val);

	value call(const char* name);
	value call(const std::string& name);
	value call(const char* name, std::size_t len);
	template <typename ...Args>
	value call(const char* name, const Args&... argv) {
		call(name, std::strlen(name), argv...);
	}
	template <typename ...Args>
	value call(const std::string& name, const Args&... argv) {
		call(name.c_str(), name.length(), argv...);
	}
	template <typename ...Args>
	value call(const char* name, std::size_t len, const Args&... argv) {
		value argv1[] = { static_cast<value>(argv)... };
		std::vector<zval*> argv2;
		int argc = sizeof...(Args);
		for(int i=0;i<argc;++i)
		{
			argv2.push_back(argv1[i].data());
		}
		return call_(name, len, argc, (zval*)argv2.data());
	}
protected:
	// 通用
	// -------------------------------------------------------------------------
	value();
	zval* val_;
	bool  ref_;
	// 文本
	// -------------------------------------------------------------------------
	value& assign_(const char* str, std::size_t len);
	value& append_(const char* str, std::size_t len);
	// 数组
	// -------------------------------------------------------------------------
	// 回调
	// -------------------------------------------------------------------------
	value invoke_(int argc, zval* argv);
	// 对象
	// -------------------------------------------------------------------------
	value call_(const char* name, std::size_t len, int argc, zval* argv);

};

}
