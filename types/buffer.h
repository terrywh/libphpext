#pragma once

namespace php {
	class value;
	class buffer {
	public:
		// 参考 smart_str 相关实现（头部额外数据，总共 256）
		buffer(int size=231);
		inline ~buffer() {
			smart_str_free(&str_);
		}
		// buffer 不允许复制
		buffer(buffer&& buf);
		// 需要放入指定大小的 buffer，返回放入位置
		char* put(int size);
		// 需要放入最大 size 大小（未实际放入，可能与 size 不同）
		char* rev(int size);
		inline int size() const {
			return str_.s->len;
		}
		inline int capacity() const {
			return str_.a;
		}
		inline operator char*() {
			return str_.s->val;
		}
		inline operator zend_string*() {
			return str_.s;
		}
	private:
		smart_str str_;
		int po_;
		friend class value;
	};
}
