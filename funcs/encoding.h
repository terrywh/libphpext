#pragma once
namespace php {
	std::shared_ptr<zend_string> base64_encode(const unsigned char* enc_str, size_t enc_len);
	std::shared_ptr<zend_string> base64_decode(const unsigned char* dec_str, size_t dec_len);
	std::shared_ptr<zend_string> url_encode(const char* enc_str, size_t enc_len);
	std::shared_ptr<zend_string> url_decode(char* dec_str, size_t& dec_len);
	std::shared_ptr<zend_string> hex2bin(unsigned char* src, size_t src_len);
	std::shared_ptr<zend_string> bin2hex(unsigned char* src, size_t src_len);
	std::shared_ptr<zend_string> json_encode(zval* val);
	php::value json_decode(char* src, std::size_t src_len);
}
