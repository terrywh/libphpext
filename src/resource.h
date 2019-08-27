#pragma once
#include "value.h"

namespace php {
    template <typename T>
    class resource: public value {
    public:
        resource() {} // undefined
        resource(std::nullptr x): value(nullptr) {}
        resource(T* x): value(zend_register_resource(x, resource_entry<T>::type())) {

        }
    };
    template <typename T>
    T* native(zend_resource* x) {
        return (T*)x->ptr;
    }
}