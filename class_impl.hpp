#pragma once

namespace php {
	CLASS::CLASS(const php::string& name) {
		zend_class_entry* ce = zend_lookup_class(name);
		if(ce == nullptr) {
			throw exception(zend_ce_type_error, "class '" + static_cast<std::string>(name) + "' NOT found");
		}
		ce_ = ce;
	}
}