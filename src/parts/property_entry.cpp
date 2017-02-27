#include "../vendor.h"
#include "../types/value.h"
#include "property_entry.h"

namespace php {
	property_entry::property_entry(const std::string& name, value&& val, int access)
	: value_(std::move(val))
	, access_(access) {
		name_ = zend_string_init(name.c_str(), name.length(), true);
	}
	property_entry::property_entry(property_entry&& entry)
	: name_(entry.name_)
	, value_(std::move(entry.value_))
	, access_(entry.access_) {
	}
	void property_entry::declare(zend_class_entry* entry) {
		zval property;
		// property 的数据会被 ZVAL_COPY_VALUE 转存到 property_table 中，需要额外的引用
		ZVAL_COPY(&property, value_.data());
		int r = zend_declare_property_ex(entry,
			name_, // name 单独申请 persistent 文本
			&property, access_, /* doc_comment */nullptr);
	}
}
