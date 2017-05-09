#include "../phpext.h"

namespace php {
	string::~string() {
		if(str_ != nullptr) {
			zend_string_release(str_); // --GC_REFCOUNT(str_) == 0 -> zend_string_free
		}
	}
	string::string(zend_string* str):str_(str) {
		addref();
	}
	string string::clone(const string& str) {
		return string(str.c_str(), str.length());
	}
	string::string(const string& str):str_(str.str_) {
		addref();
	}
	string::string(string&& str):str_(str.str_) {
		str.str_ = nullptr;
	}
	string::string(std::size_t size, bool p)
		:str_(zend_string_alloc(size,p)) {}
	string::string(const char* val, std::size_t len, bool p)
		:str_(zend_string_init(val, len, p)) {}
	string::string(const std::string& str)
		:str_(zend_string_init(str.c_str(), str.length(), false)) {}
	string string::substr(std::size_t pos, std::size_t count) {
		if(count == -1) {
			count = length() - pos;
		}
		return string(str_->val + pos, count);
	}
	zend_string* string::concat(zend_string* s1, zend_string* s2) {
		zend_string* s3 = zend_string_alloc(s1->len + s2->len, false);
		std::memcpy(s3->val, s1->val, s1->len);
		std::memcpy(s3->val + s1->len, s2->val, s2->len);
		return s3;
	}
	string& string::operator=(const string& cb) {
		str_ = cb.str_;
		addref();
		return *this;
	}
	string& string::operator=(string&& cb) {
		str_ = cb.str_;
		cb.str_ = nullptr;
		return *this;
	}
}
