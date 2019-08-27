#pragma once

#ifndef GC_ADDREF
# define GC_ADDREF(p) (++GC_REFCOUNT(p))
#endif
#ifndef GC_DELREF
# define GC_DELREF(p) (GC_REFCOUNT(p)--)
#endif

#ifndef ZEND_CONSTANT_SET_FLAGS
#define ZEND_CONSTANT_SET_FLAGS(c,f,m) do { \
    (c)->module_number = module;              \
    (c)->flags = f;                           \
} while(0);
#endif

#ifndef ZEND_TRY_ASSIGN_COPY
#define ZEND_TRY_ASSIGN_COPY(zv, other_zv) do { \
    zval_ptr_dtor(zv);                          \
	ZVAL_COPY(zv, other_zv);                    \
} while (0)
#endif

#ifndef ZEND_TRY_ASSIGN_NULL
#define ZEND_TRY_ASSIGN_NULL(zv) do { \
	zval_ptr_dtor(zv);                \
    ZVAL_NULL(zv);                    \
} while(0)
#endif

#ifndef ZEND_TRY_ASSIGN_NEW_STR
#define ZEND_TRY_ASSIGN_NEW_STR(zv, str) do { \
    zval_ptr_dtor(zv);                        \
	ZVAL_NEW_STR(zv, str);                    \
} while(0)
#endif
