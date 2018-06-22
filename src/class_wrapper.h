#pragma once

namespace php {
	struct class_wrapper {
		class_base* cpp;
		zend_object obj;
	};
	class_base* native(zend_object* obj);
}