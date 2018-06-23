#include "vendor.h"
#include "stream_buffer.h"

namespace php {
	stream_buffer::stream_buffer(std::size_t max_size)
	: str_({nullptr, 0}) {
		smart_str_alloc(&str_, 512, false);
		reset();
	}
	stream_buffer::~stream_buffer() {
		smart_str_free(&str_);
	}
	// 可读取数据大小
	std::size_t stream_buffer::size() {
		return pptr() - gptr();
	}
	// 可读取数据缓冲区
	const char* stream_buffer::data() {
		return gptr();
	}
	// 消费读取缓冲区
	void stream_buffer::consume(std::size_t n) {
		char* g = gptr(), *p = pptr();
		if(egptr() < pptr()) {
			setg(str_.s->val, g, p);
		}
		if(n > p - g) {
			n = p - g;
		}
		if(n == p - g) {
			reset();
		}else{
			gbump(n);
		}
	}
	// 可写入 n 的写入缓冲区
	char* stream_buffer::prepare(std::size_t n) {
		char* p = pptr();
		if(p + n > str_.s->val + str_.a) {
			realloc(p + n - str_.s->val);
			p = pptr();
		}
		return p;
	}
	// 提交写入的数据
	void stream_buffer::commit(std::size_t n) {
		char* p = pptr();
		if(p + n > str_.s->val + str_.a) {
			n = str_.s->val + str_.a - p;
		}
		setg(str_.s->val, gptr(), p + n);
		setp(p + n, str_.s->val + str_.a);
	}
	int stream_buffer::underflow() {
		char* g = gptr(), *p = pptr();
		if(egptr() < p) {
			setg(str_.s->val, g, p);
			return traits_type::to_int_type(*g);
		}else{
			reset();
			return traits_type::eof();
		}
	}
	std::streamsize stream_buffer::xsgetn(char* s, std::streamsize n) {
		char* g = gptr(), *p = pptr();
		if(egptr() < pptr()) {
			setg(str_.s->val, g, p);
		}
		if(n > p - g) {
			n = p - g;
		}
		std::memcpy(s, g, n);
		if(n == p - g) {
			reset();
		}else{
			gbump(n);
		}
		return n;
	}
	int stream_buffer::overflow(int c) {
		realloc(str_.a + 512);
		return sputc(c);
	}
	std::streamsize stream_buffer::xsputn(const char* s, std::streamsize n) {
		char* p = prepare(n);
		std::memcpy(p, s, n);
		p += n;
		setg(str_.s->val, gptr(), p);
		setp(p, str_.s->val + str_.a);
		return n;
	}
	void stream_buffer::realloc(std::size_t n) {
		if(n > max_) {
			throw std::range_error("realloc exceed max");
		}
		std::size_t g = gptr() - str_.s->val, p = pptr() - str_.s->val;
		smart_str_realloc(&str_, n); // 指针位置可能发生变更，需要重新设置
		setg(str_.s->val, str_.s->val + g, str_.s->val + p);
		setp(str_.s->val + p, str_.s->val + str_.a);
	}
	void stream_buffer::reset() {
		setg(str_.s->val, str_.s->val, str_.s->val);
		setp(str_.s->val, str_.s->val + str_.a);
	}
}