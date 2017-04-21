#include "../vendor.h"

namespace php {
	void fail(const std::string& msg) {
		zend_error(E_USER_ERROR, msg.c_str());
	}
	void warn(const std::string& msg) {
		zend_error(E_USER_WARNING, msg.c_str());
	}
	void info(const std::string& msg) {
		zend_error(E_USER_NOTICE, msg.c_str());
	}

    std::shared_ptr<php_url> parse_url(const char* url, size_t url_len) {
        return std::shared_ptr<php_url>(php_url_parse_ex(url, url_len), php_url_free);
    }

    const char* ini_get(char* name, uint name_len) {
        return zend_ini_string(name, name_len, 0);
    }

    std::string md5(char* enc_str, uint32_t enc_len) {
        char md5str[33] = {0};
        PHP_MD5_CTX context;
        unsigned char digest[16];

        PHP_MD5Init(&context);
        PHP_MD5Update(&context, enc_str, enc_len);
        PHP_MD5Final(digest, &context);
        make_digest_ex(md5str, digest, 16);
        return std::string(md5str);
    }

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
        zend_string* url = zend_string_init(dec_str, dec_len, false);
        url->len = php_url_decode(url->val, url->len);
        return std::shared_ptr<zend_string>(url, zend_string_release);
    }

    std::string sha1(const unsigned char* enc_str, size_t enc_len) {
        char sha1str[41] = {0};
        PHP_SHA1_CTX context;
        unsigned char digest[20];

        PHP_SHA1Init(&context);
        PHP_SHA1Update(&context, enc_str, enc_len);
        PHP_SHA1Final(digest, &context);
        make_digest_ex(sha1str, digest, 20);

        return std::string(sha1str);
    }
}

