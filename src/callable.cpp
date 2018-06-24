#include "vendor.h"
#include "callable.h"

#include "exception.h"
#include "parameters.h"
#include "array_member.h"
#include "property.h"

namespace php {
	value callable::__call(zval* cb) {
		value rv;
		int r = call_user_function(EG(function_table), nullptr, cb, rv, 0, nullptr);
		assert(r == SUCCESS && "调用失败");
		exception::rethrow();
		return std::move(rv);
	}
	value callable::__call(zval* cb, std::vector<value> argv) {
		zval params[argv.size()];
		for(int i=0;i<argv.size();++i) {
			ZVAL_COPY_VALUE(&params[i], static_cast<zval*>(argv[i]));
		}
	
		value rv;
		int r = call_user_function(EG(function_table), nullptr, cb, rv, argv.size(), params);
		assert(r == SUCCESS && "调用失败");
		exception::rethrow();
		return std::move(rv);
	}
	callable::callable() {
		
	}
	callable::callable(std::nullptr_t n)
	: value(n) {

	}
	callable::callable(zval* v, bool ref)
	: value(v, ref) {
		
	}
	callable::callable(const char* fname)
	: callable(std::string(fname)) {
		
	}
	callable::callable(const std::string& fname) {
		ZVAL_STRINGL(&val_, fname.c_str(), fname.length());
	}
	callable::callable(zend_object* cb) {
		ZVAL_OBJ(&val_, cb);
		assert(typeof(TYPE::CALLABLE));
		addref();
	}
	callable::callable(std::function<php::value (php::parameters& params)> v)
	: value(v) {

	}
	callable::callable(const value& v)
	: value(v/* , TYPE::CALLABLE */) {

	}
	callable::callable(value&& v)
	: value(std::move(v)/* , TYPE::CALLABLE */) {

	}
	callable::callable(const parameter& v)
	: value(v.raw()) {

	}
	callable::callable(const array_member& v)
	: value(v.raw()) {

	}
	callable::callable(const property& v)
	: value(v.raw()) {

	}
	// ---------------------------------------------------------------------
	value callable::call() const {
		return __call(ptr_);
	}
	value callable::call(std::vector<value> argv) const {
		return __call(ptr_, argv);
	}
	value callable::operator()() const {
		return __call(ptr_);
	}
	value callable::operator()(std::vector<value> argv) const {
		return __call(ptr_, argv);
	}
	// ---------------------------------------------------------------------
	callable& callable::operator =(const parameter& v) {
		value::operator =(v.operator value());
		return *this;
	}
	callable& callable::operator =(const array_member& v) {
		value::operator =(v.operator value());
		return *this;
	}
	callable& callable::operator =(const property& v) {
		value::operator =(v.operator value());
		return *this;
	}
}
