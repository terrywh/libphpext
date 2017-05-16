#pragma once

namespace php {
	class value;
	class callable : public value {
	private:
		static value __invoke(zval* callable, int argc, zval argv[], bool silent);
	public:
		callable(): value() {}
		callable(const callable& cb): value(cb) {}
		callable(callable&& cb): value(std::move(cb)) {}
		inline value invoke(bool silent) {
			return __invoke(&value_, 0, nullptr, silent);
		}
		template <typename ...Args>
		inline value invoke(bool silent, const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __invoke(&value_, sizeof...(Args), (zval*)params, silent);
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
	};
}
