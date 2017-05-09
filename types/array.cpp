#include "../phpext.h"

namespace php {
	array array::clone(const array& a1) {
		array a2(a1.length());
		zend_hash_copy(a2.arr_, a1.arr_, nullptr);
	}
	array::~array() {
		if(arr_ != nullptr && delref() == 0) {
			zend_array_destroy(arr_);
		}
	}
	array::array(const array& arr):arr_(arr.arr_) {
		addref();
	}
	array::array(array&& arr):arr_(arr.arr_) {
		arr.arr_ = nullptr;
	}
	array::array(zend_array* arr):arr_(arr) {
		addref();
	}
	array::array(std::size_t size) {
		arr_ = (zend_array*)emalloc(sizeof(zend_array));
		_zend_hash_init(arr_, size, ZVAL_PTR_DTOR, 0 ZEND_FILE_LINE_RELAY_CC);
	}
	value& array::at(std::size_t idx) {
		value* item = (value*)zend_hash_index_find(arr_, idx);
		if(item == nullptr) {
			item = (value*)zend_hash_index_add_empty_element(arr_, idx);
		}
		return *item;
	}
	value& array::at(const char* key, std::size_t len) {
		zend_string* key_ = zend_string_init(key, len, false);
		value* item = (value*)zend_hash_find(arr_, key_);
		if(item == nullptr) {
			item = (value*)zend_hash_add_empty_element(arr_, key_);
		}
		zend_string_release(key_);
		return *item;
	}
    array_iterator array::begin() {
        return std::move(array_iterator(*this));
    }
    array_iterator array::end() {
        return std::move(array_iterator(*this, arr_->nNumUsed));
    }

	array& array::operator=(const array& a) {
		arr_ = a.arr_;
		addref();
		return *this;
	}
	array& array::operator=(array&& a) {
		arr_ = a.arr_;
		a.arr_ = nullptr;
		return *this;
	}
    array_iterator& array_iterator::operator++() {
        assert(pos < end);   // throw
        zval* z = nullptr;
        do {
            z = &(b + ++pos)->val;
        } while(Z_TYPE_P(z) == IS_UNDEF && pos < end);
        return *this;
    }
    array_iterator  array_iterator::operator++(int) {
        assert(pos < end);   // throw
        //if( pos >= end) return *this;
        array_iterator ai = *this;
        zval* z = nullptr;
        do {
            z = &(b + ++pos)->val;
        } while(Z_TYPE_P(z) == IS_UNDEF && pos < end);
        return ai;
    }
    array_iterator& array_iterator::operator--() {
        assert(pos > 0);   // throw
        //if(pos == 0) return *this;
        zval* z = nullptr;
        do {
            z = &(b + --pos)->val;
        } while(Z_TYPE_P(z) == IS_UNDEF && pos > 0);
        return *this;
    }
    array_iterator  array_iterator::operator--(int) {
        assert(pos > 0);   // throw
        //if(pos == 0) return *this;
        array_iterator ai = *this;
        zval* z = nullptr;
        do {
            z = &(b + --pos)->val;
        } while(Z_TYPE_P(z) == IS_UNDEF && pos > 0);
        return ai;
    }

    array_iterator::reference array_iterator::operator*()  { 
        return val_   = value_type(std::string((b + pos)->key->val, (b + pos)->key->len), (b + pos)->val);  }
    array_iterator::pointer   array_iterator::operator->() { 
        return &(val_ = value_type(std::string((b + pos)->key->val, (b + pos)->key->len), (b + pos)->val)); }
    bool operator==(const array_iterator& lhs, const array_iterator& rhs) { 
        if(lhs.b == rhs.b && lhs.pos == rhs.pos) {
            return true;
        }
        return false;
    }
    bool operator!=(const array_iterator& lhs, const array_iterator& rhs) {
        if(lhs.b != rhs.b || lhs.pos != rhs.pos) {
            return true;
        }
        return false;
    }
}
