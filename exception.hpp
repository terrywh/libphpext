#pragma once

namespace php {
	// exception => zend_ce_throwable
	class exception: public value, public std::exception {
	protected:
		exception() {}
	public:
		// 将 PHP 中发生的异常重新抛出到 CPP 中
		static void maybe_rethrow() {
			if(EG(exception) == nullptr) return;
			exception ex;
			ZVAL_OBJ(&ex.value_ins, EG(exception));
			ex.addref();
			zend_clear_exception();
			throw ex;
		}
		exception(zend_object* v) {
			if(!instanceof_function(v->ce, zend_ce_throwable)) {
				throw exception(zend_ce_type_error, "instance of '" + CLASS(zend_ce_throwable).name() + "' expected, '" + CLASS(v->ce).name() + "' given");
			}
			ZVAL_OBJ(&value_ins, v);
			addref();
		}
		exception(const CLASS& c, const std::string& message, int code = 0);
		exception(const value& v)
		: value(v, zend_ce_throwable) {

		}
		exception(value&& v)
		: value(std::move(v), zend_ce_throwable) {

		}
		// ----------------------------------------------------------------
		exception& operator =(const value& v) {
			assign(v, zend_ce_throwable);
			return *this;
		}
		exception& operator =(value&& v) {
			assign(std::move(v), zend_ce_throwable);
			return *this;
		}
		// ----------------------------------------------------------------
		error_info info() const {
			return error_info(&value_ins);
		}
		virtual const char* what() const noexcept {
			return info().message.c_str();
		}
	};
}
