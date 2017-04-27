#include "../phpext.h"

namespace php {

	// 参考 smart_str 相关实现（头部额外数据，总共 256）
	buffer::buffer(int size): po_(0) {
		str_.s = nullptr;
		smart_str_alloc(&str_, size, false);
	}
	buffer::buffer(buffer&& buf)
		: str_(buf.str_)
		, po_(buf.po_) {
		buf.str_.s = nullptr;
		buf.str_.a = 0;
		buf.po_ = 0; 
	}

	// 需要放入指定大小的 buffer，返回放入位置
	char* buffer::put(int size) {
		smart_str_alloc(&str_, size, false);
		char* p = str_.s->val + po_;
		po_ += size;
		str_.s->len = po_;
		return p;
	}
		// 需要放入最大 size 大小（未实际放入，可能与 size 不同）
	char* buffer::rev(int size) {
		smart_str_alloc(&str_, size, false);
		return str_.s->val + po_;
	}
}
