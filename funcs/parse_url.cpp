#include "../phpext.h"

namespace php {
	std::shared_ptr<php_url> parse_url(const char* url, size_t url_len) {
		return std::shared_ptr<php_url>(php_url_parse_ex(url, url_len), php_url_free);
	}
}
