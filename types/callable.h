#pragma once

namespace php {
	class value;
	class callable {
	private:
		zval cb_;
		static value __invoke(zval* callable, int argc, zval argv[], bool silent);
		callable(zval* callable);
	public:
		~callable();
		void reset();
		callable() {
			ZVAL_UNDEF(&cb_);
		}
		callable(const callable& cb);
		callable(callable&& cb);

		inline value invoke(bool silent) {
			return __invoke(&cb_, 0, nullptr, silent);
		}
		template <typename ...Args>
		inline value invoke(bool silent, const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __invoke(&cb_, sizeof...(Args), (zval*)params, silent);
		}
		inline value operator()() {
			return __invoke(&cb_, 0, nullptr, false);
		}
		template <typename ...Args>
		inline value operator()(const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __invoke(&cb_, sizeof...(Args), (zval*)params, false);
		}
		inline std::uint32_t addref() {
			return Z_ADDREF(cb_);
		}
		inline std::uint32_t delref() {
			return Z_DELREF(cb_);
		}
		callable& operator=(const callable& cb);
		callable& operator=(callable&& cb);
		friend class value;
	};
}
