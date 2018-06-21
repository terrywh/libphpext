#pragma once

namespace php {
	class parameter: public value_fn {
	public:
		parameter& operator =(const value& v);
		virtual value ptr() const;
		virtual zval* raw() const; // 不适用 operator zval* 会与 value 构造发生混淆
	private:
		parameter(zval* arg);
		zval* arg_;
		friend class parameters;
		friend class value;
	};
	class parameters {
		public:
			parameters(zend_execute_data* execute_data);
			parameters(int argc, zval* argv);
			parameter operator[](std::uint8_t index) const; // ref = true
			value get(std::uint8_t index, bool ptr = false) const;
			void  set(std::uint8_t index, const value& v);
			std::uint8_t length() const;
			std::uint8_t size() const;
			operator std::vector<value>() const;
		private:
			zval*        argv_;
			std::uint8_t argc_;
	};
}
