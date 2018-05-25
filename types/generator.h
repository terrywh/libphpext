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
		
		
		void rewind();
		bool valid();
		php::value current();
		php::value key();
		void next();
		php::value send(const php::value& v);
		php::value throw_exception(const std::string& ex, int code = 0);
		php::value throw_exception(php::value ex);
		php::value get_return();
	};
}
