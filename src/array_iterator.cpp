#include "array_iterator.h"
#include "value.h"
#include "env.h"

namespace php {
    // 构建：引用空数据
    array_iterator_basic::array_iterator_basic(const zend_array* self, HashPosition pos)
    : self_(self)
    , pos_(pos)
    , entry_(nullptr) {
        do_entry();
    }
    // 重构当前项
    void array_iterator_basic::do_entry() {
        if(entry_) pefree(entry_, false);
        zval *val;
        val = zend_hash_get_current_data_ex(const_cast<zend_array*>(self_), &pos_);
        if(val) {
            // 注意 entry_ 内的引用不能 ”赋值“ 需要替换
            entry_ = reinterpret_cast<entry_type*>pemalloc(sizeof(entry_type), false);
            entry_ = new (entry_) entry_type({}, *reinterpret_cast<value*>(val));
            // 注意此函数会进行 ZVAL_STR_COPY 复制动作
            zend_hash_get_current_key_zval_ex(const_cast<zend_array*>(self_), entry_->first, &pos_);
        }
        else {
            entry_ = reinterpret_cast<entry_type*>pemalloc(sizeof(entry_type), false);
            entry_ = new (entry_) entry_type({}, env::undefined_value);
            pos_ = self_->nNumUsed; // 结束标记
        }
    }
}