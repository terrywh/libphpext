#pragma once

namespace php {
	class class_base {
	public:
		class_base() {}
		virtual ~class_base() {}
	protected:
		zval obj_;

		value get(const php::string& key) const {
			return property::get(php::value(&obj_), key);
		}
		void set(const php::string& key, const php::value& val) const {
			property::set(php::value(&obj_), key, val);
		}
		php::value call(const php::string& name) const {
			return object::call(php::object(&obj_), name);
		}
		php::value call(const php::string& name, const std::vector<php::value>& argv) {
			return object::call(php::object(&obj_), name, argv);
		}
		property operator [](const php::string& name) const {
			return property(&obj_, name);
		}
		property prop(const php::string& name) const {
			return property(&obj_, name);
		}
		friend class object;
		template <class T>
		friend class class_entry;
	};
}
