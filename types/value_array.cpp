#include "../phpext.h"

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
	bool value::offsetExists(const char* key) {
		return offsetExists(key, std::strlen(key));
	}
	bool value::offsetExists(const char* key, std::size_t len) {
		return zend_hash_str_exists(Z_ARRVAL_P(val_), key, len);
	}
	bool value::offsetExists(const std::string& key) {
		return offsetExists(key.c_str(), key.length());
	}
	bool value::offsetExists(std::size_t index) {
		return zend_hash_index_exists(Z_ARRVAL_P(val_), index);
	}
	// 清理
	void value::offsetUnset(const char* key) {
		offsetUnset(key, std::strlen(key));
	}
	void value::offsetUnset(const std::string& key) {
		offsetUnset(key.c_str(), key.length());
	}
	void value::offsetUnset(const char* key, std::size_t len) {
		zend_hash_str_del(Z_ARRVAL_P(val_), key, len);
	}
	void value::offsetUnset(std::size_t index) {
		zend_hash_index_del(Z_ARRVAL_P(val_), index);
	}
	// 元素
	value value::operator[] (std::size_t index) {
		return item(index);
	}
	value value::operator[] (const char* key) {
		return item(key, std::strlen(key));
	}
	value value::operator[] (const std::string& key) {
		return item(key.c_str(), key.length());
	}
	value value::item(const char* key, std::size_t len) {
		zend_string* key_ = zend_string_init(key, len, false);
		zval*        item = zend_hash_find(Z_ARRVAL_P(val_), key_);
		if(item == nullptr)
		{
			zval def;
			ZVAL_UNDEF(&def);
			item = _zend_hash_add(Z_ARRVAL_P(val_), key_, &def);
		}
		zend_string_release(key_);
		return value(item, /*ref=*/true);
	}
	value value::item(std::size_t index) {
		zval* item = zend_hash_index_find(Z_ARRVAL_P(val_), index);
		if(item == nullptr)
		{
			zval def;
			ZVAL_UNDEF(&def);
			item = _zend_hash_index_add(Z_ARRVAL_P(val_), index, &def);
		}
		return value(item, /*ref=*/true);
	}
}
