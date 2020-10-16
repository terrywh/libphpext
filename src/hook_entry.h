#ifndef LIBPHPEXT_EVENT_TARGET_H
#define LIBPHPEXT_EVENT_TARGET_H

#include "vendor.h"

namespace php {
    class module_entry;
    using hook_handler = std::function<void(module_entry&)>;
    class hook_entry {
    public:
        // 正向触发
        static void emit(std::vector<hook_handler> handler, module_entry& module) {
            for(auto i=handler.begin();i!=handler.end();++i)
                (*i)(module);
        }
        // 负向触发
        static void fire(std::vector<hook_handler> handler, module_entry& module) {
            for(auto i=handler.rbegin();i!=handler.rend();++i)
                (*i)(module);
        }
        // 构造
        template <typename Handler>
        hook_entry(Handler&& cb)
        : cb_(cb) {}

        virtual ~hook_entry() {}
    protected:
        hook_handler cb_;
        friend class module_entry;
    };
    class module_startup:   public hook_entry {
        using hook_entry::hook_entry;
    };
    class module_shutdown:  public hook_entry {
        using hook_entry::hook_entry;
    };
    class request_startup:  public hook_entry {
        using hook_entry::hook_entry;
    };
    class request_shutdown: public hook_entry {
        using hook_entry::hook_entry;
    };
}

#endif // LIBPHPEXT_EVENT_TARGET_H
