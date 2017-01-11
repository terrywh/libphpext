#include <zend.h>

#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

namespace core
{

struct user_allocator_zend
{
	typedef std::size_t    size_type;        // An unsigned integral type that can represent the size of the largest object to be allocated.
	typedef std::ptrdiff_t difference_type;  // A signed integral type that can represent the difference of any two pointers.

	// public static functions
	static char * malloc(const size_type size)
	{
		return (char*)emalloc(size);
	}
	static void free(char *const block)
	{
		return efree(block);
	}
};

}

#endif // CORE_MEMORY_H
