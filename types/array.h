#pragma once

namespace php {
	class value;
	class array_iterator;
	class array_item_index;
	class array_item_assoc;
	class array : public value {
	public:
		using value::value;
		using value::operator =;
		using value::operator ==;
		using value::operator zval*;
		using value::operator zend_array*;
		
		array(): value() {}
		explicit array(int size): array(std::size_t(size)) {}
		explicit array(std::size_t size) {
			ZVAL_NEW_ARR(&value_);
			_zend_hash_init(Z_ARRVAL(value_), size, ZVAL_PTR_DTOR, 0 ZEND_FILE_LINE_RELAY_CC);
		}
		array(const php::value& v): value(v) {}
		array(php::value&& v): value(std::move(v)) {}
		// ---------------------------------------------------------------------
		// 全局对象
		// ---------------------------------------------------------------------
		static array server() {
			php::array symbol(&EG(symbol_table));
			return symbol.at("_SERVER", 7);
		}
		inline std::size_t length() const {
			return zend_array_count(Z_ARR(value_));
		}
		inline void erase(const std::size_t idx) {
			zend_hash_index_del(Z_ARR(value_), idx);
		}
		inline void erase(const std::string& key) {
			zend_hash_str_del(Z_ARR(value_), key.c_str(), key.length());
		}
		inline void erase(const char* key, std::size_t len) {
			zend_hash_str_del(Z_ARR(value_), key, len);
		}
		inline array_item_index at(std::size_t idx) {
			return array_item_index(*this, idx);
		}
		inline array_item_index operator [](int idx) {
			return at(std::size_t(idx));
		}
		inline array_item_index operator [](std::size_t idx) {
			return at(idx);
		}
		inline array_item_assoc at(const php::string& key) {
			return array_item_assoc(*this, key);
		}
		inline array_item_assoc at(const char* key, std::size_t len) {
			return at(php::string(key, len));
		}
		inline array_item_assoc at(const std::string& key) {
			return at(key.c_str(), key.length());
		}
		inline array_item_assoc operator [](const php::string& key) {
			return at(key);
		}
		inline array_item_assoc operator [](const std::string& key) {
			return at(key.c_str(), key.length());
		}
		inline array_item_assoc operator [](const char* key) {
			return at(key, strlen(key));
		}
		inline bool has(const std::string& key) {
			return has(key.c_str(), key.length());
		}
		inline bool has(const php::string& key) {
			return zend_hash_exists(Z_ARR(value_), key);
		}
		inline bool has(const char* key, std::size_t len) {
			return zend_hash_str_exists(Z_ARR(value_), key, len);
		}
		inline bool has(int idx) {
			return zend_hash_index_exists(Z_ARR(value_), idx);
		}
		array_iterator begin() const;
		const array_iterator end() const;

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
		array_iterator(const array& a, size_t pos)
			: buk_(static_cast<zend_array*>(a)->arData)
			, pos_(pos)
			, end_(static_cast<zend_array*>(a)->nNumUsed) {

			}
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
