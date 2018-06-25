#include "vendor.h"
#include "buffer.h"

#include "value.h"
#include "string.h"
#include "util.h"

namespace php {
	void buffer::append(const php::value& v) {
		php::string s = v;
		if(!s.typeof(php::TYPE::STRING)) {
			s = php::json_encode(s);
		}
		if(!s.typeof(php::TYPE::STRING)) {
			s.to_string();
		}
		append(s.c_str(), s.size());
	}
}