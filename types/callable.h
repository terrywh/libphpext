#pragma once

namespace php {
	class value;
	class callable : public value {
	public:
		callable(): value() {}
		callable(std::nullptr_t np): value(nullptr) {}
		callable(const callable& cb): value(cb) {}
		callable(callable&& cb): value(std::move(cb)) {}
		static value __invoke(zval* callable, int argc, zval argv[], bool silent);
		inline value invoke() {
			return __invoke(&value_, 0, nullptr, false);
		}
		inline value sinvoke() {
			return __invoke(&value_, 0, nullptr, true);
		}
		template <typename ...Args>
		inline value invoke(const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __invoke(&value_, sizeof...(Args), (zval*)params, false);
		}
		template <typename ...Args>
		inline value sinvoke(const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __invoke(&value_, sizeof...(Args), (zval*)params, true);
		}
		inline value operator()() {
			return __invoke(&value_, 0, nullptr, false);
		}
		template <typename ...Args>
		inline value operator()(const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __invoke(&value_, sizeof...(Args), (zval*)params, false);
		}
		inline std::uint32_t addref() {
			return Z_ADDREF(value_);
		}
		inline std::uint32_t delref() {
			return Z_DELREF(value_);
		}
		using value::operator =;
		using value::operator ==;
	};
}
