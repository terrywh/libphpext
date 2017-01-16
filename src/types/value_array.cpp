#include "value.h"

namespace php {
	// 构建
	value value::array(std::size_t size) {
		value a;
		ZVAL_NEW_ARR(a.val_);
		_zend_hash_init(Z_ARRVAL_P(a.val_), size, ZVAL_PTR_DTOR, false); // GC_REFCOUNT(ht) = 1;
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
}
