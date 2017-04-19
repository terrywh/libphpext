#pragma once

namespace php {
	class class_closure: public class_base {
	public:
		value __invoke(parameters& params);
	private:
		std::function<value (parameters&)> fn_;
		friend class value;
	};
}
