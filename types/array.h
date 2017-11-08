#pragma once

namespace php {
	class value;
	class array_iterator;
	class array : public value {
	public:
		array();
		array(int size): array(std::size_t(size)) {}
		array(std::size_t size);
		array(const array& a2): value(a2) {}
		array(zend_array*  a1, bool create = false): value(a1, create) {}
		array(array&& a2): value(std::move(a2)) {}
		// ---------------------------------------------------------------------
		// 全局对象
		// ---------------------------------------------------------------------
		static array server() {
			php::array symbol(&EG(symbol_table));
			return static_cast<zend_array*>(symbol.at("_SERVER", 7));
		}
		inline std::size_t length() const {
			return zend_hash_num_elements(Z_ARR(value_));
		}
		// inline array_item find(std::size_t idx) {
		// 	return (value*)zend_hash_index_find(Z_ARR(value_), idx);
		// }
		// inline value* find(const std::string& key) {
		// 	return find(key.c_str(), key.length());
		// }
		// inline value* find(const char* key, std::size_t len) {
		// 	return (value*)zend_hash_str_find(Z_ARR(value_), key, len);
		// }
		inline void erase(const std::size_t idx) {
			zend_hash_index_del(Z_ARR(value_), idx);
		}
		inline void erase(const std::string& key) {
			zend_hash_str_del(Z_ARR(value_), key.c_str(), key.length());
		}
		inline void erase(const char* key, std::size_t len) {
			zend_hash_str_del(Z_ARR(value_), key, len);
		}
		bool is_a_list();
		bool is_a_map();
		inline array_item at(std::size_t idx) {
			return array_item(*this, idx);
		}
		inline array_item at(const php::string& key) {
			return array_item(*this, key);
		}
		inline array_item at(const char* key, std::size_t len) {
			return at(php::string(key, len));
		}
		inline array_item at(const std::string& key) {
			return at(key.c_str(), key.length());
		}
		inline array_item operator [](int idx) {
			return at(std::size_t(idx));
		}
		inline array_item operator [](std::size_t idx) {
			return at(idx);
		}
		inline array_item operator [](const php::string& key) {
			return at(key);
		}
		inline array_item operator [](const std::string& key) {
			return at(key.c_str(), key.length());
		}
		inline array_item operator [](const char* key) {
			return at(key, std::strlen(key));
		}
		array_iterator begin();
		array_iterator end();
		using value::operator =;
		array& operator = (const array& array);
		array& operator = (array&& array);
		using value::operator ==;

		inline operator zend_array*() const {
			return Z_ARR(value_);
		}
		friend class array_iterator;
		friend class array_item_str;
		friend class array_item_idx;
	};

	class array_iterator {
	public:
		typedef std::pair<php::value, php::value>  value_type;
		typedef value_type& reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		typedef value_type* pointer;
	public:
		array_iterator(array& a, size_t pos = 0)
			: buk_(static_cast<zend_array*>(a)->arData)
			, pos_(pos)
			, end_(static_cast<zend_array*>(a)->nNumUsed) { }
		array_iterator(const array_iterator& ai)
			: buk_(ai.buk_)
			, pos_(ai.pos_)
			, end_(ai.end_) {}

		array_iterator& operator++();
		array_iterator  operator++(int);
		array_iterator& operator--();
		array_iterator  operator--(int);

		reference operator*();
		pointer   operator->();

		array_iterator  operator+(size_t n)  {
			array_iterator tmp(*this);
			while(n--) ++tmp;
			return tmp;
		}
		array_iterator& operator+=(size_t n) {
			while(n--) ++*this;
			return *this;
		}
		array_iterator  operator-(size_t n)  {
			array_iterator tmp(*this);
			while(n--) --tmp;
			return tmp;
		}
		array_iterator& operator-=(size_t n) {
			while(n--) --*this;
			return *this;
		}
		friend bool operator==(const array_iterator& lhs, const array_iterator& rhs);
		friend bool operator!=(const array_iterator& lhs, const array_iterator& rhs);

	private:
		value_type  val_;
		Bucket*     buk_;
		size_t      pos_;
		size_t      end_;
	};
}
