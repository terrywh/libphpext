#pragma once

namespace php {
	class object;
	class property: public value {
	private:
		php::object object_; // 需要复制一份对象的引用, 防止对象提前销毁
		php::string   name_;
	public:
		property(const php::object& obj, const php::string& name);
		property(const php::object& obj, const char* name, size_t size = -1) 
		: property(obj, php::string(name, (size == -1 ? std::strlen(name) : size))) {}
		property& operator =(const php::value& val);
		property& operator =(php::value&& val);
	};
}
