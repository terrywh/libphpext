#pragma once

namespace php {
	class class_base;
	struct class_wrapper {
		class_base* cpp;
		zend_object obj;
	};
	class_base* native(zend_object* obj);
}