#pragma once
namespace php {
	template <int MAX_LENGTH>
	static std::string format(const char* format, ...) {
		char buffer[MAX_LENGTH];
		int  length;
		va_list argv;
		va_start(argv, format);
		length = vsprintf(buffer, format, argv);
		va_end(argv);
		return std::string(buffer, length);
	}
}