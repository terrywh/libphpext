#pragma once

namespace php {
	class parameter {
	public:
		parameter& operator =(const value& v);
		operator value() const;
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
			parameter operator[](std::uint8_t index) const;
			std::uint8_t length() const;
			std::uint8_t size() const;
			operator std::vector<value>() const;
		private:
			zval*        argv_;
			std::uint8_t argc_;
	};
}
