#include "phpext.h"

namespace php {
	CLASS::CLASS(const zend_class_entry* ce)
	: ce_(const_cast<zend_class_entry*>(ce)) {
		assert(ce_ != nullptr && "未知类型");
	}
	CLASS::CLASS(const php::string& name) {
		zend_class_entry* ce = zend_lookup_class(name);
		if(ce == nullptr) {
			throw exception(zend_ce_type_error, "class '" + static_cast<std::string>(name) + "' NOT found");
		}
		ce_ = ce;
	}
	std::string CLASS::name() const {
		return std::string(ce_->name->val, ce_->name->len);
	}
	CLASS::operator zend_class_entry*() const {
		return ce_;
	}
}