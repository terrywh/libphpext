#pragma once

#include "value.h"
#include "property.h"

namespace php {
	class string;
	class class_base {
	public:
		class_base() {}
		virtual ~class_base() {}
	protected:
		zval obj_;

		value get(const string& key, bool ptr = false) const;
		void set(const string& key, const value& val) const;
		value call(const string& name) const;
		value call(const string& name, const std::vector<value>& argv) const;
		property operator [](const string& name) const;
		property property(const string& name) const;

		friend class value;
		friend class object;
		template <class T>
		friend class class_entry;
	};
}
