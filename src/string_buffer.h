#ifndef LIBPHPEXT_STRING_BUFFER_H
#define LIBPHPEXT_STRING_BUFFER_H

#include "vendor.h"

namespace php {
	class value;
    // 基于 smart_str 的流缓冲区，可用于 iostream 或 asio 相关缓冲区
    class string_buffer: public std::streambuf {
	public:
        // 
		string_buffer()
        : str_ {nullptr, 0} {
            realloc(128);
        }
		// 
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
        value str();
		// 
        operator smart_str*() {
            return &str_;
        }
	protected:
		int underflow() override;
        // 加速读取
		std::streamsize xsgetn(char* s, std::streamsize n) override;
		int overflow(int c = EOF) override;
        // 加速写入
		std::streamsize xsputn(const char* s, std::streamsize n) override;
	private:
        smart_str str_;
		// 重新分配（扩容），超出最大限制时抛出异常
		void realloc(std::size_t n);
	};
}

#endif // LIBPHPEXT_STRING_BUFFER_H
