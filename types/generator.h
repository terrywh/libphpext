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
		php::value throw_exception(const php::value& ex);
		php::value async_exception(const std::string& message, int code);
		bool valid();
		php::value get_return();

		using value::operator =;
		using value::operator ==;
		inline generator& operator=(const generator& v) {
			value::operator=(v);
			return *this;
		}
		inline generator& operator=(generator&& v) {
			value::operator=(std::move(v));
			return *this;
		}
	};
}
