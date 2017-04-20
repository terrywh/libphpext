#pragma once

namespace php {
	class value;
	class buffer {
	public:
		buffer(int size=231);
		~buffer();
		// 需要放入指定大小的 buffer，返回放入位置
		char* put(int size);
		// 需要放入最大 size 大小（未实际放入，可能与 size 不同）
		char* rev(int size);

		int size() const;
		int capacity() const;
	private:
		smart_str str_;
		int po_;
		friend class value;
	};
}
