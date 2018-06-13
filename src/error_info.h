#pragma once

namespace php {
	class error_info {
		class suppressor {
		public:
			suppressor(zval* err);
			~suppressor();
		private:
			zval* error_;
			zval  prev_;
		};
	public:
		std::string message;
		std::string file;
		int         line;
		int         code;

		std::string type;
		
		// 用于获取 PHP 全局的错误信息
		error_info();
		error_info(const zval* err);
	};
}