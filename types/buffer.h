#pragma once

namespace php {
	class value;
	class buffer {
	public:
		inline ~buffer() {
			smart_str_free(&str_);
		}
		// 参考 smart_str 相关实现（头部额外数据，总共 256）
		buffer(int size=231);
		buffer(buffer&& buf);
		// 需要放入指定大小的 buffer，返回放入位置
		char* put(int size);
		void  adv(int size);
		// 需要放入最大 size 大小（未实际放入，可能与 size 不同）
		char* rev(int size);
		// 当前放入位置
		inline char* current() {
			return str_.s->val + str_.s->len;
		}
		inline char* data() {
			return str_.s->val;
		}
		inline unsigned int size() const {
			return str_.s->len;
		}
		inline int capacity() const {
			return str_.a;
		}
		inline bool is_null() const {
			return str_.s == nullptr;
		}
		void reset(int size = 0) {
			str_.s->len = size;
		}
		inline operator char*() {
			return str_.s->val;
		}
		inline operator zend_string*() {
			return str_.s;
		}
		inline operator smart_str*() {
			return &str_;
		}
		buffer& operator =(buffer&& buf);
	private:
		smart_str str_;
		friend class value;
		friend class string;
	};
}
