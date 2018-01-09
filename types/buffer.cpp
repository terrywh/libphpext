#include "../phpext.h"

namespace php {

	// 参考 smart_str 相关实现（头部额外数据，总共 256）
	buffer::buffer(int size) {
		str_.s = nullptr;
		smart_str_alloc(&str_, size, false);
	}
	buffer::buffer(buffer&& buf)
		: str_(buf.str_) {
		buf.str_.s = nullptr;
		buf.str_.a = 0;
	}
	// 需要放入指定大小的 buffer，返回放入位置
	char* buffer::put(int size) {
		smart_str_alloc(&str_, size, false);
		char* p = str_.s->val + str_.s->len;
		str_.s->len += size;
		return p;
	}
	void buffer::add(char c) {
		if(str_.a == 0 || str_.a - str_.s->len < 64) {
			smart_str_alloc(&str_, 64, false);
		}
		*(str_.s->val + str_.s->len) = c;
		++str_.s->len;
	}
	void buffer::adv(int size) {
		str_.s->len += size;
	}
	// 需要放入最大 size 大小（未实际放入，可能与 size 不同）
	char* buffer::rev(int size) {
		smart_str_alloc(&str_, size, false);
		return str_.s->val + str_.s->len;
	}

	buffer& buffer::operator =(buffer&& buf) {
		smart_str_free(&str_);
		str_ = buf.str_;
		buf.str_.s = nullptr;
		buf.str_.a = 0;
	}
}
