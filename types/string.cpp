#include "../phpext.h"

namespace php {
	string string::substr(std::size_t pos, std::size_t count) {
		if(count == 0) {
			count = length() - pos;
		}
		return string(Z_STRVAL(value_) + pos, count);
	}
	static zend_string* concat_zend_string(zend_string* s1, zend_string* s2) {
		zend_string* s3 = zend_string_alloc(s1->len + s2->len, false);
		std::memcpy(s3->val, s1->val, s1->len);
		std::memcpy(s3->val + s1->len, s2->val, s2->len);
		return s3;
	}
	string concat(const string& s1, const string& s2) {
		string str;
		concat_zend_string(
			static_cast<zend_string*>(const_cast<string&>(s1)),
			static_cast<zend_string*>(const_cast<string&>(s2))
		);
		return std::move(str);
	}
}
