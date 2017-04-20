#include "../phpext.h"

namespace php {
	buffer::buffer(int size)
	: po_(0) {
		str_.s = nullptr;
		smart_str_alloc(&str_, size, false);
	}

	buffer::~buffer() {
		smart_str_free(&str_);
	}

	char* buffer::put(int size) {
		smart_str_alloc(&str_, size, false);
		char* p = str_.s->val + po_;
		po_ += size;
		str_.s->len = po_;
		return p;
	}
	
	char* buffer::rev(int size) {
		smart_str_alloc(&str_, size, false);
		return str_.s->val + po_;
	}

	int buffer::size() const {
		return str_.s->len;
	}

	int buffer::capacity() const {
		return str_.a;
	}
}
