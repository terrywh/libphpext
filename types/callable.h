#pragma once

namespace php {
	class value;

	class callable {
	private:
		zval* cb_;
		static value _invoke(zval* callable, int argc, zval argv[]);
		callable(zval* callable);
	public:
		~callable();
		callable(const callable& cb);
		callable(callable&& cb);

		inline value invoke() {
			return _invoke(cb_, 0, nullptr);
		}
		template <typename ...Args>
		inline value invoke(const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return _invoke(cb_, sizeof...(Args), (zval*)params);
		}
		inline value operator()() {
			return _invoke(cb_, 0, nullptr);
		}
		template <typename ...Args>
		inline value operator()(const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return _invoke(cb_, sizeof...(Args), (zval*)params);
		}
		friend class value;
	};
}
