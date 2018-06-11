#pragma once

namespace php {
	class argument {
	public:
		argument(const char* name, bool byref = false, bool nullable = false)
		: argument(name, nullptr, TYPE::UNDEFINED, byref, nullable) {}
		argument(const char* name, const char* class_name, bool nullable = false)
		: argument(name, class_name, TYPE::OBJECT, false, nullable) {}
		// 注意：基础类型的 type_hint 检查 PHP 暂时还未实现
		argument(const char* name, zend_uchar type_hint, bool byref = false, bool nullable = false)
		: argument(name, nullptr, type_hint, byref, false) {}
	private:
		argument(const char* name, const char* class_name, zend_uchar type_hint, bool byref, bool nullable)
		: arg_ {name, class_name, type_hint, byref, nullable, false} {

		}
		zend_internal_arg_info arg_;
		friend class arguments;
	};
	class arguments {
	public:
		arguments(std::initializer_list<argument> args) {
			int required = -1, rset = false;
			args_.emplace_back( zend_internal_arg_info {
				(const char*)(zend_uintptr_t)(required),
				nullptr, // class_name
				0,       // type_hint
				false,   // retrun_reference
				false,   // allow_null
				0,
			} );
			required = 0;
			for(auto i=args.begin(); i!=args.end(); ++i) {
				if(i->arg_.allow_null && !rset) {
					args_.front().name = (const char*)(zend_uintptr_t)(required);
					rset = true;
				}
				++required;
				args_.push_back( i->arg_ );
			}
		}
		arguments(arguments&& a)
		: args_(std::move(a.args_)) {

		}
		operator zend_internal_arg_info*() const {
			return const_cast<zend_internal_arg_info*>(args_.data());
		}
		std::uint32_t size() const {
			return args_.size() - 1;
		}
	private:
		std::vector<zend_internal_arg_info> args_;
	};
}