#include "vendor.h"
#include "stream_buffer.h"

namespace php {
	stream_buffer::stream_buffer(std::size_t max_size)
	: max_(max_size)
	, str_({nullptr, 0}) {
		realloc(199);
	}
	stream_buffer::~stream_buffer() {
		smart_str_free(&str_);
	}
	std::size_t stream_buffer::max_size() {
		return max_;
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
		gbump(n);
	}
	// 可写入 n 的写入缓冲区
	char* stream_buffer::prepare(std::size_t n) {
		char* p = pptr();
		if(p + n > str_.s->val + str_.a) { // 1. 不够
			char *g = gptr();
			// 读取区域搬迁到头部
			std::memmove(str_.s->val, g, p - g);
			setg(str_.s->val, str_.s->val, str_.s->val + (p - g));
			setp(str_.s->val + (p - g), str_.s->val + str_.a);
			p = pptr();
		}
		if(p + n > str_.s->val + str_.a) { // 2. 不够
			// 申请更大的空间
			realloc(p + n - str_.s->val);
			p = pptr();
		}
		assert(p >= str_.s->val && p < str_.s->val + str_.a);
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
		gbump(n);
		return n;
	}
	int stream_buffer::overflow(int c) {
		char* p = prepare(512);
		*p = c;
		commit(1);
		return traits_type::to_int_type(c);
	}
	std::streamsize stream_buffer::xsputn(const char* s, std::streamsize n) {
		char* p = prepare(n);
		std::memcpy(p, s, n);
		p += n;
		commit(n);
		return n;
	}
	void stream_buffer::realloc(std::size_t n) {
		if(n > max_) {
			throw std::range_error("realloc exceed max");
		}
		std::size_t g, p;
		if(str_.s) {
			g = gptr() - str_.s->val;
			p = pptr() - str_.s->val;
		}else{
			g = 0;
			p = 0;
		}
		smart_str_erealloc(&str_, n); // 指针位置可能发生变更，需要重新设置
		setg(str_.s->val, str_.s->val + g, str_.s->val + p);
		setp(str_.s->val + p, str_.s->val + str_.a);
	}
}