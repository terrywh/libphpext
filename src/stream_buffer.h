#pragma once

namespace php {
	class stream_buffer: public std::streambuf {
	public:
		stream_buffer(std::size_t max_size = 8 * 1024 * 1024);
		~stream_buffer();
		std::size_t max_size();
		// 可读取数据大小
		std::size_t size();
		// 可读取数据缓冲区
		const char* data();
		// 消费读取缓冲区
		void consume(std::size_t n);
		// 可写入 n 的写入缓冲区
		char* prepare(std::size_t n);
		// 提交写入的数据
		void commit(std::size_t n);
	protected:
		std::size_t max_;
		smart_str str_;
		int underflow() override;
		std::streamsize xsgetn(char* s, std::streamsize n) override;
		int overflow(int c = EOF) override;
		std::streamsize xsputn(const char* s, std::streamsize n) override;
	private:
		// 重新分配（扩容），超出最大限制时抛出异常
		void realloc(std::size_t n);

		friend class value;
	};
}