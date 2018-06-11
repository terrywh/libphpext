#pragma once

namespace php {
	class parameter {
	public:
		parameter& operator =(const value& v) {
			if(Z_ISREF_P(arg_)) {
				zval_ptr_dtor(Z_REFVAL_P(arg_));
				// 引用参数直接使用，允许更改其变量内容
				ZVAL_COPY(Z_REFVAL_P(arg_), static_cast<zval*>(v));
			}else{
				zval_ptr_dtor(arg_);
				ZVAL_COPY(arg_, static_cast<zval*>(v));
			}
			return *this;
		}
		operator value() const {
			if(Z_ISREF_P(arg_)) return value(Z_REFVAL_P(arg_));
			else return value(arg_);
		}
	private:
		parameter(zval* arg)
		: arg_(arg) {

		}

		zval* arg_;
		friend class parameters;
		friend class value;
	};
	class parameters {
		public:
			parameters(zend_execute_data* execute_data) {
				argc_ = ZEND_CALL_NUM_ARGS(execute_data);
				if(argc_ > 0) {
					argv_ = ZEND_CALL_ARG(execute_data, 1);
				}
			}
			parameters(int argc, zval* argv)
			: argc_(argc)
			, argv_(argv) {

			}
			parameter operator[](std::uint8_t index) {
				if(index >= argc_) {
					// TODO 补充当前函数或方法名称信息?
					throw exception(zend_ce_type_error, "missing argument " + std::to_string(index+1));
				}
				return parameter(&argv_[index]);
			}
			std::uint8_t length() const {
				return argc_;
			}
			std::uint8_t size() const {
				return argc_;
			}
		private:
			zval*        argv_;
			std::uint8_t argc_;
	};
}
