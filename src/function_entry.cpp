#include "function_entry.h"

namespace php {
    // 构建函数项
    function_entry::function_entry(zif_handler fn, std::string_view name, return_info&& ri,
        std::initializer_list<argument_info>&& pi)
    : fn_(fn)
    , nm_(name)
    , ai_(1) {
        // 计算必要参数信息
        int required = 0;
        for(auto i=pi.begin(); i!=pi.end(); ++i) {
            if(!ZEND_TYPE_ALLOW_NULL(i->type)) ++required;
            ai_.emplace_back( *i );
        }
        // 设置必要参数个数
        ri.required(required);
        ai_[0] = ri; // 参数信息缓冲
    }
    // 构建 zend_function_entry 用于注册函数
    function_entry::operator zend_function_entry() const {
        return zend_function_entry {
            nm_.c_str(), // fname
            fn_, // handler
            ai_.data(), // arg_info, 
            // 容器额外包含返回信息，故实际参数个数需要调整
            static_cast<std::uint32_t>(ai_.size() - 1), // num_args // -Wnarrowing
            0,
        };
    }
    // 构建 zend_function_entry 列表
    function_entries::operator zend_function_entry*() {
        if(entries_.empty()) { 
            for(auto& fn: refs_) entries_.push_back(fn);
            // 补充函数表结束标志
            entries_.push_back({nullptr, nullptr, nullptr, 0, 0});
        }
        return entries_.data();
    }
}