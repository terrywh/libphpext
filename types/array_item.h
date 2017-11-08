#pragma once

namespace php {
	class array;
	class array_item: public php::value {
	private:
		php::array& array_;
		php::value  key_;
	public:
		array_item(php::array& arr, const php::value& key);
		array_item& operator =(const php::value& val);
		array_item& operator =(php::value&& val);
	};
}
