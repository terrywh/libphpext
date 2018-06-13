#pragma once

namespace php {
	class class_base {
	public:
		class_base() {}
		virtual ~class_base() {}
	protected:
		zval obj_;

		value get(const php::string& key) const;
		void set(const php::string& key, const php::value& val) const;
		php::value call(const php::string& name) const;
		php::value call(const php::string& name, const std::vector<php::value>& argv) const;
		property operator [](const php::string& name) const;
		property property(const php::string& name) const;

		friend class value;
		friend class object;
		template <class T>
		friend class class_entry;
	};
}
