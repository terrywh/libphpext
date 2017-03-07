#pragma once

#include "../vendor.h"


namespace php {
class class_base;

template <class T>
class class_wrapper {
public:
	T*          cpp;
	zend_object obj;

	static inline T* from_this(zval* val) {
		class_wrapper<T>* wrapper = reinterpret_cast<class_wrapper<T>*>((char*)Z_OBJ_P(val) - XtOffsetOf(class_wrapper<T>, obj));
		return wrapper->cpp;
	}
};

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
	bool operator==(const value& v) const;
	inline bool is_empty() {
		if(ref_ && val_ == nullptr) return true;
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
		if(ref_) { // 被引用使用的指针可能为空，需要特殊处理
			return val_ == nullptr || Z_TYPE_P(val_) == IS_NULL;
		} else {
			return Z_TYPE_P(val_) == IS_NULL;
		}
	}
	inline void addref() {
		Z_TRY_ADDREF_P(val_); // value may hold non-refcounted types
	}
	inline void delref() {
		Z_TRY_DELREF_P(val_);
	}
	std::size_t length();
	inline zval* data() const {
		return val_;
	}
	// 布尔相关
	// -------------------------------------------------------------------------
	value(bool v);
	value& to_bool();
	operator bool();
	// 整数
	// -------------------------------------------------------------------------
	value(int v);
	value(std::int64_t v);
	value& to_long(int base = 10);
	operator int();
	operator std::int64_t();
	inline bool is_long() const	{
		return ref_ && val_ == nullptr ? false : Z_TYPE_P(val_) == IS_LONG;
	}
	// 判定
	bool operator==(int i) const;
	bool operator==(std::int64_t l) const;
	bool operator>(int i) const;
	bool operator<(int i) const;
	bool operator>(std::int64_t l) const;
	bool operator<(std::int64_t l) const;
	// 浮点
	// -------------------------------------------------------------------------
	value(double v);
	value& to_double();
	operator double();
	inline bool is_double() const {
		return ref_ && val_ == nullptr ? false : Z_TYPE_P(val_) == IS_DOUBLE;
	}
	// 判定
	bool operator==(double d) const;
	bool operator>(double d) const;
	bool operator<(double d) const;
	// 字符串
	// -------------------------------------------------------------------------
	value(const char* str); // 没有 persistent 参数，防止和 str,len 混淆
	value(const std::string& str);
	value(const char* str, std::size_t len, bool persistent = false);
	value(buffer&& buf);
	value(const buffer& buf);
	// 直接构造 zend_string
	static value string(int size);
	value& to_string();
	operator const char*();
	operator zend_string*();
	// 赋值字符串均为 非持久 non-persistent 类型
	value& operator+=(const char* str);
	value& operator+=(const std::string& str);
	// 当 from|count < 0 时，从尾部计数
	value substr(int from, int count = 0);
	void string_length(unsigned int size);
	inline bool is_string() const {
		return ref_ && val_ == nullptr ? false : Z_TYPE_P(val_) == IS_STRING;
	}
	// 判定
	bool operator==(const char* str);
	bool operator==(const std::string str);
	bool equal(const char* str, std::size_t len);
	bool operator>(const char* str);
	bool operator>(const std::string str);
	bool operator<(const char* str);
	bool operator<(const std::string str);
	int compare(const char* str, std::size_t len);
	// 数组
	// -------------------------------------------------------------------------
	static value array(std::size_t size, bool persistent = false);
	bool isset(const char* key);
	bool isset(const char* key, std::size_t len);
	bool isset(const std::string& key);
	void unset(const char* key);
	void unset(const char* key, std::size_t len);
	void unset(const std::string& key);
	bool isset(std::size_t idx);
	void unset(std::size_t idx);
	inline bool is_array() const {
		return ref_ && val_ == nullptr ? false : Z_TYPE_P(val_) == IS_ARRAY;
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
		std::vector<zval> argv2;
		int argc = sizeof...(Args);
		for(int i=0;i<argc;++i)
		{
			argv2.push_back(*argv1[i].data());
		}
		return invoke_(argc, (zval*)argv2.data());
	}
	value operator() ();
	inline bool is_callable() const	{
		return ref_ && val_ == nullptr ? false : zend_is_callable(val_, IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr);
	}
	// 对象
	// -------------------------------------------------------------------------
	value(class_base* base);
	value prop(const char* name);
	value prop(const char* name, std::size_t len);
	// returns *this;
	value& prop(const char* name, const value& val);
	value& prop(const char* name, std::size_t len, const value& val);

	value call(const char* name);
	value call(const char* name, std::size_t len);
	template <typename ...Args>
	value call(const char* name, const Args&... argv) {
		call(name, std::strlen(name), argv...);
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
	inline bool is_object() const {
		return ref_ && val_ == nullptr ? false : Z_TYPE_P(val_) == IS_OBJECT;
	}
	inline bool is_instance_of(const std::string& class_name) const {
		if(ref_ && val_ == nullptr || Z_TYPE_P(val_) != IS_OBJECT) return false;
		zend_string*      cn = zend_string_init(class_name.c_str(), class_name.length(), false);
		zend_class_entry* ce = zend_lookup_class(cn);
		zend_string_release(cn);
		return instanceof_function(Z_OBJCE_P(val_), ce);
	}
	template <class T>
	T* native() {
		if(!is_object()) return nullptr;
		return class_wrapper<T>::from_this(val_);
	}
protected:
	// 通用
	// -------------------------------------------------------------------------
	value()
	: val_(&value_)
	, ref_(false) {
		// 请在申请后立刻为 val_ 赋值
		// ZVAL_UNDEF(val_);
	}
	zval* val_;
	bool  ref_;
	// 文本
	// -------------------------------------------------------------------------
	value& append_(const char* str, std::size_t len);
	// 数组
	// -------------------------------------------------------------------------
	// 回调
	// -------------------------------------------------------------------------
	value invoke_(int argc, zval* argv);
	// 对象
	// -------------------------------------------------------------------------
	value call_(const char* name, std::size_t len, int argc, zval* argv);

private:
	zval value_;
};

}
