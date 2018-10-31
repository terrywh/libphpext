#include "vendor.h"
#include "arguments.h"

namespace php {
	argument::argument(const char* name, bool byref, bool nullable)
	: argument(name, TYPE::UNDEFINED, false, nullable) {}
	argument::argument(const char* name, const char* class_name)
	: argument(name, class_name, false) {}
	// 注意：基础类型的 type_hint 检查 PHP 暂时还未实现
	argument::argument(const char* name, TYPE type_hint, bool byref, bool nullable)
	: arg_ {name, ZEND_TYPE_ENCODE(type_hint, nullable), byref, false} {}
	argument::argument(const char* name, const char* class_name, bool byref)
	: arg_ {name, (zend_type) class_name, byref, false} {

	}
	arguments::arguments(std::initializer_list<argument> args) {
		int required = 0, rset = 0;
		args_.emplace_back( zend_internal_arg_info {
			(const char*)(zend_uintptr_t)(required),
			0, // zend_type
			false,   // retrun_reference
			false,   // _is_variadic
		} );
		for(auto i=args.begin(); i!=args.end(); ++i) {
			if(!rset) {
				if(!ZEND_TYPE_ALLOW_NULL(i->arg_.type)) ++required;
				else rset = 1;
			}
			args_.push_back( i->arg_ );
		}
		args_.front().name = (const char*)(zend_uintptr_t)(required);
	}
	arguments::arguments(arguments&& a)
	: args_(std::move(a.args_)) {

	}
	arguments::operator zend_internal_arg_info*() const {
		return const_cast<zend_internal_arg_info*>(args_.data());
	}
	std::uint32_t arguments::size() const {
		return args_.size() - 1;
	}
}