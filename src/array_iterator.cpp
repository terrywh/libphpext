#include "array_iterator.h"
#include "value.h"
#include "env.h"

namespace php {
    // 构建：引用空数据
    array_iterator_basic::array_iterator_basic(const zend_array* self, HashPosition pos)
    : self_(self)
    , pos_(pos)
    , entry_(nullptr) {
        build_entry();
    }
    // 复制
    array_iterator_basic::array_iterator_basic(const array_iterator_basic& it)
    : self_(it.self_)
    , pos_(it.pos_) {
        entry_ = reinterpret_cast<entry_type*>pemalloc(sizeof(entry_type), false);
        entry_ = new (entry_) entry_type(*it.entry_);
    }
    // 移动
    array_iterator_basic::array_iterator_basic(array_iterator_basic&& it)
    : self_(it.self_)
    , pos_(it.pos_) {
        entry_ = it.entry_;
        it.pos_  = self_->nNumUsed;
        it.entry_ = nullptr;
    }
    //
    array_iterator_basic& array_iterator_basic::operator =(const array_iterator_basic& it) {
        self_ = it.self_;
        pos_ = it.pos_;
        reset_entry();
        entry_ = reinterpret_cast<entry_type*>pemalloc(sizeof(entry_type), false);
        entry_ = new (entry_) entry_type(*it.entry_);
        return *this;
    }
    //
    array_iterator_basic& array_iterator_basic::operator =(array_iterator_basic&& it) {
        self_ = it.self_;
        pos_ = it.pos_;
        reset_entry();
        entry_ = it.entry_;
        it.pos_ = self_->nNumUsed;
        it.entry_ = nullptr;
        return *this;
    }
    //
    void array_iterator_basic::reset_entry() {
        if(entry_) {
            entry_->~entry_type();
            pefree(entry_, false);
            entry_ = nullptr;
        }
    }
    // 重构当前项
    void array_iterator_basic::build_entry() {
        reset_entry();
        zval *val;
        val = zend_hash_get_current_data_ex(const_cast<zend_array*>(self_), &pos_);
        if(val) {
            // 注意 entry_ 内的引用不能 ”赋值“ 需要替换
            entry_ = reinterpret_cast<entry_type*>pemalloc(sizeof(entry_type), false);
            entry_ = new (entry_) entry_type(nullptr, *reinterpret_cast<value*>(val));
            // 注意此函数会进行 ZVAL_STR_COPY 复制动作
            zend_hash_get_current_key_zval_ex(const_cast<zend_array*>(self_), entry_->first, &pos_);
        }
        else {
            entry_ = reinterpret_cast<entry_type*>pemalloc(sizeof(entry_type), false);
            entry_ = new (entry_) entry_type(nullptr, env::undefined_value);
            pos_ = self_->nNumUsed; // 结束标记
        }
    }
    // 销毁
    array_iterator::~array_iterator() {
        reset_entry();
    }
    // 销毁
    array_reverse_iterator::~array_reverse_iterator() {
        reset_entry();
    }
}