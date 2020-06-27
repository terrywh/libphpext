#include "string_buffer.h"

namespace php {
    // PHP 数据序列化
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
    // 消费读取缓冲区
    void string_buffer::consume(std::size_t n) {
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
    char* string_buffer::prepare(std::size_t n) {
        char* p = pptr();
        // 适当减少对 realloc 的调用
        if(!str_.s) // 1. 无空间
            realloc(n);
        else if(p + n > str_.s->val + str_.a) { // 2. 现有空间不足
            char *g = gptr();
            // 先将读取区域搬迁到头部 -> (X)
            std::memmove(str_.s->val, g, p - g);
            setg(str_.s->val, str_.s->val, str_.s->val + (p - g));
            setp(str_.s->val + (p - g), str_.s->val + str_.a);
            p = pptr();
        }
        // (X) -> 搬迁后空间依然不足
        if(p + n > str_.s->val + str_.a) {
            // 申请更大的空间
            realloc(p + n - str_.s->val);
            p = pptr();
        }
        assert(p >= str_.s->val && p < str_.s->val + str_.a);
        return p;
    }
    // 提交写入的数据
    void string_buffer::commit(std::size_t n) {
        char* p = pptr();
        if(p + n > str_.s->val + str_.a) {
            n = str_.s->val + str_.a - p;
        }
        setg(str_.s->val, gptr(), p + n);
        setp(p + n, str_.s->val + str_.a);
    }
    // 获取构建的字符串
    zend_string* string_buffer::str() {
        assert(str_.s); // 保证字符串存在

        str_.s->len = size();
        smart_str_0(&str_); // 保证 \0 结束

        zend_string* s = str_.s;
        // 当前 buffer 需要清空
        memset(&str_, 0, sizeof(smart_str));
        setg(nullptr, nullptr, nullptr);
        setp(nullptr, nullptr);

        return s;
    }
    int string_buffer::underflow() {
        char* g = gptr(), *p = pptr();
        if(egptr() < p) {
            setg(str_.s->val, g, p);
            return traits_type::to_int_type(*g);
        }else{
            return traits_type::eof();
        }
    }
    // 加速读取
    std::streamsize string_buffer::xsgetn(char* s, std::streamsize n) {
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
    int string_buffer::overflow(int c = EOF) {
        char* p = prepare(16);
        *p = c;
        commit(1);
        return traits_type::to_int_type(c);
    }
    // 加速写入
    std::streamsize string_buffer::xsputn(const char* s, std::streamsize n) {
        char* p = prepare(n);
        std::memcpy(p, s, n);
        commit(n);
        return n;
    }
}