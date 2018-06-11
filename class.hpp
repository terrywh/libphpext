#pragma once

namespace php {
	class string;
	class CLASS {
	public:
		CLASS(const zend_class_entry* ce)
		: ce_(const_cast<zend_class_entry*>(ce)) {
			assert(ce_ != nullptr && "未知类型");
		}
		CLASS(const php::string& name); // -> class_impl.hpp
		std::string name() const {
			return std::string(ce_->name->val, ce_->name->len);
		}
		operator zend_class_entry*() const {
			return ce_;
		}
	private:
		zend_class_entry* ce_;
	};
}