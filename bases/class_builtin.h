#pragma once

namespace php {
	class extension_entry;
	class class_exception: public class_base {
	public:
		value __construct(parameters& params);
		value getMessage(parameters& params);
		value getCode(parameters& params);
		value getFile(parameters& params);
		value getLine(parameters& params);
		value getTrace(parameters& params);
		value getTraceAsString(parameters& params);
		value getPrevious(parameters& params);
		value __toString(parameters& params);
	};
	class class_closure: public class_base {
	public:
		value __invoke(parameters& params);
	private:
		std::function<value (parameters&)> fn_;
		friend class value;
	};

	void _register_builtin_class(extension_entry& extension);
}
