#include "../phpext.h"

namespace php {
	value class_exception::__construct(parameters& params) {
		prop("message") = params[0];
		if(params.length()>1) {
			prop("code") = static_cast<int>(params[1]);
		}
	}
	value class_exception::getMessage(parameters& params) {
		return prop("message");
	}
	value class_exception::getCode(parameters& params) {
		return prop("code");
	}
	value class_exception::getFile(parameters& params) {
		return php::value("php::extension");
	}
	value class_exception::getLine(parameters& params) {
		return 0;
	}
	value class_exception::getTrace(parameters& params) {
		return static_cast<zend_array*>(php::array());
	}
	value class_exception::getTraceAsString(parameters& params) {
		return php::value("");
	}
	value class_exception::getPrevious(parameters& params) {
		return php::value(nullptr);
	}
	value class_exception::__toString(parameters& params) {
		std::string message = prop("message");
		message.append(" (code=");
		message.append(std::to_string(static_cast<int>(prop("code"))));
		message.append(")");
		return php::value(message);
	}
	value class_closure::__invoke(parameters& params) {
		return fn_(params);
	}
	void _register_builtin_class(extension_entry& extension) {
		zend_ce_throwable->interface_gets_implemented = nullptr;
		php::class_entry<php::class_closure> class_closure_entry("__closure");
		class_closure_entry.add<&php::class_closure::__invoke>("__invoke");
		extension.add(std::move(class_closure_entry));
		php::class_entry<php::class_exception> class_exception_entry("__exception", "exception");
		class_exception_entry.implements("Throwable");
		// class_exception_entry.add<&php::class_exception::__construct>("__construct");
		// class_exception_entry.add<&php::class_exception::getMessage>("getMessage");
		// class_exception_entry.add<&php::class_exception::getCode>("getCode");
		// class_exception_entry.add<&php::class_exception::getFile>("getFile");
		// class_exception_entry.add<&php::class_exception::getLine>("getLine");
		// class_exception_entry.add<&php::class_exception::getTrace>("getTrace");
		// class_exception_entry.add<&php::class_exception::getTraceAsString>("getTraceAsString");
		// class_exception_entry.add<&php::class_exception::getPrevious>("getPrevious");
		class_exception_entry.add<&php::class_exception::__toString>("__tostring");
		class_exception_entry.add(php::property_entry("message", std::string("")));
		class_exception_entry.add(php::property_entry("code", 0));
		extension.add(std::move(class_exception_entry));
	}

}
