#include "../phpext.h"

namespace php {
	std::shared_ptr<php_url> parse_url(const char* url, std::size_t url_len) {
		return std::shared_ptr<php_url>(php_url_parse_ex(url, url_len), php_url_free);
	}
	php::string build_query(php::array array) {
		php::string s;
		smart_str   r = {0};
		if(php_url_encode_hash_ex(
			static_cast<zend_array*>(array),
			&r,
			nullptr, 0,
			nullptr, 0,
			nullptr, 0,
			nullptr,
			(char*)"&",
			PHP_QUERY_RFC1738) == FAILURE) {

			if(r.s) {
				smart_str_free(&r);
			}
			return php::string(0);
		}
		if(!r.s) {
			return php::string(0);
		}
		smart_str_0(&r);
		ZVAL_NEW_STR(s, r.s);
		return std::move(s);
	}
}
