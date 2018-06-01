#pragma once

namespace php {
	class array_item_assoc: public php::value {
	private:
		php::value  array_;
		php::string assoc_;
	public:
		array_item_assoc(const php::array& arr, const php::string& key);
		array_item_assoc& operator =(const php::value& val);
		array_item_assoc& operator =(php::value&& val);

		operator php::value();
	};

	class array_item_index: public php::value {
	private:
		php::value array_;
		zend_ulong index_;
	public:
		array_item_index(const php::array& arr, zend_ulong key);
		array_item_index& operator =(const php::value& val);
		array_item_index& operator =(php::value&& val);

		operator php::value();
	};
}
