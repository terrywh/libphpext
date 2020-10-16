#include "class_entry.h"

namespace php {
    void class_entry_basic::do_register(std::vector<class_entry_basic*> entry, int module) {
        for(auto& c : entry) c->do_register(module);
    }
}