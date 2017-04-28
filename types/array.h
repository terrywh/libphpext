#pragma once

namespace php {
	class value;
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

		friend class value;
	};
}
