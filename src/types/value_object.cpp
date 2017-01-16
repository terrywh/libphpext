#include "value.h"
#include "exception.h"

namespace php {
	value value::prop(const char* name) {
		prop(name, std::strlen(name));
	}
	value value::prop(const std::string& name) {
		prop(name.c_str(), name.length());
	}
	value value::prop(const char* name, std::size_t len) {
		zval nv, zv, *pv;
		ZVAL_STRINGL(&nv, name, len);
		pv = Z_OBJ_P(val_)->handlers->read_property(val_, &nv, BP_VAR_R, nullptr, &zv);
		_zval_dtor(&nv);
		value rv(pv, /*ref*/true);
		rv.delref(); // value will addref() to pv and we only need on ref
		return std::move(rv);
	}
	value& value::prop(const char* name, const value& val) {
		return prop(name, std::strlen(name), val);
	}
	value& value::prop(const std::string& name, const value& val) {
		return prop(name.c_str(), name.length(), val);
	}
	value& value::prop(const char* name, std::size_t len, const value& val) {
		zval nv;
		ZVAL_STRINGL(&nv, name, len);
		Z_OBJ_P(val_)->handlers->write_property(val_, &nv, val.data(), nullptr);
		// write_property will addref()
		_zval_dtor(&nv);
		return *this;
	}
	value value::call(const char* name) {
		call_(name, std::strlen(name), 0, nullptr);
	}
	value value::call(const std::string& name) {
		call_(name.c_str(), name.length(), 0, nullptr);
	}
	value value::call(const char* name, std::size_t len) {
		call_(name, len, 0, nullptr);
	}
	value value::call_(const char* name, std::size_t len, int argc, zval* argv) {
		zval nv;
		value rv(nullptr);
		ZVAL_STRINGL(&nv, name, len);
		if(call_user_function(CG(function_table), val_, &nv, rv.data(), argc, argv) == SUCCESS) {
			return std::move(rv);
		}
		throw exception("call to " + std::string(name, len) + " failed", 0);
		// return nullptr;
	}
}
