#pragma once

namespace php {
	class value;
	// stream_buffer 由于实现 streambuf 接口(相对较重内部指针*6) buffer 提供更简单高效的单纯缓存区
	class buffer {
	public:
		buffer(std::size_t max = 16 * 1024 * 1024)
		: max_(max)
		, str_({nullptr, 0})
		, get_(0) {
			smart_str_alloc(&str_, 199, 0);
		}
		buffer(buffer&& b)
		: max_(b.max_)
		, str_(b.str_)
		, get_(b.get_) {
			b.str_.s = nullptr;
			b.str_.a = 0;
		}
		~buffer() {
			smart_str_free(&str_);
		}
		void push_back(char c) {
			if(!str_.s || str_.s->len + 1 > str_.a) {
				smart_str_realloc(&str_, str_.a + 64); // PHP 会自动靠齐 4096 整页
			}
			str_.s->val[str_.s->len] = c;
			++str_.s->len;
		}
		void append(const char* data, std::size_t size) {
			if(!str_.s || str_.s->len + size > str_.a) {
				smart_str_realloc(&str_, str_.a + size); // PHP 会自动靠齐 4096 整页
			}
			std::memcpy(&str_.s->val[str_.s->len], data, size);
			str_.s->len += size;
		}
		void append(const php::value& v);
		char* prepare(std::size_t size) {
			if(!str_.s || str_.s->len + size > str_.a) {
				smart_str_realloc(&str_, str_.s->len + size); // PHP 会自动靠齐 4096 整页
			}
			return &str_.s->val[str_.s->len];
		}
		void commit(std::size_t size) {
			if(!str_.s) {
				return;
			}
			if(size > str_.a - str_.s->len) {
				size = str_.a - str_.s->len;
			}
			str_.s->len += size;
		}
		char* data() const {
			return const_cast<char*>(str_.s->val + get_);
		}
		void consume(std::size_t size) {
			if(!str_.s) return;
			
			if(get_ + size >= str_.s->len) {
				get_ = 0;
				str_.s->len = 0;
			}else{
				get_ += size;
			}
		}
		std::size_t size() const {
			return str_.s ? (str_.s->len - get_): 0;
		}
		std::size_t capacity() const {
			return str_.a - get_;
		}
		std::size_t max_size() const {
			return max_;
		}
	private:
		std::size_t max_;
		smart_str   str_;
		std::size_t get_;

		friend class value;
	};
}