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
		/** 计算 k/K m/M g/G 单位得到具体数值 */
		std::int64_t calc();
	private:
		std::string key_;
	};
}
