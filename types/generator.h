#pragma once

namespace php {
	class value;
	class generator : public value {
	public:
		generator(): value() {};
		generator(const generator& obj): value(obj) {}
		generator(generator&& obj): value(std::move(obj)) {}

		php::value current();
		void next();
		void send(const php::value& v);
		void throw_exception(const php::value& e);
		void throw_exception(const std::string& msg, int code = 0);
		bool valid();
		php::value get_return();

		using value::operator =;
		using value::operator ==;
	};
}
