#pragma once

namespace php {
	class value;
	class generator {
	private:
		zend_generator* gen_;
	public:
		~generator();
		generator(zend_object* obj);
		generator(const generator& obj);
		generator(generator&& obj);

		php::value current();
		void next();
		void send(const php::value& v);
		void throw_exception(const php::value& e);
		void throw_exception(const std::string& msg, int code = 0);
		bool valid();
		friend class value;
	};
}
