#pragma once

namespace php {
	class callable: public value {
	private:
		static value __invoke(zval* cb) {
			value rv;
			int r = call_user_function(EG(function_table), nullptr, cb, rv, 0, nullptr);
			assert(r == SUCCESS && "调用失败");
			exception::maybe_rethrow();
			return std::move(rv);
		}
		static value __invoke(zval* cb, std::vector<value> argv) {
			zval params[argv.size()];
			for(int i=0;i<argv.size();++i) {
				ZVAL_COPY_VALUE(&params[i], static_cast<zval*>(argv[i]));
			}
		
			value rv;
			int r = call_user_function(EG(function_table), nullptr, cb, rv, argv.size(), params);
			assert(r == SUCCESS && "调用失败");
			exception::maybe_rethrow();
			return std::move(rv);
		}
	public:
		callable(const std::string& fname) {
			ZVAL_STRINGL(&value_ins, fname.c_str(), fname.length());
		}
		callable(zend_object* cb) {
			ZVAL_OBJ(&value_ins, cb);
			if(!typeof(TYPE::CALLABLE)) {
				ZVAL_UNDEF(&value_ins);
				throw exception(zend_ce_type_error, "type of 'callable' is expected, '" + typeof().name() + "' given");
			}
			addref();
		}
		callable(const value& v)
		: value(v, TYPE::CALLABLE) {
		}
		callable(value&& v)
		: value(std::move(v), TYPE::CALLABLE) {

		}
		// ---------------------------------------------------------------------
		inline value invoke() {
			return __invoke(&value_ins);
		}
		inline value invoke(std::vector<value> argv) {
			return __invoke(&value_ins, argv);
		}
		inline value operator()() {
			return __invoke(&value_ins);
		}
		inline value operator()(std::vector<value> argv) {
			return __invoke(&value_ins, argv);
		}
		// -------------------------------------------------------------------
		callable& operator =(const value& v) {
			assign(v, TYPE::CALLABLE);
			return *this;
		}
		callable& operator =(value&& v) {
			assign(std::move(v), TYPE::CALLABLE);
			return *this;
		}
	};
}
