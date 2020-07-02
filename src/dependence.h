#ifndef LIBPHPEXT_DEPENDENCE_H
#define LIBPHPEXT_DEPENDENCE_H

#include "vendor.h"

namespace php {
    using dependence = zend_module_dep;
    // 模块依赖容器
    class dependences {
    private:
        // 模块依赖
		std::vector<dependence> deps_;

    public:
        // 添加依赖
        dependences& operator += (const dependence& dep) {
            assert(deps_.empty() || deps_.back().name != nullptr); // 禁止结束后重新添加
            deps_.push_back(dep);
            return *this;
        }
        // 返回依赖配置
        operator const zend_module_dep*() {
            // 依赖列表结束标志
            if(deps_.empty() || deps_.back().name != nullptr) {
                deps_.push_back(ZEND_MOD_END);
            }
            return deps_.data();
        }
    };

}

#endif // LIBPHPEXT_DEPENDENCE_H
