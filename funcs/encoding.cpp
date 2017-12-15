#include "../phpext.h"

namespace php {
	php::string base64_encode(const unsigned char* enc_str, std::size_t enc_len) {
		php::string s;
		ZVAL_STR(s, php_base64_encode(enc_str, enc_len));
		return std::move(s);
	}
	php::string base64_decode(const unsigned char* dec_str, std::size_t dec_len) {
		php::string s;
		ZVAL_STR(s, php_base64_decode_ex(dec_str, dec_len, 0));
		return std::move(s);
	}

	php::string url_encode(const char* enc_str, std::size_t enc_len) {
		php::string s;
		ZVAL_STR(s, php_url_encode(enc_str, enc_len));
		return std::move(s);
	}

 	php::string url_decode(const char* dec_str, std::size_t dec_len) {
		php::string s(dec_str, dec_len);
		s.length() = php_url_decode(s.data(), dec_len);
		return std::move(s);
	}

	void url_decode_inplace(char* dec_str, std::size_t& dec_len) {
		dec_len = php_url_decode(dec_str, dec_len);
	}

	static char HEX_CONVERSION_TABLE[] = "0123456789abcdef";
	php::string bin2hex(const unsigned char *old, const size_t len) {
		php::string s(2*len);
		char* data = s.data();
		size_t i, j;

		for (i = j = 0; i < len; i++) {
			data[j++] = HEX_CONVERSION_TABLE[old[i] >> 4];
			data[j++] = HEX_CONVERSION_TABLE[old[i] & 15];
		}
		data[j] = '\0';

		return std::move(s);
	}

	php::string php_hex2bin(const unsigned char *old, const size_t len)	{
		if(len % 2 != 0) {
			throw php::exception("hex2bin failed: illegal hex string");
		}

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
			if (EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
				d = (l - 0x10 - 0x27 * is_letter) << 4;
			} else {
				throw php::exception("hex2bin failed: illegal hex string");
			}
			c = old[j++];
			l = c & ~0x20;
			is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
			if (EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
				d |= l - 0x10 - 0x27 * is_letter;
			} else {
				throw php::exception("hex2bin failed: illegal hex string");
			}
			ret[i] = d;
		}
		ret[i] = '\0';

		return std::move(s);
	}

	php::string json_encode(const php::value& data, int options) {
		php::buffer str;
		php_json_encode(str, (zval*)&data, options);
		return std::move(str);
	}
	php::value json_decode(const char* str, std::size_t size, int depth) {
		php::value rv;
		php_json_decode_ex(
			(zval*)&rv, const_cast<char*>(str), size, PHP_JSON_OBJECT_AS_ARRAY, depth);
		return std::move(rv);
	}
}
