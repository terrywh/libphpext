#pragma once

namespace php {
	class value;
	class callable : public value {
	private:
		static value __invoke(zval* cb, int argc, zval* argv, bool silent);
	public:
		callable(): value() {}
		explicit callable(std::nullptr_t np): value(nullptr) {}
		explicit callable(const std::string& name): value(name) {}
		explicit callable(const php::string& name);
		callable(const callable& cb): value(cb) {}
		callable(callable&& cb): value(std::move(cb)) {}
		inline value invoke() {
			return __invoke(&value_, 0, nullptr, false);
		}
		inline value sinvoke() {
			return __invoke(&value_, 0, nullptr, true);
		}
		inline value invoke(std::vector<value> argv) {
			return __invoke(&value_, argv.size(), (zval*)argv.data(), false);
		}
		inline value sinvoke(std::vector<value> argv) {
			return __invoke(&value_, argv.size(), (zval*)argv.data(), true);
		}
		inline value operator()() {
			return __invoke(&value_, 0, nullptr, false);
		}
		inline value operator()(std::vector<value> argv) {
			return __invoke(&value_, argv.size(), (zval*)argv.data(), false);
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
