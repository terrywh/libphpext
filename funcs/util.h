#pragma once
namespace php {
	// template <int MAX_LENGTH>
	// static std::string format(const char* format, ...) {
	// 	std::string str(MAX_LENGTH);
	// 	//char buffer[MAX_LENGTH];
	// 	// char buffer[1024];
	// 	int length;
	// 	va_list argv;
	// 	va_start(argv, format);
	// 	length = vsprintf(const_cast<char*>(str.c_str()), format, argv);
	// 	va_end(argv);
	// 	str.resize(length);
	// 	return std::move(str);
	// }
	void fail(const std::string& msg);
    void fail(const char *format, ...);
	void warn(const std::string& msg);
    void warn(const char *format, ...);
	void info(const std::string& msg);
    void info(const char *format, ...);

    const char* ini_get(char* name, uint name_len);
    std::string md5(char* enc_str, uint32_t enc_len);
    std::string sha1(const unsigned char* enc_str, size_t enc_len);
    uint32_t crc32(unsigned char* src, uint32_t src_len);

    std::shared_ptr<zend_string> base64_encode(const unsigned char* enc_str, size_t enc_len);
    std::shared_ptr<zend_string> base64_decode(const unsigned char* dec_str, size_t dec_len);
    std::shared_ptr<zend_string> url_encode(const char* enc_str, size_t enc_len);
    std::shared_ptr<zend_string> url_decode(char* dec_str, size_t& dec_len);
    std::shared_ptr<zend_string> hex2bin(unsigned char* src, size_t src_len);
    std::shared_ptr<zend_string> bin2hex(unsigned char* src, size_t src_len);
    std::shared_ptr<zend_string> json_encode(zval* val);
    php::value json_decode(char* src, int32_t src_len);
    std::shared_ptr<php_url> parse_url(const char* url, size_t url_len);
    php::value parse_ini_file(char* filename);
}

