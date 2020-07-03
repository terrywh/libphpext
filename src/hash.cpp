#include "hash.h"
#include <ext/standard/crc32.h>

namespace php {
    // 
    std::uint32_t crc32(std::string_view data) {
        std::size_t len = data.size();
        const char* src = data.data();
        uint32_t crc32_val = 0;
		for(; --len; ++src) {
			crc32_val = ((crc32_val >> 8) & 0x00FFFFFF) ^ crc32tab[(crc32_val ^ (*src)) & 0xFF ];
		}
		return crc32_val;
    }
}