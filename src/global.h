#ifndef LIBPHPEXT_GLOBAL_H
#define LIBPHPEXT_GLOBAL_H

#include "vendor.h"

namespace php {
    class value;
    
    value& global(std::string_view name);
}

#endif // LIBPHPEXT_GLOBAL_H
