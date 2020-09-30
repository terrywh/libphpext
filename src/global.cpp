#include "global.h"
#include "value.h"

namespace php {

    value& global(std::string_view name) {
        return php::array::find(&EG(symbol_table), php::string::create(name, false));
    }
}