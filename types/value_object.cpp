#include "../phpext.h"

namespace php {
	value::value(class_base* base)
	: val_(&value_)
	, ref_(false) {
		if(Z_TYPE(base->val_) != IS_UNDEF) {
			ZVAL_COPY(val_, &base->val_);
		}else{
			throw exception("object is not yet created");
		}
	}
	value value::prop(const std::string& name) {
		return prop(name.c_str(), name.length());
	}
	value value::prop(const char* name, std::size_t len) {
		if( !is_object() ) throw exception("type error: object expected");
		zval nv, zv, *pv;
		ZVAL_STRINGL(&nv, name, len);
		pv = Z_OBJ_P(val_)->handlers->read_property(val_, &nv, BP_VAR_R, nullptr, &zv);
		_zval_dtor(&nv);
		return value(pv, /*ref*/true);
	}
	value value::call(const std::string& name) {
		return call_(name.c_str(), name.length(), 0, nullptr);
	}
	value value::call_(const char* name, std::size_t len, int argc, zval* argv) {
		if( !is_object() ) throw exception("type error: object expected");
		zval vname;
		value rv(nullptr);
		ZVAL_STRINGL(&vname, name, len);
		if(call_user_function(CG(function_table), val_, &vname, rv.data(), argc, argv) == FAILURE) {
			throw exception("faild to invoke method", exception::INVOKE_METHOD_FAILED);
		}
		_zval_dtor(&vname);
		return std::move(rv);
	}
}
