#include "../phpext.h"

namespace php {
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

	std::uint32_t crc32(unsigned char* src, uint32_t src_len) {
		register uint32_t crc32_val = 0;
		for(; --src_len; ++src) {
			crc32_val = ((crc32_val >> 8) & 0x00FFFFFF) ^ crc32tab[(crc32_val ^ (*src)) & 0xFF ];
		}
		return crc32_val;
	}
}
