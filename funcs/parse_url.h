#pragma once
namespace php {
	std::shared_ptr<php_url> parse_url(const char* url, size_t url_len);
}
