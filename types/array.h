#pragma once

namespace php {
	class value;
    class array_iterator;
	class array {
	private:
		zend_array* arr_;
		array(zend_array* arr);
	public:
		~array();
		array(const array& arr);
		array(array& arr);
		array(std::size_t size = 0);
		inline std::size_t length() const {
			return zend_hash_num_elements(arr_);
		}
		inline value* find(std::size_t idx) {
			return (value*)zend_hash_index_find(arr_, idx);
		}
		inline value* find(const std::string& key) {
			return find(key.c_str(), key.length());
		}
		inline value* find(const char* key, std::size_t len) {
			return (value*)zend_hash_str_find_ptr(arr_, key, len);
		}
		inline void erase(const std::size_t idx) {
			zend_hash_index_del(arr_, idx);
		}
		inline void erase(const std::string& key) {
			zend_hash_str_del(arr_, key.c_str(), key.length());
		}
		inline void erase(const char* key, std::size_t len) {
			zend_hash_str_del(arr_, key, len);
		}
		value& at(std::size_t idx);
		value& at(const char* key, std::size_t len);
		inline value& at(const std::string& key) {
			return at(key.c_str(), key.length());
		}
		inline value& operator [](std::size_t idx) {
			return at(idx);
		}
		inline value& operator [](const std::string& key) {
			return at(key.c_str(), key.length());
		}
		inline operator zend_array*() {
			return arr_;
		}
		static array clone(const array& arr);
		// array_iterator begin()
		// array_iterator end()
		// array_iterator erase(const array_iterator& i);
		// TODO erase(iterator) -> next iterator
		array_iterator begin();
		array_iterator end();

		inline std::uint32_t addref() {
			return ++GC_REFCOUNT(arr_);
		}
		inline std::uint32_t delref() {
			return --GC_REFCOUNT(arr_);
		}
		array& operator=(const array& a);
		array& operator=(array&& a);
		friend class value;
		friend class array_iterator;
	};

    class array_iterator {
        public:
            typedef std::pair<std::string, php::value>  value_type;
            typedef value_type& reference;
            typedef size_t      size_type;
            typedef ptrdiff_t   difference_type;
            typedef value_type* pointer;
        public:
            array_iterator(array arr) : b(arr.arr_->arData), end(arr.arr_->nNumUsed) { }
            array_iterator(array arr, size_t pos) : b(arr.arr_->arData), pos(pos), end(arr.arr_->nNumUsed) { }
            array_iterator(const array_iterator& ai):b(ai.b), pos(ai.pos), end(ai.end) {}
            array_iterator(array_iterator&& ai):b(ai.b), pos(ai.pos), end(ai.end) {}
            ~array_iterator() {};

            array_iterator& operator++();
            array_iterator  operator++(int);
            array_iterator& operator--();
            array_iterator  operator--(int);

            reference operator*()  { return val_   = std::move(value_type(std::move(std::string(b->key->val, b->key->len)), b->val));  }
            pointer   operator->() { return &(val_ = std::move(value_type(std::move(std::string(b->key->val, b->key->len)), b->val))); }
            array_iterator operator=(const array& arr) { b = arr.arr_->arData; pos = arr.arr_->nNumUsed; return *this;}
            array_iterator operator=(array&& arr) { b = arr.arr_->arData; pos = arr.arr_->nNumUsed; return *this;}

            array_iterator  operator+(size_t n)  {
                array_iterator tmp(*this);
                while(n--)++tmp;
                return tmp;
            }
            array_iterator& operator+=(size_t n) { while(n--) ++*this; return *this; }
            array_iterator  operator-(size_t n)  {
                array_iterator tmp(*this);
                while(n--)--tmp;
                return tmp;
            }
            array_iterator& operator-=(size_t n) { while(n--) --*this; return *this; }
            friend bool operator==(const array_iterator& lhs, const array_iterator& rhs);
            friend bool operator!=(const array_iterator& lhs, const array_iterator& rhs);

        private:
            value_type  val_;
            Bucket*     b    = nullptr;
            size_t      pos  = 0;
            size_t      end  = 0;
    };
}
