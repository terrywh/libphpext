#pragma once

namespace php {
	class value;
	class callable : public value {
	private:
		static value __invoke(zval* cb, int argc, zval* argv);
	public:
		using value::value;
		using value::operator =;
		using value::operator ==;
		using value::operator zval*;
		
		callable(): value() {}
		callable(const php::value& v): value(v) {}
		callable(php::value&& v): value(std::move(v)) {}
		
		inline value invoke() {
			return __invoke(&value_, 0, nullptr);
		}
		inline value invoke(std::vector<value> argv) {
			return __invoke(&value_, argv.size(), (zval*)argv.data());
		}
		inline value operator()() {
			return __invoke(&value_, 0, nullptr);
		}
		inline value operator()(std::vector<value> argv) {
			return __invoke(&value_, argv.size(), (zval*)argv.data());
		}
	};
}
