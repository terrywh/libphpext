#pragma once
#include <string>

namespace php {
    class resource_entry_base {
    public:
        virtual void declare(int module) = 0;
    };
    template <typename T>
    class resource_entry: public resource_entry_base {
    public:
        resource_entry(const std::string& name)
        : name_(name) {}
        void declare(int module) override {
            type_ = zend_register_list_destructors_ex(resource_entry<T>::destructor, nullptr, name_.c_str(), module);
        }
        static int type() {
            return type_;
        }
    private:
        std::string name_;
        static int  type_;
        static void destructor(zend_resource* res) {
            T* x = (T*)res->ptr;
            if(x) delete x;
        }
    };

    template <typename T>
    int resource_entry<T>::type_;
}