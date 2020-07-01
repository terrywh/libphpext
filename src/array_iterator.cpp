#include "array_iterator.h"

namespace php {
    // 重构当前项
    void array_iterator_basic::do_entry() {
        zval *val;
        val = zend_hash_get_current_data_ex(const_cast<zend_array*>(self_), &pos_);
        if(val) {
            // 注意 entry_ 内的引用不能 ”赋值“ 需要替换
            entry_ = new value_type({}, *reinterpret_cast<value*>(val));
            // 注意此函数会进行 ZVAL_STR_COPY 复制动作
            zend_hash_get_current_key_zval_ex(const_cast<zend_array*>(self_), entry_->first, &pos_);
        }
        else {
            entry_ = new value_type({}, env::undefined_value);
            pos_ = self_->nNumUsed;
        }
    }
}