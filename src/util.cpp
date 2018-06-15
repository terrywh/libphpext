#include "phpext.h"

namespace php {
	php::string base64_encode(const unsigned char* str, std::size_t len) {
		return php::string(php_base64_encode(str, len));
	}
	php::string base64_decode(const unsigned char* str, std::size_t len) {
		return php::string(php_base64_decode(str, len));
	}
	php::string url_encode(const char* str, std::size_t len) {
		return php::string(php_url_encode(str, len));
	}
 	php::string url_decode(const char* str, std::size_t len) {
		php::string dec(str, len);
		dec.shrink(php_url_decode(dec.data(), len));
		return std::move(dec);
	}
	std::size_t url_decode_inplace(char* str, std::size_t len) {
		return php_url_decode(str, len);
	}
	php::string bin2hex(const unsigned char *old, std::size_t len) {
		php::string str(2*len);
		// md5.c
		make_digest_ex(str.data(), old, len);
		return std::move(str);
	}
	php::string php_hex2bin(const unsigned char *old, const size_t len)	{
		size_t target_length = len >> 1;
		php::string s(target_length);
		unsigned char *ret = (unsigned char*)s.data();
		size_t i, j;

		for (i = j = 0; i < target_length; i++) {
			unsigned char c = old[j++];
			unsigned char l = c & ~0x20;
			int is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
			unsigned char d;

			/* basically (c >= '0' && c <= '9') || (l >= 'A' && l <= 'F') */
			if(! EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
				throw exception(zend_ce_error, "invalid hex string");
			}
			d = (l - 0x10 - 0x27 * is_letter) << 4;
			c = old[j++];
			l = c & ~0x20;
			is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
			if(!EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
				throw exception(zend_ce_error, "invalid hex string");
			}
			d |= l - 0x10 - 0x27 * is_letter;
			ret[i] = d;
		}
		ret[i] = '\0';

		return std::move(s);
	}
	php::string json_encode(const php::value& val) {
		php::buffer str;
		php_json_encode(str, val, PHP_JSON_UNESCAPED_UNICODE);
		return std::move(str);
	}
	php::value json_decode(const char* str, std::size_t size) {
		php::value rv;
		php_json_decode_ex(rv, const_cast<char*>(str), size, PHP_JSON_OBJECT_AS_ARRAY, PHP_JSON_PARSER_DEFAULT_DEPTH);
		return std::move(rv);
	}
	php::value json_decode(const php::string& str) {
		php::value rv;
		php_json_decode_ex(rv, const_cast<char*>(str.data()), str.size(), PHP_JSON_OBJECT_AS_ARRAY, PHP_JSON_PARSER_DEFAULT_DEPTH);
		return std::move(rv);
	}
	void sha1(const unsigned char* enc_str, size_t enc_len, char* output) {
		PHP_SHA1_CTX context;
		unsigned char digest[20];

		PHP_SHA1Init(&context);
		PHP_SHA1Update(&context, enc_str, enc_len);
		PHP_SHA1Final(digest, &context);
		make_digest_ex(output, digest, 20);
		output[40] = '\0';
	}
	php::string sha1(const php::string& str) {
		php::string s(40);
		sha1(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), s.data());
		return std::move(s);
	}
	void md5(const unsigned char* enc_str, uint32_t enc_len, char* output) {
		PHP_MD5_CTX context;
		unsigned char digest[16];

		PHP_MD5Init(&context);
		PHP_MD5Update(&context, enc_str, enc_len);
		PHP_MD5Final(digest, &context);
		make_digest_ex(output, digest, 16);
		output[32] = '\0';
	}
	php::string md5(const php::string& str) {
		php::string s(32);
		md5(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), s.data());
		return std::move(s);
	}
	std::uint32_t crc32(const unsigned char* src, uint32_t src_len) {
		uint32_t crc32_val = 0;
		for(; --src_len; ++src) {
			crc32_val = ((crc32_val >> 8) & 0x00FFFFFF) ^ crc32tab[(crc32_val ^ (*src)) & 0xFF ];
		}
		return crc32_val;
	}
	std::uint32_t crc32(const php::string& str) {
		return crc32(reinterpret_cast<const unsigned char*>(str.c_str()), str.size());
	}
	std::shared_ptr<php_url> parse_url(const char* url, std::size_t url_len) {
		return std::shared_ptr<php_url>(php_url_parse_ex(url, url_len), php_url_free);
	}
	std::shared_ptr<php_url> parse_url(const php::string& url) {
		return std::shared_ptr<php_url>(php_url_parse_ex(url.c_str(), url.size()), php_url_free);
	}
	const std::string& error_type_name(int type) {
		static std::string FATAL           {"FATAL"};
		static std::string WARNING       {"WARNING"};
		static std::string ERROR           {"ERROR"};
		static std::string NOTICE         {"NOTICE"};
		static std::string STRICT         {"STRICT"};
		static std::string DEPRECATED {"DEPRECATED"};
		static std::string UNKNOWN       {"UNKNOWN"};

		switch (type) {
			case E_ERROR:
			case E_CORE_ERROR:
			case E_COMPILE_ERROR:
			case E_USER_ERROR:
			case E_RECOVERABLE_ERROR:
				return FATAL;
			case E_WARNING:
			case E_CORE_WARNING:
			case E_COMPILE_WARNING:
			case E_USER_WARNING:
				return WARNING;
			case E_PARSE:
				return ERROR;
			case E_NOTICE:
			case E_USER_NOTICE:
				return NOTICE;
			case E_STRICT:
				return STRICT;
			case E_DEPRECATED:
			case E_USER_DEPRECATED:
				return DEPRECATED;
			default:
				return UNKNOWN;
		}
	}
	php::string uppercase(const char* str, size_t len) {
		php::string s(str, len);
		php_strtoupper(s.data(), len);
		return std::move(s);
	}
	php::string lowercase(const char* str, size_t len) {
		php::string s(str, len);
		php_strtolower(s.data(), len);
		return std::move(s);
	}
}