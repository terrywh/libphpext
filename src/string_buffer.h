#ifndef LIBPHPEXT_STRING_BUFFER_H
#define LIBPHPEXT_STRING_BUFFER_H

#include "forward.h"
#include "value.h"

namespace php {
    // 自动序列化
    std::ostream& operator << (std::ostream& os, const value& data) {
        smart_str buffer {nullptr, 0};
        if(!data.is(value::TYPE_STRING)) {
            php_json_encode(&buffer, data, PHP_JSON_UNESCAPED_UNICODE);
            os.write(buffer.s->val, buffer.s->len);
        }
        else {
            std::string_view sv = data;
            os.write(sv.data(), sv.size());
        }
		return os;
    }
    // 基于 smart_str 的流缓冲区
    class string_buffer: public std::streambuf {
	public:
        // 
		string_buffer()
        : str_ {nullptr, 0} {
            realloc(128);
        }
		~string_buffer() {
            smart_str_free(&str_);
        }
		// 可读取数据大小
		inline std::size_t size() {
            return pptr() - gptr();
        }
		// 可读取数据缓冲区
		inline const char* data() {
            return gptr();
        }
		// 消费读取缓冲区
		void consume(std::size_t n);
		// 可写入 n 的写入缓冲区
		char* prepare(std::size_t n);
		// 提交写入的数据
		void commit(std::size_t n);
        // 获取构建的字符串
        zend_string* str();
	protected:
		int underflow() override  {
            char* g = gptr(), *p = pptr();
            if(egptr() < p) {
                setg(str_.s->val, g, p);
                return traits_type::to_int_type(*g);
            }else{
                return traits_type::eof();
            }
        }
        // 加速读取
		std::streamsize xsgetn(char* s, std::streamsize n) override {
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
		int overflow(int c = EOF) override {
            char* p = prepare(16);
            *p = c;
            commit(1);
            return traits_type::to_int_type(c);
        }
        // 加速写入
		std::streamsize xsputn(const char* s, std::streamsize n) override {
            char* p = prepare(n);
            std::memcpy(p, s, n);
            commit(n);
            return n;
        }
	private:
        smart_str str_;
		// 重新分配（扩容），超出最大限制时抛出异常
		void realloc(std::size_t n) {
            std::size_t g, p;
            if(str_.s) {
                g = gptr() - str_.s->val;
                p = pptr() - str_.s->val;
                str_.s->len = p;
            }else{
                g = 0;
                p = 0;
            }
            smart_str_erealloc(&str_, n);
            // 指针位置可能发生变更，需要重新设置
            setg(str_.s->val, str_.s->val + g, str_.s->val + p);
            setp(str_.s->val + p, str_.s->val + str_.a);
        }
	};
}

#endif // LIBPHPEXT_STRING_BUFFER_H
