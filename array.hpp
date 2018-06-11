#pragma once

namespace php {
	class array: public value {
	public:
		array(): array(std::size_t(0)) {

		}
		explicit array(std::size_t size) {
			ZVAL_NEW_ARR(&value_ins);
			zend_hash_init(Z_ARRVAL(value_ins), size, nullptr, ZVAL_PTR_DTOR, 0);
		}
		array(zend_array* v)
		: value(v) {

		}
		array(const value& v)
		: value(v, TYPE::ARRAY) {

		}
		array(value&& v)
		: value(std::move(v), TYPE::ARRAY) {

		}
		// ---------------------------------------------------------------------
		void erase(const std::size_t idx) {
			zend_hash_index_del(Z_ARR(value_ins), idx);
		}
		void erase(const php::string& key) {
			zend_hash_del(Z_ARR(value_ins), key);
		}
		// ---------------------------------------------------------------------
		bool has(const php::string& key) {
			return zend_hash_exists(Z_ARR(value_ins), key);
		}
		bool has(std::size_t idx) {
			return zend_hash_index_exists(Z_ARR(value_ins), idx);
		}
		value get(std::size_t idx) const {
			return value(zend_hash_index_find(Z_ARR(value_ins), idx));
		}
		value get(const php::string& key) const {
			return value(zend_symtable_find_ind(Z_ARR(value_ins), key));
		}
		void set(std::size_t idx, const php::value& val) {
			SEPARATE_ARRAY(&value_ins);
			zend_hash_index_update(Z_ARR(value_ins), idx, val);
			val.addref();
		}
		void set(const php::string& key, const php::value& val) {
			SEPARATE_ARRAY(&value_ins);
			zend_symtable_update(Z_ARR(value_ins), key, val);
			val.addref();
		}
		array_member operator [](std::size_t idx) const {
			return array_member(const_cast<array&>(*this), idx);
		}
		array_member operator [](const php::string& key) const {
			return array_member(const_cast<array&>(*this), key);
		}
		array_member operator [](const char* key) const {
			return array_member(const_cast<array&>(*this), php::string(key));
		}
		// --------------------------------------------------------------------
		array_iterator begin() const {
			HashPosition p;
			zend_hash_internal_pointer_reset_ex(*this, &p);
			return array_iterator(const_cast<array&>(*this), p);
		}
		const array_iterator end() const {
			return array_iterator(const_cast<array&>(*this), HT_INVALID_IDX);
		}
		array_iterator rbegin() const {
			HashPosition p;
			zend_hash_internal_pointer_end_ex(*this, &p);
			return array_iterator(const_cast<array&>(*this), p);
		}
		const array_iterator rend() const {
			return array_iterator(const_cast<array&>(*this), HT_INVALID_IDX);
		}
		// ------------------------------------------------------------------
		array& operator =(const value& v) {
			assign(v, TYPE::ARRAY);
			return *this;
		}
		array& operator =(value&& v) {
			assign(std::move(v), TYPE::ARRAY);
			return *this;
		}
		// ------------------------------------------------------------------
		static array server() {
			php::array symbol(&EG(symbol_table));
			return symbol.get({"_SERVER", 7});
		}
	};
}
