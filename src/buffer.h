#pragma once

namespace php {
	class buffer {
	public:
		~buffer();
		// 参考 smart_str 相关实现（头部额外数据，总共 256）
		explicit buffer(int size=231);
		buffer(buffer&& buf);
		// 需要放入指定大小的 buffer，返回放入位置
		char* put(int size);
		void  add(char c);
		void  adv(int size);
		// 需要放入最大 size 大小（未实际放入，可能与 size 不同）
		char* rev(int size);
		// 当前放入位置
		char* current();
		char* data();
		unsigned int size() const;
		int capacity() const;
		bool is_null() const;
		void reset(int size = 0);
		buffer& operator =(buffer&& buf);
		operator smart_str*();
	private:
		smart_str str_;
		friend class string;
	};
}
