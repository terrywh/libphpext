#include "vendor.h"
#include "ini.h"

namespace php {
	ini::ini(const std::string& key)
	: key_(key) {

	}
	const char* ini::c_str() {
		return data();
	}
	char* ini::data() {
		return zend_ini_string(const_cast<char*>(key_.data()), key_.length(), 0);
	}
	ini::operator std::int64_t() {
		return zend_ini_long(const_cast<char*>(key_.data()), key_.length(), 0);
	}
	ini::operator std::size_t() {
		return zend_ini_long(const_cast<char*>(key_.data()), key_.length(), 0);
	}
	ini::operator std::string() {
		return std::string(data());
	}
	ini::operator double() {
		return zend_ini_double(const_cast<char*>(key_.data()), key_.length(), 0);
	}
	std::int64_t ini::calc() {
		std::int64_t size = zend_ini_long(const_cast<char *>(key_.data()), key_.length(), 0);
		std::string  val_ = static_cast<std::string>(*this);
		std::size_t npos = val_.find_last_not_of(' ');
		if(npos != val_.npos) {
			switch(val_[npos]) {
			case 't':
			case 'T':
				size *= 1024;
			case 'g':
			case 'G':
				size *= 1024;
			case 'm':
			case 'M':
				size *= 1024;
			case 'k':
			case 'K':
				size *= 1024;
			}
		}
		return size;
	}
}
