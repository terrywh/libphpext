#pragma once

namespace php {
	class ini {
	public:
		ini(const std::string& key);
		const char* c_str();
		char* data();
		operator std::int64_t();
		operator std::size_t();
		operator std::string();
		operator double();
	private:
		std::string key_;
	};
}
