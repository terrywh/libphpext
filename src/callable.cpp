#include "phpext.h"

namespace php {
	value callable::__call(const zval* cb) {
		value rv;
		int r = call_user_function(EG(function_table), nullptr, const_cast<zval*>(cb), rv, 0, nullptr);
		assert(r == SUCCESS && "调用失败");
		exception::rethrow();
		return std::move(rv);
	}
	value callable::__call(const zval* cb, std::vector<value> argv) {
		zval params[argv.size()];
		for(int i=0;i<argv.size();++i) {
			ZVAL_COPY_VALUE(&params[i], static_cast<zval*>(argv[i]));
		}
	
		value rv;
		int r = call_user_function(EG(function_table), nullptr, const_cast<zval*>(cb), rv, argv.size(), params);
		assert(r == SUCCESS && "调用失败");
		exception::rethrow();
		return std::move(rv);
	}
	// 注意: 此种构造形式无类型检查
	callable::callable(const zval* v, bool copy)
	: value(v, copy) {
		
	}
	callable::callable(const char* fname)
	: callable(std::string(fname)) {
		
	}
	callable::callable(const std::string& fname) {
		ZVAL_STRINGL(&value_ins, fname.c_str(), fname.length());
	}
	callable::callable(zend_object* cb) {
		ZVAL_OBJ(&value_ins, cb);
		if(!typeof(TYPE::CALLABLE)) {
			ZVAL_UNDEF(&value_ins);
			throw exception(zend_ce_type_error, "type of 'callable' is expected, '" + typeof().name() + "' given");
		}
		addref();
	}
	callable::callable(std::function<php::value (php::parameters& params)> v)
	: value(v) {

	}
	callable::callable(const value& v)
	: value(v, TYPE::CALLABLE) {

	}
	callable::callable(value&& v)
	: value(std::move(v), TYPE::CALLABLE) {

	}
	// ---------------------------------------------------------------------
	value callable::call() const {
		return __call(&value_ins);
	}
	value callable::call(std::vector<value> argv) const {
		return __call(&value_ins, argv);
	}
	value callable::operator()() const {
		return __call(&value_ins);
	}
	value callable::operator()(std::vector<value> argv) const {
		return __call(&value_ins, argv);
	}
	// -------------------------------------------------------------------
	callable& callable::operator =(const value& v) {
		assign(v, TYPE::CALLABLE);
		return *this;
	}
	callable& callable::operator =(value&& v) {
		assign(std::move(v), TYPE::CALLABLE);
		return *this;
	}
	callable& callable::operator =(const zval* v) {
		value::operator =(v);
		return *this;
	}
}
