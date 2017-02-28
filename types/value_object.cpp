#include "value.h"
#include "exception.h"

namespace php {
	value value::prop(const char* name) {
		prop(name, std::strlen(name));
	}
	value value::prop(const char* name, std::size_t len) {
		assert( is_object() );
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
	value& value::prop(const char* name, std::size_t len, const value& val) {
		assert( is_object() );
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
	value value::call(const char* name, std::size_t len) {
		call_(name, len, 0, nullptr);
	}
	value value::call_(const char* name, std::size_t len, int argc, zval* argv) {
		assert( is_object() );
		zval nv;
		value rv(nullptr);
		ZVAL_STRINGL(&nv, name, len);
		if(call_user_function(CG(function_table), val_, &nv, rv.data(), argc, argv) == FAILURE) {
			throw exception("faild to invoke method", exception::INVOKE_METHOD_FAILED);
		}
		return std::move(rv);
	}
}
