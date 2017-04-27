#include "../phpext.h"

namespace php {
	std::shared_ptr<zend_string> base64_encode(const unsigned char* enc_str, size_t enc_len) {
		return std::shared_ptr<zend_string>(php_base64_encode(enc_str, enc_len), zend_string_release);
	}
	std::shared_ptr<zend_string> base64_decode(const unsigned char* dec_str, size_t dec_len) {
		//php_base64_decode_ex(dec_str, dec_len, 0);
		return std::shared_ptr<zend_string>(php_base64_decode_ex(dec_str, dec_len, 0), zend_string_release);

	}

	std::shared_ptr<zend_string> url_encode(const char* enc_str, size_t enc_len) {
		return std::shared_ptr<zend_string>(php_url_encode(enc_str, enc_len), zend_string_release);
	}

	std::shared_ptr<zend_string> url_decode(char* dec_str, size_t& dec_len) {
		// void url_decode(char* dec_str, size_t& dec_len) {
		// dec_len = php_url_decode(dec_str, dec_len);
		zend_string* url = zend_string_init(dec_str, dec_len, false);
		url->len = php_url_decode(url->val, url->len);
		return std::shared_ptr<zend_string>(url, zend_string_release);
	}

	static char hexconvtab[] = "0123456789abcdef";
	static zend_string *php_bin2hex(const unsigned char *old, const size_t oldlen)
	{
		zend_string *result;
		size_t i, j;

		result = zend_string_safe_alloc(oldlen, 2 * sizeof(char), 0, 0);

		for (i = j = 0; i < oldlen; i++) {
			ZSTR_VAL(result)[j++] = hexconvtab[old[i] >> 4];
			ZSTR_VAL(result)[j++] = hexconvtab[old[i] & 15];
		}
		ZSTR_VAL(result)[j] = '\0';

		return result;
	}

	static zend_string *php_hex2bin(const unsigned char *old, const size_t oldlen)
	{
		size_t target_length = oldlen >> 1;
		zend_string *str = zend_string_alloc(target_length, 0);
		unsigned char *ret = (unsigned char *)ZSTR_VAL(str);
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
				zend_string_free(str);
				return NULL;
			}
			c = old[j++];
			l = c & ~0x20;
			is_letter = ((unsigned int) ((l - 'A') ^ (l - 'F' - 1))) >> (8 * sizeof(unsigned int) - 1);
			if (EXPECTED((((c ^ '0') - 10) >> (8 * sizeof(unsigned int) - 1)) | is_letter)) {
				d |= l - 0x10 - 0x27 * is_letter;
			} else {
				zend_string_free(str);
				return NULL;
			}
			ret[i] = d;
		}
		ret[i] = '\0';

		return str;
	}

	std::shared_ptr<zend_string> hex2bin(unsigned char* src, size_t src_len) {
		if(src_len % 2 != 0) {
			return nullptr;
		}
		return std::shared_ptr<zend_string>(php_hex2bin(src, src_len), zend_string_release);
	}

	std::shared_ptr<zend_string> bin2hex(unsigned char* src, size_t src_len) {
		if(src_len % 2 != 0) {
			return nullptr;
		}
		return std::shared_ptr<zend_string>(php_bin2hex(src, src_len), zend_string_release);
	}

	std::shared_ptr<zend_string> json_encode(zval* val) {
		smart_str buf = {0};

		JSON_G(error_code) = PHP_JSON_ERROR_NONE;
		JSON_G(encode_max_depth) = PHP_JSON_PARSER_DEFAULT_DEPTH;
		php_json_encode(&buf, val, PHP_JSON_UNESCAPED_UNICODE);

		if (JSON_G(error_code) != PHP_JSON_ERROR_NONE && !(PHP_JSON_UNESCAPED_UNICODE & PHP_JSON_PARTIAL_OUTPUT_ON_ERROR)) {
			return nullptr;
		} else {
			ZSTR_VAL(buf.s)[ZSTR_LEN(buf.s)] = '\0';
			return std::shared_ptr<zend_string>(buf.s, zend_string_release);
		}
	}

	php::value json_decode(char* src, std::size_t src_len) {
		int32_t depth = PHP_JSON_PARSER_DEFAULT_DEPTH;
		int32_t options = PHP_JSON_OBJECT_AS_ARRAY;
		php::value v;
		php_json_decode_ex(reinterpret_cast<zval*>(&v), src, src_len, options, depth);
		return std::move(v);
	}
}
