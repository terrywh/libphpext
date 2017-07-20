#pragma once
namespace php {
	php::string base64_encode(const unsigned char* enc_str, std::size_t enc_len);
	php::string base64_decode(const unsigned char* dec_str, std::size_t dec_len);
	php::string url_encode(const char* enc_str, std::size_t enc_len);
	php::string url_decode(const char* dec_str, std::size_t dec_len);
	void url_decode_inplace(char* dec_str, std::size_t& dec_len);
	php::string hex2bin(unsigned char* src, std::size_t src_len);
	php::string bin2hex(unsigned char* src, std::size_t src_len);
	php::string json_encode(const php::value& data, int options = PHP_JSON_UNESCAPED_UNICODE);
	php::value json_decode(const php::string& str, int depth = PHP_JSON_PARSER_DEFAULT_DEPTH);
}
