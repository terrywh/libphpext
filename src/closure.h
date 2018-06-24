#pragma once

#include "class_base.h"
#include "value.h"
#include "parameters.h"

namespace php {
	class closure: public class_base {
	public:
		value __invoke(parameters& params) {
			return fn_(params);
		}
	private:
		std::function<value (parameters&)> fn_;
		friend class value;
	};
}
