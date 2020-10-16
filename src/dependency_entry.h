#ifndef LIBPHPEXT_DEPENDENCY_ENTRY_H
#define LIBPHPEXT_DEPENDENCY_ENTRY_H

#include "vendor.h"

namespace php {

    class dependency_entry: public zend_module_dep {
    public:
        dependency_entry(const char* name, const char* ver)
        : zend_module_dep {name, "ge", ver, MODULE_DEP_REQUIRED} {}
        dependency_entry(const char* name)
        : zend_module_dep {name, "ge", nullptr, MODULE_DEP_REQUIRED} {}

        static zend_module_dep* finalize(std::vector<dependency_entry>& r) {
            // 依赖列表结束标志
            if(r.empty() || r.back().name != nullptr) {
                r.push_back({});
            }
            return r.data();
        }
    private:
        dependency_entry()
        : zend_module_dep ZEND_MOD_END {}
    };
}

#endif // LIBPHPEXT_DEPENDENCY_ENTRY_H
