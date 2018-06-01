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
		php::value throw_exception(const char* msg) {
			return throw_exception( php::exception::create(msg, std::strlen(msg)), 0);
		}
		php::value throw_exception(const char* msg, size_t len, int code = 0) {
			return throw_exception( php::exception::create(msg, (len == -1 ? std::strlen(msg) : len), code) );
		}
		php::value throw_exception(const php::string& ex, int code = 0) {
			return throw_exception( php::exception::create(ex, code) );
		}
		php::value throw_exception(php::value ex);
		php::value get_return();
	};
}
