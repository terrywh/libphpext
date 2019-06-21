#pragma once

#include "value.h"
#include "string.h"
#include "object.h"

namespace php {
	extern std::ostream& operator << (std::ostream& os, const php::value& data);
	object datetime(std::int64_t now = 0);
	object datetime(const char* datetime);
	string base64_encode(const unsigned char* str, std::size_t len);
	string base64_decode(const unsigned char* str, std::size_t len);
	string url_encode(const char* str, std::size_t len);
 	string url_decode(const char* str, std::size_t len);
	std::size_t url_decode_inplace(char* str, std::size_t len);
	string bin2hex(const unsigned char *old, std::size_t len);
	string php_hex2bin(const unsigned char *old, const size_t len);
	string json_encode(const value& val);
	value json_decode(const char* str, std::size_t size);
	value json_decode(const string& str);
	void sha1(const unsigned char* enc_str, size_t enc_len, char* output);
	string sha1(const string& str);
	void md5(const unsigned char* enc_str, uint32_t enc_len, char* output);
	string md5(const string& str);
	std::uint32_t crc32(const unsigned char* src, uint32_t src_len);
	std::uint32_t crc32(const string& str);
	/*
	typedef struct php_url {
		char *scheme;
		char *user;
		char *pass;
		char *host;
		unsigned short port;
		char *path;
		char *query;
		char *fragment;
	} php_url;
	*/
	typedef struct php_url url;
	std::shared_ptr<url> parse_url(const char* url, std::size_t url_len);
	std::shared_ptr<url> parse_url(const string& url);
	const std::string& error_type_name(int type);
	string uppercase(const char* str, size_t len);
	string lowercase(const char* str, size_t len);
	// void uppercase_inplace(char* str, std::size_t len)
	const auto uppercase_inplace = php_strtoupper;
	// void lowercase_inplace(char* str, std::size_t len)
	const auto lowercase_inplace = php_strtolower;
}