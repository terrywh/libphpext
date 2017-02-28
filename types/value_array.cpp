#include "value.h"

namespace php {
	// 构建
	value value::array(std::size_t size, bool persistent) {
		value a;
		if(persistent) {
			ZVAL_NEW_PERSISTENT_ARR(a.val_);
		}else{
			ZVAL_NEW_ARR(a.val_);
		}
		_zend_hash_init(Z_ARRVAL_P(a.val_), size, ZVAL_PTR_DTOR, persistent); // GC_REFCOUNT(ht) = 1;
		return std::move(a);
	}
	// TODO 使用 initializer_list 构建
	// 检测
	bool value::isset(const char* key) {
		return isset(key, std::strlen(key));
	}
	bool value::isset(const char* key, std::size_t len) {
		return zend_hash_str_exists(Z_ARRVAL_P(val_), key, len);
	}
	bool value::isset(const std::string& key) {
		return isset(key.c_str(), key.length());
	}
	bool value::isset(std::size_t index) {
		return zend_hash_index_exists(Z_ARRVAL_P(val_), index);
	}
	// 清理
	void value::unset(const char* key) {
		unset(key, std::strlen(key));
	}
	void value::unset(const std::string& key) {
		unset(key.c_str(), key.length());
	}
	void value::unset(const char* key, std::size_t len) {
		zend_hash_str_del(Z_ARRVAL_P(val_), key, len);
	}
	void value::unset(std::size_t index) {
		zend_hash_index_del(Z_ARRVAL_P(val_), index);
	}
	// 元素
	static value item_(zval* array, const char* key, std::size_t len) {
		zend_string* key_ = zend_string_init(key, len, false);
		zval*        val_ = zend_hash_find(Z_ARRVAL_P(array), key_);
		if(val_ == nullptr)
		{
			zval undefined;
			ZVAL_UNDEF(&undefined);
			val_ = _zend_hash_add(Z_ARRVAL_P(array), key_, &undefined);
		}
		// value will addref to val_, here we just need 1 ref
		value rv(val_, /*ref=*/true);
		rv.delref();
		return std::move(rv);
	}
	static value item_(zval* array, std::size_t index) {
		zval* val_ = zend_hash_index_find(Z_ARRVAL_P(array), index);
		if(val_ == nullptr)
		{
			zval undefined;
			ZVAL_UNDEF(&undefined);
			val_ = _zend_hash_index_add(Z_ARRVAL_P(array), index, &undefined);
		}
		// value will addref to val_, here we just need 1 ref
		value rv(val_, /*ref=*/true);
		rv.delref();
		return std::move(rv);
	}

	value value::operator[] (std::size_t index) {
		return item_(val_, index);
	}
	value value::operator[] (const char* key) {
		return item_(val_, key, std::strlen(key));
	}
	value value::operator[] (const std::string& key) {
		return item_(val_, key.c_str(), key.length());
	}
}
