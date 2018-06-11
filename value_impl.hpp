#pragma once

namespace php {
	value::value(const value& v, const TYPE& t) {
		if(!v.typeof(t)) {
			throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		}
		ZVAL_COPY(&value_ins, v);
	}
	value::value(value&& v, const TYPE& t) {
		if(!v.typeof(t)) {
			throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		}
		ZVAL_COPY_VALUE(&value_ins, &v.value_ins);
		ZVAL_UNDEF(&v.value_ins);
	}
	value::value(const value& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		ZVAL_COPY(&value_ins, v);
	}
	value::value(value&& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		ZVAL_COPY_VALUE(&value_ins, &v.value_ins);
		ZVAL_UNDEF(&v.value_ins);
	}
	void value::assign(const value& v, const TYPE& t) {
		if(!v.typeof(t)) throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		operator =(static_cast<const value&>(v));
	}
	void value::assign(value&& v, const TYPE& t) {
		if(!v.typeof(t)) throw exception(zend_ce_type_error, "type of '" + t.name() + "' is expected, '" + v.typeof().name() + "' given");
		value::operator =(std::move(v));
	}
	void value::assign(const value& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		operator =(static_cast<const value&>(v));
	}
	void value::assign(value&& v, const CLASS& c) {
		if(!v.instanceof(c)) throw exception(zend_ce_type_error, "instance of '" + c.name() + "' is expected, '" + v.classof().name() + "' given");
		value::operator =(std::move(v));
	}
}