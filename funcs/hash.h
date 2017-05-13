#pragma once
namespace php {
	php::string md5(char* enc_str, uint32_t enc_len);
	php::string sha1(const unsigned char* enc_str, size_t enc_len);
	std::uint32_t crc32(unsigned char* src, uint32_t src_len);
}
