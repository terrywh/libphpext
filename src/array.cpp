#include "phpext.h"

namespace php {
	array::array(): array(std::size_t(0)) {

	}
	array::array(std::size_t size) {
		ZVAL_NEW_ARR(&value_ins);
		zend_hash_init(Z_ARRVAL(value_ins), size, nullptr, ZVAL_PTR_DTOR, 0);
	}
	// 注意: 此种构造形式无类型检查
	array::array(const zval* v)
	: value(v) {
		
	}
	array::array(zend_array* v)
	: value(v) {

	}
	array::array(const value& v)
	: value(v, TYPE::ARRAY) {

	}
	array::array(value&& v)
	: value(std::move(v), TYPE::ARRAY) {

	}
	// ---------------------------------------------------------------------
	void array::erase(const std::size_t idx) {
		zend_hash_index_del(Z_ARR(value_ins), idx);
	}
	void array::erase(const php::string& key) {
		zend_hash_del(Z_ARR(value_ins), key);
	}
	// ---------------------------------------------------------------------
	bool array::has(const php::string& key) {
		return zend_hash_exists(Z_ARR(value_ins), key);
	}
	bool array::has(std::size_t idx) {
		return zend_hash_index_exists(Z_ARR(value_ins), idx);
	}
	value array::get(std::size_t idx) const {
		return value(zend_hash_index_find(Z_ARR(value_ins), idx));
	}
	value array::get(const php::string& key) const {
		return value(zend_symtable_find_ind(Z_ARR(value_ins), key));
	}
	void array::set(std::size_t idx, const php::value& val) {
		SEPARATE_ARRAY(&value_ins);
		zend_hash_index_update(Z_ARR(value_ins), idx, val);
		val.addref();
	}
	void array::set(const php::string& key, const php::value& val) {
		SEPARATE_ARRAY(&value_ins);
		zend_symtable_update(Z_ARR(value_ins), key, val);
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
	// ------------------------------------------------------------------
	array& array::operator =(const value& v) {
		assign(v, TYPE::ARRAY);
		return *this;
	}
	array& array::operator =(value&& v) {
		assign(std::move(v), TYPE::ARRAY);
		return *this;
	}
	array& array::operator =(const zval* v) {
		value::operator =(v);
		return *this;
	}
}
