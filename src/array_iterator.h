#pragma once

#include "value.h"

namespace php {
	class array_iterator {
	public:
		typedef std::pair<value, value> value_type;
		typedef value_type& reference;
		typedef size_t      size_type;
		typedef value_type* pointer;
	private:
		php::value&  arr_;
		HashPosition pos_;
		std::shared_ptr<value_type> entry_;

		void create();
	public:
		array_iterator(value& a, HashPosition p);
		array_iterator(const array_iterator& ai);
		array_iterator& operator++();
		array_iterator  operator++(int);
		array_iterator& operator--();
		array_iterator operator--(int);
		value_type& operator*();
		value_type* operator->();
		array_iterator operator+(size_t n);
		array_iterator& operator+=(size_t n);
		array_iterator operator-(size_t n);
		array_iterator& operator-=(size_t n);
		bool operator==(const array_iterator& ai) const;
		bool operator!=(const array_iterator& ai) const;
	};
	
}