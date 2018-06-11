#pragma once

namespace php {
	exception::exception(const CLASS& c, const std::string& message, int code) {
		object e(c, {message, code});
		ZVAL_COPY(&value_ins, e);
	}
}