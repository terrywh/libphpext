#include "../phpext.h"

namespace php {
	void generator::rewind() {
		reinterpret_cast<php::object*>(this)->call(std::string("rewind",6));
	}
	bool generator::valid() {
		return reinterpret_cast<php::object*>(this)->call(std::string("valid",5)).to_bool();
	}
	php::value generator::current() {
		return reinterpret_cast<php::object*>(this)->call(std::string("current",7));
	}
	php::value generator::key() {
		return reinterpret_cast<php::object*>(this)->call(std::string("key",3));
	}
	void generator::next() {
		reinterpret_cast<php::object*>(this)->call(std::string("next",4));
	}
	php::value generator::send(const php::value& v) {
		return reinterpret_cast<php::object*>(this)->call(std::string("send",4), {v});
	}
	php::value generator::throw_exception(const std::string& ex, int code) {
		return throw_exception( php::object::create_exception(ex, code) );
	}
	php::value generator::throw_exception(php::value ex) {
		// 异步流程修正错误发生的位置 (注意这里需要使用基类类型)
		zend_execute_data *original_execute_data = EG(current_execute_data);
		zend_generator* g = (zend_generator*)Z_OBJ_P(&value_);
		EG(current_execute_data) = g->execute_data;
		g->execute_data->opline--;
		// 异步流程修正错误发生的位置(注意这里需要使用基类类型)
		if(static_cast<php::object&>(ex).is_instance_of(zend_ce_throwable)) {
			static_cast<php::object&>(ex).prop("file",4) = php::string(zend_get_executed_filename());
			static_cast<php::object&>(ex).prop("line",4) = zend_get_executed_lineno();
		}
		g->execute_data->opline++;
		EG(current_execute_data) = original_execute_data;
		
		return reinterpret_cast<php::object*>(this)->call(std::string("throw",5), {ex});
	}
	php::value generator::get_return() {
		return reinterpret_cast<php::object*>(this)->call(std::string("getReturn",9));
	}
}
