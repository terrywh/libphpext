#include "vendor.h"
#include "parameters.h"

namespace php {
	parameter& parameter::operator =(const value& v) {
		if(Z_ISREF_P(arg_)) {
			zval_ptr_dtor(Z_REFVAL_P(arg_));
			// 引用参数改其内容
			ZVAL_COPY(Z_REFVAL_P(arg_), static_cast<zval*>(v));
		}else{
			zval_ptr_dtor(arg_);
			ZVAL_COPY(arg_, static_cast<zval*>(v));
		}
		return *this;
	}
	value parameter::ptr() const {
		if(Z_ISREF_P(arg_)) return value(Z_REFVAL_P(arg_), true);
		else return value(arg_, true);
	}
	zval* parameter::raw() const {
		if(Z_ISREF_P(arg_)) return Z_REFVAL_P(arg_);
		else return arg_;
	}
	parameter::parameter(zval* arg)
	: arg_(arg) {

	}
	parameters::parameters(zend_execute_data* execute_data) {
		argc_ = ZEND_CALL_NUM_ARGS(execute_data);
		if(argc_ > 0) {
			argv_ = ZEND_CALL_ARG(execute_data, 1);
		}
	}
	parameters::parameters(int argc, zval* argv)
	: argc_(argc)
	, argv_(argv) {

	}
	parameter parameters::operator[](std::uint8_t index) const {
		if(index >= argc_) {
			// TODO 补充当前函数或方法名称信息?
			throw exception(zend_ce_type_error, "missing argument " + std::to_string(index+1));
		}
		if(Z_ISREF(argv_[index])) {
			return parameter(Z_REFVAL(argv_[index]));
		}else{
			return parameter(&argv_[index]);
		}
	}
	value parameters::get(std::uint8_t index, bool ptr) const {
		if(index >= argc_) {
			// TODO 补充当前函数或方法名称信息?
			throw exception(zend_ce_type_error, "missing argument " + std::to_string(index+1));
		}

		if(Z_ISREF(argv_[index])) {
			return value(Z_REFVAL(argv_[index]), ptr);
		}else{
			return value(&argv_[index], ptr);
		}
	}
	void parameters::set(std::uint8_t index, const value& v) {
		if(Z_ISREF(argv_[index])) {
			zval_ptr_dtor(Z_REFVAL(argv_[index]));
			// 引用参数改其内容
			ZVAL_COPY(Z_REFVAL(argv_[index]), static_cast<zval*>(v));
		}else{
			zval_ptr_dtor(&argv_[index]);
			ZVAL_COPY(&argv_[index], static_cast<zval*>(v));
		}
	}
	std::uint8_t parameters::length() const {
		return argc_;
	}
	std::uint8_t parameters::size() const {
		return argc_;
	}
	parameters::operator std::vector<value>() const {
		std::vector<value> argv(argc_);
		for(int i=0;i<argc_;++i) {
			ZVAL_COPY(argv[i], argv_ + i);
		}
		return std::move(argv);
	}
}
