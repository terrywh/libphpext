#ifndef LIBPHPEXT_ARRAY_ITERATOR_H
#define LIBPHPEXT_ARRAY_ITERATOR_H

#include "vendor.h"
#include "value.h"
#include "env.h"

namespace php {
    // 数组迭代器（核心）
    class array_iterator_basic {
    public:
        using value_type = std::pair<value, value&>;
        // 构建：引用空数据
        array_iterator_basic(const zend_array* self, HashPosition pos)
        : self_(self)
        , pos_(pos) {
            entry_ = new value_type({}, env::undefined_value);
            do_entry();
        }
        // 访问数据项
        value_type& operator*() {
            return *entry_;
        }
        // 访问数据项
        value_type* operator->() {
            return  entry_;
        }
        // 
        bool operator !=(const array_iterator_basic& i) const {
            assert(self_ == i.self_); // 不能比较两个无关的迭代器
            return pos_ != i.pos_;
        }
    protected:
        const zend_array* self_;
        HashPosition pos_;
        value_type* entry_;
        // 重构当前项
        void do_entry();
    };
    // 数组正序迭代器
    class array_iterator: public array_iterator_basic {
    public:
        using array_iterator_basic::array_iterator_basic;
        // 下一项：正序迭代器
        array_iterator& operator ++() {
            // 移动失败或移动到了末尾时做特殊标记
            if(FAILURE == zend_hash_move_forward_ex(const_cast<zend_array*>(self_), &pos_))
                pos_ = self_->nNumUsed;
            do_entry();
            return *this;
        }
        // 下一项：正序迭代器
        array_iterator operator ++(int) {
            array_iterator i(self_, pos_);
            ++i;
            return i;
        }
        // 上一项：正序迭代器
        array_iterator& operator --() {
            if(pos_ == HT_INVALID_IDX) pos_ = self_->nNumUsed;
            if(FAILURE == zend_hash_move_backwards_ex(const_cast<zend_array*>(self_), &pos_))
                pos_ = self_->nNumUsed;
            do_entry();
            return *this;
        }
        // 上一项：正序迭代器
        array_iterator operator --(int) {
            array_iterator i(self_, pos_);
            --i;
            return i;
        }
    };
    // 数组倒序迭代器
    class array_reverse_iterator: public array_iterator_basic {
    public:
        using array_iterator_basic::array_iterator_basic;
        // 下一项：倒序迭代器
        array_reverse_iterator& operator ++() {
            if(FAILURE == zend_hash_move_backwards_ex(const_cast<zend_array*>(self_), &pos_))
                pos_ = self_->nNumUsed;
            do_entry();
            return *this;
        }
        // 下一项：倒序迭代器
        array_reverse_iterator operator ++(int) {
            array_reverse_iterator ri {self_, pos_};
            ++ri;
            return ri;
        }
        // 上一项：倒序迭代器
        array_reverse_iterator& operator --() {
            if(FAILURE == zend_hash_move_forward_ex(const_cast<zend_array*>(self_), &pos_))
                pos_ = self_->nNumUsed;
            do_entry();
            return *this;
        }
        // 上一项：倒序迭代器
        array_reverse_iterator operator --(int) {
            array_reverse_iterator ri {self_, pos_};
            --ri;
            return ri;
        }
    };
}

#endif // LIBPHPEXT_ARRAY_ITERATOR_H
