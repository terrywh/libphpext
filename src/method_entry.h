#ifndef LIBPHPEXT_FUNCTION_ENTRY_METHOD_H
#define LIBPHPEXT_FUNCTION_ENTRY_METHOD_H

#include "vendor.h"
#include "value.h"
#include "parameter.h"
#include "function_entry.h"
#include "class_entry.h"

namespace php {
    template <auto>
    class method;
    template <class T, value (T::*M)(parameters& params)>
    class method<M> {
    public:
        method(const char* name, std::initializer_list<argument_entry> args)
        : entry_{ class_entry_basic::method<T,M>, name, std::move(args), ZEND_ACC_PUBLIC } {}
        method(const char* name)
        : entry_{ class_entry_basic::method<T,M>, name, {}, ZEND_ACC_PUBLIC } {}

        method(zend_string* name, std::initializer_list<argument_entry> args)
        : entry_{ class_entry_basic::method<T,M>, name, std::move(args), ZEND_ACC_PUBLIC } {}
        method(zend_string* name)
        : entry_{ class_entry_basic::method<T,M>, name, {}, ZEND_ACC_PUBLIC } {}
        
        operator zend_function_entry() const {
            return std::move(entry_);
        }
    protected:
        function_entry entry_;
    };

    template <auto>
    class private_method;
    template <class T, value (T::*M)(parameters& params)>
    class private_method<M> {
    public:
        private_method(const char* name, std::initializer_list<argument_entry> args)
        : entry_{ class_entry_basic::method<T,M>, name, std::move(args), ZEND_ACC_PRIVATE } {}
        private_method(const char* name)
        : entry_{ class_entry_basic::method<T,M>, name, {}, ZEND_ACC_PRIVATE } {}

        private_method(zend_string* name, std::initializer_list<argument_entry> args)
        : entry_{ class_entry_basic::method<T,M>, name, std::move(args), ZEND_ACC_PRIVATE } {}
        private_method(zend_string* name)
        : entry_{ class_entry_basic::method<T,M>, name, {}, ZEND_ACC_PRIVATE } {}
        
        operator zend_function_entry() const {
            return std::move(entry_);
        }
    protected:
        function_entry entry_;
    };

    template <auto>
    class protected_method;
    template <class T, value (T::*M)(parameters& params)>
    class protected_method<M> {
    public:
        protected_method(const char* name, std::initializer_list<argument_entry> args)
        : entry_{ class_entry_basic::method<T,M>, name, std::move(args), ZEND_ACC_PROTECTED } {}
        protected_method(const char* name)
        : entry_{ class_entry_basic::method<T,M>, name, { {TYPE_UNDEFINED} }, ZEND_ACC_PROTECTED } {}

        protected_method(zend_string* name, std::initializer_list<argument_entry> args)
        : entry_{ class_entry_basic::method<T,M>, name, std::move(args), ZEND_ACC_PROTECTED } {}
        protected_method(zend_string* name)
        : entry_{ class_entry_basic::method<T,M>, name, { {TYPE_UNDEFINED} }, ZEND_ACC_PROTECTED } {}
        
        operator zend_function_entry() const {
            return std::move(entry_);
        }
    protected:
        function_entry entry_;
    };
}

#endif // LIBPHPEXT_FUNCTION_ENTRY_METHOD_H
