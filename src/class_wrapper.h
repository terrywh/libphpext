#pragma once

namespace php {
	struct class_wrapper {
		std::unique_ptr<class_base> cpp;
		zend_object obj;
	};
	extern class_base* native(zend_object* obj);
}