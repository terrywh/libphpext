#pragma once

namespace php {
	class callable: public value {
	private:
		static value __call(const zval* cb);
		static value __call(const zval* cb, std::vector<value> argv);
	public:
		// 注意: 此种构造形式无类型检查
		callable(const zval* v, bool copy = true);
		callable(const char* fname);
		callable(const std::string& fname);
		callable(zend_object* cb);
		callable(std::function<php::value (php::parameters& params)> v);
		callable(const value& v);
		callable(value&& v);
		// ---------------------------------------------------------------------
		value call() const;
		value call(std::vector<value> argv) const;
		value operator()() const;
		value operator()(std::vector<value> argv) const;
		// -------------------------------------------------------------------
		callable& operator =(const zval* v);
		callable& operator =(const value& v);
		callable& operator =(value&& v);

	};
}
