#pragma once

namespace php {
	class value;
	class extension_entry;
	class parameters;
	class class_closure: public class_base {
	public:
		value __invoke(parameters& params);
	private:
		std::function<value (parameters&)> fn_;
		friend class value;
		friend class callable;
	};

	void _register_builtin_class(extension_entry& extension);
}
