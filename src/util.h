#pragma once

namespace php {
	php::string base64_encode(const unsigned char* str, std::size_t len);
	php::string base64_decode(const unsigned char* str, std::size_t len);
	php::string url_encode(const char* str, std::size_t len);
 	php::string url_decode(const char* str, std::size_t len);
	std::size_t url_decode_inplace(char* str, std::size_t len);
	php::string bin2hex(const unsigned char *old, std::size_t len);
	php::string php_hex2bin(const unsigned char *old, const size_t len);
	php::string json_encode(const php::value& val);
	php::value json_decode(const char* str, std::size_t size);
	php::value json_decode(const php::string& str);
	php::string sha1(const unsigned char* enc_str, size_t enc_len);
	php::string sha1(const php::string& str);
	php::string md5(const unsigned char* enc_str, uint32_t enc_len);
	php::string md5(const php::string& str);
	std::uint32_t crc32(const unsigned char* src, uint32_t src_len);
	std::uint32_t crc32(const php::string& str);
	std::shared_ptr<php_url> parse_url(const char* url, std::size_t url_len);
	std::shared_ptr<php_url> parse_url(const php::string& url);
	const std::string& error_type_name(int type);
	php::string uppercase(const char* str, size_t len);
	php::string lowercase(const char* str, size_t len);
	// void uppercase_inplace(char* str, std::size_t len)
	const auto uppercase_inplace = php_strtoupper;
	// void lowercase_inplace(char* str, std::size_t len)
	const auto lowercase_inplace = php_strtolower;
}