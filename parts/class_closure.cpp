#include "../phpext.h"

namespace php {
	value class_closure::__invoke(parameters& params) {
		return fn_(params);
	}
}
