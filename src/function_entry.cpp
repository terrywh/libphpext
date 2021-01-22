#include "function_entry.h"

namespace php {
    // 构建函数项
    function_entry::function_entry(zif_handler fn, zend_string* name, 
            std::vector<type_desc>&& desc, uint32_t flag)
    : handler_(fn)
    , name_(name)
    , argv_(1)
    , flag_(flag) {
        // 计算必要参数信息
        int r = 0;
        for(int i=1; i<desc.size(); ++i) {
            if(!ZEND_TYPE_ALLOW_NULL( static_cast<zend_type>(desc[i]) )) ++r;
            argv_.push_back(zend_internal_arg_info {"argument", desc[i], nullptr});
        }
        // 函数必要参数个数、返回值
        argv_[0] = zend_internal_arg_info { (const char*)(zend_uintptr_t)r, desc[0], nullptr };
    }

    static std::vector< argument_entry_ref > refs; // 参数引用
    // 保留参数描述引用数据后构建 entry
    function_entry::operator zend_function_entry() const {
        // 使用容器保存参数描述信息的引用
        auto& ref = refs.emplace_back( std::move(argv_) );
        // 结构本身可复制
        return zend_function_entry {
            name_->val,  // fname
            handler_,    // handler
            ref.data(),  // arg_info, 
            // argv,
            // 容器额外包含返回信息，故实际参数个数需要调整
            static_cast<std::uint32_t>(argv_.size() - 1), // num_args // -Wnarrowing
            flag_,       // 标记
        };
    }
    // 构建 zend_function_entry 列表
    zend_function_entry* function_entry::finalize(std::vector<zend_function_entry>& entry) {
        if(entry.empty() || entry.back().fname) // 补充函数表结束标志
            entry.push_back({nullptr});
        // Zend 引擎将使用下述内存区域
        return entry.data();
    }
}