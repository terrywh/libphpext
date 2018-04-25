#pragma once

namespace php {
	class value;
	class callable : public value {
	public:
		callable(): value() {}
		explicit callable(std::nullptr_t np): value(nullptr) {}
		explicit callable(const std::string& name): value(name) {}
		explicit callable(const php::string& name);
		callable(const callable& cb): value(cb) {}
		callable(callable&& cb): value(std::move(cb)) {}
		static value __invoke(php::value& cb, int argc, zval* argv, bool silent);
		static value __invoke(php::value& cb, std::vector<value> argv, bool silent);
		inline value invoke() {
			return __invoke(*this, 0, nullptr, false);
		}
		inline value sinvoke() {
			return __invoke(*this, 0, nullptr, true);
		}
		inline value invoke(std::initializer_list<value> argv) {
			return __invoke(*this, argv, false);
		}
		inline value sinvoke(std::initializer_list<value> argv) {
			return __invoke(*this, argv, true);
		}
		inline value operator()() {
			return __invoke(*this, 0, nullptr, false);
		}
		inline value operator()(std::initializer_list<value> argv) {
			return __invoke(*this, argv, false);
		}
		inline std::uint32_t addref() {
			return Z_ADDREF(value_);
		}
		inline std::uint32_t delref() {
			return Z_DELREF(value_);
		}
		using value::operator =;
		using value::operator ==;
		inline callable& operator=(const callable& v) {
			value::operator=(v);
			return *this;
		}
		inline callable& operator=(callable&& v) {
			value::operator=(std::move(v));
			return *this;
		}
	};
}
