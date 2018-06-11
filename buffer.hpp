#pragma once

namespace php {
	class value;
	class buffer {
	public:
		inline ~buffer() {
			smart_str_free(&str_);
		}
		// 参考 smart_str 相关实现（头部额外数据，总共 256）
		explicit buffer(int size=231) {
			str_.s = nullptr;
			smart_str_alloc(&str_, size, false);
		}
		buffer(buffer&& buf)
		: str_(buf.str_) {
			buf.str_.s = nullptr;
			buf.str_.a = 0;
		}
		// 需要放入指定大小的 buffer，返回放入位置
		char* put(int size) {
			smart_str_alloc(&str_, size, false);
			char* p = str_.s->val + str_.s->len;
			str_.s->len += size;
			return p;
		}
		void  add(char c) {
			if(str_.a == 0 || str_.a - str_.s->len < 64) {
				smart_str_alloc(&str_, 64, false);
			}
			*(str_.s->val + str_.s->len) = c;
			++str_.s->len;
		}
		void  adv(int size){
			str_.s->len += size;
		}
		// 需要放入最大 size 大小（未实际放入，可能与 size 不同）
		char* rev(int size) {
			smart_str_alloc(&str_, size, false);
			return str_.s->val + str_.s->len;
		}
		// 当前放入位置
		char* current() {
			return str_.s->val + str_.s->len;
		}
		char* data() {
			return str_.s->val;
		}
		unsigned int size() const {
			return str_.s == nullptr ? 0 : str_.s->len;
		}
		int capacity() const {
			return str_.a;
		}
		bool is_null() const {
			return str_.s == nullptr;
		}
		void reset(int size = 0) {
			if(!str_.s) {
				smart_str_alloc(&str_, size, false);
			}
			str_.s->len = size;
		}
		buffer& operator =(buffer&& buf) {
			smart_str_free(&str_);
			str_ = buf.str_;
			buf.str_.s = nullptr;
			buf.str_.a = 0;
			return *this;
		}
		operator smart_str*() {
			return &str_;
		}
	private:
		smart_str str_;
		friend class string;
	};
}
