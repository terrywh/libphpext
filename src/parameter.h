#ifndef LIBPHPEXT_PARAMETER_H
#define LIBPHPEXT_PARAMETER_H

#include "vendor.h"
#include "value.h"

namespace php {
	using parameter = value;
	class parameter_iterator;
	class parameter_reverse_iterator;
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
		parameter& operator[](std::uint8_t index) const;
		// 获取参数
		parameter& get(std::uint8_t index) const;
		// 设置参数
		void set(std::uint8_t index, const value& v);
		//
		inline std::uint8_t length() const {
			return argc_;
		}
		//
		inline std::uint8_t size() const {
			return argc_;
		}

		parameter_iterator begin() const;
		parameter_iterator end() const;
		parameter_reverse_iterator rbegin() const;
		parameter_reverse_iterator rend() const;

		operator std::vector<value>() const;
	private:
		zval*        argv_;
		std::uint8_t argc_;
	};
	// 正序迭代器
	class parameter_iterator {
	public:
		parameter_iterator(const parameters* params, int index)
		: params_(params)
		, index_(index) {

		}
		// 访问数据项
        parameter& operator*() const {
            return params_->get(index_);
        }
        // 访问数据项
        parameter* operator->() const {
            return & params_->get(index_);
        }
		//
		parameter_iterator& operator ++() {
			++index_;
			return *this;
		}
		//
		parameter_iterator operator ++(int) const {
			parameter_iterator i(*this);
			++i;
			return i;
		}
		//
		bool operator ==(const parameter_iterator& i) const {
			return params_ == i.params_ && index_ == i.index_;
		}
		bool operator !=(const parameter_iterator& i) const {
			return params_ != i.params_ || index_ != i.index_;
		}
	private:
		const parameters* params_;
		int index_;
	};
	// 倒序迭代器
	class parameter_reverse_iterator {
	public:
		parameter_reverse_iterator(const parameters* params, int index)
		: params_(params)
		, index_(index) { }

		parameter_reverse_iterator(const parameter_reverse_iterator& ri)
		: params_(ri.params_)
		, index_(ri.index_) {}
		 // 访问数据项
        parameter& operator*() const {
            return params_->get(index_);
        }
        // 访问数据项
        parameter* operator->() const {
            return & params_->get(index_);
        }
		//
		parameter_reverse_iterator& operator ++() {
			--index_;
			return *this;
		}
		//
		parameter_reverse_iterator operator ++(int) const {
			parameter_reverse_iterator ri(*this);
			--ri.index_;
			return ri;
		}
		//
		bool operator ==(const parameter_reverse_iterator& i) const {
			return params_ == i.params_ && index_ == i.index_;
		}
		//
		bool operator !=(const parameter_reverse_iterator& i) const {
			return params_ != i.params_ || index_ != i.index_;
		}
	private:
		const parameters* params_;
		int index_;
	};
}

#endif // LIBPHPEXT_PARAMETER_H
