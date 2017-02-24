#include "../../core.h"

namespace phpext { namespace internal {
	arg_info::arg_info(string&& name, bool reqired, bool by_ref)
	:arg_info(std::move(name), 0, reqired, by_ref) {

	}
	arg_info::arg_info(string&& name, zend_uchar type, bool reqired, bool by_ref) {
		name.addref();
		class_name = nullptr;
		type_hint = type;
		pass_by_reference = by_ref;
		allow_null = reqired;
		is_variadic = false;
	}
	arg_info::arg_info(string&& name, string&& class_name, bool reqired, bool by_ref) {
		name.addref();
		class_name.addref();
		type_hint = 0;
		pass_by_reference = by_ref;
		allow_null = reqired;
		is_variadic = false;
	}
}}
