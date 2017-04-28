#pragma once

namespace php {
	class extension_entry;
	class class_closure: public class_base {
	public:
		value __invoke(parameters& params);
	private:
		std::function<value (parameters&)> fn_;
		friend class value;
	};

	void _register_builtin_class(extension_entry& extension);
}
