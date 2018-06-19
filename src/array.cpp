#include "phpext.h"

namespace php {
	array::array()
	: array(std::size_t(0)) {

	}
	array::array(int size)
	: array(std::size_t(size)) {

	}
	array::array(std::size_t size) {
		ZVAL_NEW_ARR(&val_);
		zend_hash_init(Z_ARRVAL(val_), size, nullptr, ZVAL_PTR_DTOR, 0);
	}
	array::array(zval* v, bool ref)
	: value(v, ref) {
		
	}
	array::array(zend_array* v)
	: value(v) {

	}
	array::array(const value& v)
	: value(v/* , TYPE::ARRAY */) {

	}
	array::array(value&& v)
	: value(std::move(v)/* , TYPE::ARRAY */) {

	}
	// ---------------------------------------------------------------------
	void array::erase(const std::size_t idx) {
		zend_hash_index_del(Z_ARR_P(ptr_), idx);
	}
	void array::erase(const php::string& key) {
		zend_hash_del(Z_ARR_P(ptr_), key);
	}
	// ---------------------------------------------------------------------
	bool array::exists(const php::string& key) const {
		return zend_hash_exists(Z_ARR_P(ptr_), key);
	}
	bool array::exists(std::size_t idx) const {
		return zend_hash_index_exists(Z_ARR_P(ptr_), idx);
	}
	value array::get(std::size_t idx) const {
		return value(zend_hash_index_find(Z_ARR_P(ptr_), idx));
	}
	value array::get(const php::string& key) const {
		return value(zend_symtable_find_ind(Z_ARR_P(ptr_), key));
	}
	void array::set(std::size_t idx, const php::value& val) {
		SEPARATE_ARRAY(ptr_);
		zend_hash_index_update(Z_ARR_P(ptr_), idx, val);
		val.addref();
	}
	void array::set(const php::string& key, const php::value& val) {
		SEPARATE_ARRAY(ptr_);
		zend_symtable_update(Z_ARR_P(ptr_), key, val);
		val.addref();
	}
	array_member array::operator [](std::size_t idx) const {
		return array_member(const_cast<array&>(*this), idx);
	}
	array_member array::operator [](const php::string& key) const {
		return array_member(const_cast<array&>(*this), key);
	}
	array_member array::operator [](const char* key) const {
		return array_member(const_cast<array&>(*this), php::string(key));
	}
	// --------------------------------------------------------------------
	array_iterator array::begin() const {
		HashPosition p;
		zend_hash_internal_pointer_reset_ex(*this, &p);
		return array_iterator(const_cast<array&>(*this), p);
	}
	const array_iterator array::end() const {
		return array_iterator(const_cast<array&>(*this), HT_INVALID_IDX);
	}
	array_iterator array::rbegin() const {
		HashPosition p;
		zend_hash_internal_pointer_end_ex(*this, &p);
		return array_iterator(const_cast<array&>(*this), p);
	}
	const array_iterator array::rend() const {
		return array_iterator(const_cast<array&>(*this), HT_INVALID_IDX);
	}
}
