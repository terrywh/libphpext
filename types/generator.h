#pragma once

namespace php {
	class value;
	class generator : public value {
	public:
		using value::operator =;
		using value::operator ==;
		using value::operator zval*;
		using value::operator zend_object*;
		
		generator():value() {}
		generator(const php::value& v): value(v) {}
		generator(php::value&& v): value(std::move(v)) {}

		php::value current();
		void next();
		php::value send(const php::value& v);
		php::value throw_exception(const php::value& ex);
		inline php::value throw_exception(const std::string& msg, int code = 0) {
			return throw_exception(php::object::create_exception(msg, code));
		}
		bool valid();
		php::value get_return();
	};
}
