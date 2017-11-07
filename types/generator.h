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
		php::value send(const php::value& v);
		php::value throw_exception(const php::value& e);
		php::value throw_exception(const std::string& msg, int code = 0);
		bool valid();
		php::value get_return();

		using value::operator =;
		using value::operator ==;
	};
}
