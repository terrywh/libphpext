#pragma once

namespace php {
	class value;
	// stream_buffer 由于实现 streambuf 接口(相对较重内部指针*6) buffer 提供更简单高效的单纯缓存区
	class buffer {
	public:
		buffer(): str_({nullptr, 0}) {
			smart_str_alloc(&str_, 199, 0);
		}
		~buffer() {
			smart_str_free(&str_);
		}
		void push_back(char c) {
			if(str_.s->len + 1 > str_.a) {
				smart_str_realloc(&str_, str_.a + 64); // PHP 会自动靠齐 4096 整页
			}
			str_.s->val[str_.s->len] = c;
			++str_.s->len;
		}
		void append(const char* data, std::size_t size) {
			if(str_.s->len + size > str_.a) {
				smart_str_realloc(&str_, str_.a + size); // PHP 会自动靠齐 4096 整页
			}
			std::memcpy(&str_.s->val[str_.s->len], data, size);
			str_.s->len += size;
		}
		void append(const php::value& v);
		char* data() const {
			return const_cast<char*>(&str_.s->val[0]);
		}
		std::size_t size() const {
			return str_.s->len;
		}
		std::size_t capacity() const {
			return str_.a;
		}
		operator smart_str*() {
			return &str_;
		}
	private:
		smart_str str_;
	};
}