#pragma once

namespace php {
	class value;
	class buffer {
	public:
		buffer(int size=231);
		~buffer();
		char* put(int size);

		int size() const;
		int capacity() const;
	private:
		smart_str str_;
		int po_;
		friend class value;
	};
}