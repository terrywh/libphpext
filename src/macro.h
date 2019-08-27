#pragma once

#if PHP_VERSION_ID < 70300
#define GC_DELREF(refc) --GC_REFCOUNT(refc)
#define GC_ADDREF(refc) ++GC_REFCOUNT(refc)
#define ZEND_CONSTANT_SET_FLAGS(c,f,m) do { \
    c->module_number = module;              \
    c->flags = f;                           \
} while(0);
#endif