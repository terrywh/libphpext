#pragma once

#include "value.h"
#include "array_iterator.h"
#include "array_member.h"

namespace php {
	class parameter;
	class string;
	class property;

	class array: public value {
	public:
		array(); // undefined
		array(std::nullptr_t n);
		explicit array(int size);
		explicit array(std::size_t size);
		array(zval* v, bool ref = false);
		array(zend_array* v);
		array(const value& v);
		array(value&& v);
		array(const parameter& v);
		array(const array_member& v);
		array(const property& v);
		// ---------------------------------------------------------------------
		void erase(const std::size_t idx);
		void erase(const php::string& key);
		// ---------------------------------------------------------------------
		bool exists(const php::string& key) const;
		bool exists(std::size_t idx) const;
		value get(std::size_t idx) const;
		value get(const php::string& key) const;
		void set(std::size_t idx, const php::value& val);
		void set(const php::string& key, const php::value& val);
		array_member operator [](std::size_t idx) const;
		array_member operator [](const php::string& key) const;
		array_member operator [](const char* key) const;
		// --------------------------------------------------------------------
		array_iterator begin() const;
		const array_iterator end() const;
		array_iterator rbegin() const;
		const array_iterator rend() const;
		// ------------------------------------------------------------------
		using value::operator =;
		array& operator =(const parameter& v);
		array& operator =(const array_member& v);
		array& operator =(const property& v);
	};
}
