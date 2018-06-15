#include "phpext.h"

namespace php {
	argument::argument(const char* name, bool byref, bool nullable)
	: argument(name, nullptr, TYPE::UNDEFINED, byref, nullable) {}
	argument::argument(const char* name, const char* class_name, bool nullable)
	: argument(name, class_name, TYPE::OBJECT, false, nullable) {}
	// 注意：基础类型的 type_hint 检查 PHP 暂时还未实现
	argument::argument(const char* name, TYPE type_hint, bool byref, bool nullable)
	: argument(name, nullptr, type_hint, byref, nullable) {}
	argument::argument(const char* name, const char* class_name, zend_uchar type_hint, bool byref, bool nullable)
	: arg_ {name, class_name, type_hint, byref, nullable, false} {

	}
	arguments::arguments(std::initializer_list<argument> args) {
		int required = 0, rset = 0;
		args_.emplace_back( zend_internal_arg_info {
			(const char*)(zend_uintptr_t)(required),
			nullptr, // class_name
			0,       // type_hint
			false,   // retrun_reference
			false,   // allow_null
			0,
		} );
		for(auto i=args.begin(); i!=args.end(); ++i) {
			if(!rset) {
				if(!i->arg_.allow_null) ++required;
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