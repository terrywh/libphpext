#pragma once

namespace php {
	class object;
	class property: public value {
	private:
		const php::object& object_;
		php::string  name_;
	public:
		property(const php::object& obj, const php::string& name, bool silent = false);
		property& operator =(const php::value& val);
		property& operator =(php::value&& val);
	};
}
