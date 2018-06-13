#pragma once

namespace php {
	class string;
	class CLASS {
	public:
		CLASS(const zend_class_entry* ce);
		CLASS(const php::string& name); // -> class_impl.hpp
		std::string name() const;
		operator zend_class_entry*() const;
	private:
		zend_class_entry* ce_;
	};
}