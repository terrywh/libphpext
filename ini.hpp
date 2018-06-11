#pragma once

namespace php {
	class ini {
	public:
		ini(const std::string& key)
		: key_(key) {

		}
		const char* c_str() {
			return data();
		}
		char* data() {
			return zend_ini_string(const_cast<char*>(key_.data()), key_.length(), 0);
		}
		operator std::int64_t() {
			return zend_ini_long(const_cast<char*>(key_.data()), key_.length(), 0);
		}
		operator std::size_t() {
			return zend_ini_long(const_cast<char*>(key_.data()), key_.length(), 0);
		}
		operator std::string() {
			return std::string(data());
		}
		operator double() {
			return zend_ini_double(const_cast<char*>(key_.data()), key_.length(), 0);
		}
	private:
		std::string key_;
	};
}
