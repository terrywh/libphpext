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
	void warn(const std::string& msg);
	void info(const std::string& msg);

}
