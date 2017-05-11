#pragma once
namespace php {
	class string_parser {
	private:
		enum {
			FIELD_BEFORE,
			FIELD,
			SEPERATOR_1, // =
			VALUE_BEFORE,
			VALUE,
			SEPERATOR_2, // sep
		};
	public:
		string_parser(const char sep2, const char sep1 = '=')
			:sep1_(sep1), sep2_(sep2), status_(FIELD_BEFORE) {}
		bool parse(std::streambuf& buffer, std::size_t n, php::array& data);
		bool parse(const char* buffer, std::size_t n, php::array& data);
		void flush(php::array& data);
	private:
		int parse(const char c, php::array& data);
		const char  sep1_;
		const char  sep2_;
		int         status_;
		std::string field_;
		std::vector<char> value_;
	};
	php::array parse_str(const char sep2, const char* buffer, std::size_t n, const char sep1 = '=');
	php::array parse_str(const char sep2, std::streambuf& buffer, std::size_t n, const char sep1 = '=');
}
