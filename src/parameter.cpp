#include "parameter.h"
#include "value.h"

namespace php {
    //
    parameter& parameters::operator[](std::uint8_t index) const {
        return get(index);
    }
    // 
    parameter& parameters::get(std::uint8_t index) const {
        if(Z_ISREF(argv_[index])) { // 引用引用变量的内容
            zval* refv = Z_REFVAL(argv_[index]);
            SEPARATE_ZVAL_IF_NOT_REF(refv);
            return *reinterpret_cast<parameter*>(refv);
        }
        else
            return *reinterpret_cast<parameter*>(argv_ + index);
    }
    //
    void parameters::set(std::uint8_t index, const value& v) {
        // 仅对引用有效
        assert(zval_get_type(argv_ + index) == IS_REFERENCE);
        ZVAL_COPY(Z_REFVAL(argv_[index]), static_cast<zval*>(v));
    }
    //
    parameters::operator std::vector<value>() const {
        return std::vector<value>(
            reinterpret_cast<value*>(argv_), reinterpret_cast<value*>(argv_ + argc_));
    }

    parameter_iterator parameters::begin() const {
        return {this, 0};
    }

    parameter_iterator parameters::end() const {
        return {this, size()};
    }

    parameter_reverse_iterator parameters::rbegin() const {
        return {this, size() - 1};
    }
    
    parameter_reverse_iterator parameters::rend() const {
        return {this, -1};
    }
}