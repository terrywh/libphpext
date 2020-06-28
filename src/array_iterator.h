#ifndef LIBPHPEXT_ARRAY_ITERATOR_H
#define LIBPHPEXT_ARRAY_ITERATOR_H

#include "vendor.h"
#include "value.h"

namespace php {
    // 数组迭代器（核心）
    class array_iterator_basic {
    public:
        using value_type = std::pair<value, value&>;
        // 空值引用
        static value undefined_value;
        // 构建：引用空数据
        array_iterator_basic(const zend_array* self, HashPosition pos)
        : self_(self)
        , pos_(pos) {
            entry_ = new value_type(undefined_value, undefined_value);
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
        void do_entry() {
            zval *val;
            val = zend_hash_get_current_data_ex(const_cast<zend_array*>(self_), &pos_);
            if(val) {
                // 注意 entry_ 内的引用不能 ”赋值“ 需要替换
                entry_ = new value_type(undefined_value, *reinterpret_cast<value*>(val));
                // 注意此函数会进行 ZVAL_STR_COPY 复制动作
                zend_hash_get_current_key_zval_ex(const_cast<zend_array*>(self_), entry_->first, &pos_);
            }
            else {
                entry_ = new value_type(undefined_value, undefined_value);
                pos_ = HT_INVALID_IDX; // 移动到了末尾 nNumUsed 修正
            }
		};
    };
    // 数组正序迭代器
    class array_iterator: public array_iterator_basic {
    public:
        using array_iterator_basic::array_iterator_basic;
        // 下一项：正序迭代器
        array_iterator& operator ++() {
            // 移动失败或移动到了末尾时做特殊标记
            if(FAILURE == zend_hash_move_forward_ex(const_cast<zend_array*>(self_), &pos_))
                pos_ = HT_INVALID_IDX;
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
                pos_ = HT_INVALID_IDX;
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
        // 起点：倒序迭代器
        static array_iterator begin(zend_array* a);
        // 钟点：倒序迭代器
        static array_iterator end(zend_array* a);

        using array_iterator_basic::array_iterator_basic;
        // 下一项：倒序迭代器
        array_reverse_iterator& operator ++();
        // 下一项：倒序迭代器
        array_reverse_iterator operator ++(int);
        // 上一项：倒序迭代器
        array_reverse_iterator& operator --();
        // 上一项：倒序迭代器
        array_reverse_iterator operator --(int);
    };
}

#endif // LIBPHPEXT_ARRAY_ITERATOR_H
