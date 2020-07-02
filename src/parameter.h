#ifndef LIBPHPEXT_PARAMETER_H
#define LIBPHPEXT_PARAMETER_H

#include "vendor.h"
#include "value.h"

namespace php {
	using parameter = value;
	// 参数表
	class parameters {
	public:
		// 提取当前函数调用参数
		parameters(zend_execute_data* execute_data)
		: argc_(ZEND_CALL_NUM_ARGS(execute_data))
		, argv_(nullptr) {
			if(argc_ > 0) { // _zend_get_parameters_array_ex
				argv_ = ZEND_CALL_ARG(execute_data, 1);
			}
		}
		// 自行构建函数参数
		parameters(int argc, zval* argv)
		: argc_(argc)
		, argv_(argv) {}
        // 自行构建函数参数
		parameters(std::vector<value>& v)
		: argc_(v.size())
		, argv_(reinterpret_cast<zval*>(v.data())) {}
		// 获取参数
		parameter& operator[](std::uint8_t index) const {
			return get(index);
		}
		// 获取参数
		parameter& get(std::uint8_t index) const {
			if(Z_ISREF(argv_[index])) // 引用引用变量的内容
				return *reinterpret_cast<parameter*>( Z_REFVAL_P(argv_ + index) );
			else
				return *reinterpret_cast<parameter*>(argv_ + index);
		}
		// 设置参数
		void set(std::uint8_t index, const value& v) {
			// 仅对引用有效
			assert(zval_get_type(argv_ + index) == IS_REFERENCE);
			ZVAL_COPY(Z_REFVAL(argv_[index]), static_cast<zval*>(v));
		}
		std::uint8_t length() const {
			return argc_;
		}
		std::uint8_t size() const {
			return argc_;
		}
		operator std::vector<value>() const {
			return std::vector<value>(
				reinterpret_cast<value*>(argv_), reinterpret_cast<value*>(argv_ + argc_));
		}
	private:
		zval*        argv_;
		std::uint8_t argc_;
	};
}

#endif // LIBPHPEXT_PARAMETER_H
