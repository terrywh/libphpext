#include "../phpext.h"

namespace php {
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	static void zend_generator_ensure_initialized(zend_generator *generator) {
		if (UNEXPECTED(Z_TYPE(generator->value) == IS_UNDEF) && EXPECTED(generator->execute_data) && EXPECTED(generator->node.parent == NULL)) {
			generator->flags |= ZEND_GENERATOR_DO_INIT;
			zend_generator_resume(generator);
			generator->flags &= ~ZEND_GENERATOR_DO_INIT;
			generator->flags |= ZEND_GENERATOR_AT_FIRST_YIELD;
		}
	}
	
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	php::value generator::current() {
		zend_generator* gen_ = reinterpret_cast<zend_generator*>(Z_OBJ(value_));
		zend_generator_ensure_initialized(gen_);
		zend_generator* root = zend_generator_get_current(gen_);
		if (EXPECTED(gen_->execute_data != NULL && Z_TYPE(root->value) != IS_UNDEF)) {
			zval *value = &root->value;
			ZVAL_DEREF(value);
			// ZVAL_COPY(return_value, value);
			return *value;
		}
		return nullptr;
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	void generator::next() {
		zend_generator* gen_ = reinterpret_cast<zend_generator*>(Z_OBJ(value_));
		zend_generator_ensure_initialized(gen_);
		zend_generator_resume(gen_);
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	php::value generator::send(const php::value& v) {
		zend_generator* gen_ = reinterpret_cast<zend_generator*>(Z_OBJ(value_)),
			*root;
		zend_generator_ensure_initialized(gen_);
		/* The generator is already closed, thus can't send anything */
		if (UNEXPECTED(!gen_->execute_data)) {
			return nullptr;
		}
		root = zend_generator_get_current(gen_);
		/* Put sent value in the target VAR slot, if it is used */
		if (root->send_target) {
			ZVAL_COPY(root->send_target, (zval*)&v);
		}
		zend_generator_resume(gen_);
		root = zend_generator_get_current(gen_);
		if (EXPECTED(gen_->execute_data)) {
			zval *val = &root->value;
			ZVAL_DEREF(val);
			// ZVAL_COPY(return_value, value);
			return *val;
		}
		return nullptr;
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	static void zend_generator_throw_exception(zend_generator *generator, zval* exception) {
		zend_execute_data *original_execute_data = EG(current_execute_data);
		/* if we don't stop an array/iterator yield from, the exception will only reach the generator after the values were all iterated over */
		if (UNEXPECTED(Z_TYPE(generator->values) != IS_UNDEF)) {
			zval_ptr_dtor(&generator->values);
			ZVAL_UNDEF(&generator->values);
		}
		EG(current_execute_data) = generator->execute_data;
		generator->execute_data->opline--;
		// 异步流程修正错误发生的位置
		zend_update_property_string(zend_ce_exception, exception, "file", sizeof("file")-1, zend_get_executed_filename());
		zend_update_property_long(zend_ce_exception, exception, "line", sizeof("line")-1, zend_get_executed_lineno());

		zend_throw_exception_object(exception);
		generator->execute_data->opline++;
		EG(current_execute_data) = original_execute_data;
	}
	// 代码流程来自 php 源码 zend/zend_generators.c 相关位置
	php::value generator::throw_exception(const php::value& ex) {
		zend_generator* gen_ = reinterpret_cast<zend_generator*>(Z_OBJ(value_)),
			*root;
		zval exception;
		ZVAL_DUP(&exception, const_cast<zval*>(reinterpret_cast<const zval*>(&ex)));
		zend_generator_ensure_initialized(gen_);
		if (gen_->execute_data) {
			root = zend_generator_get_current(gen_);
			zend_generator_throw_exception(root, &exception);
			zend_generator_resume(gen_);
			root = zend_generator_get_current(gen_);
			if (gen_->execute_data) {
				zval *val = &root->value;
				ZVAL_DEREF(val);
				// ZVAL_COPY(return_value, value);
				return *val;
			}
		} else {
			zend_throw_exception_object(&exception);
		}
		return nullptr;
	}
	
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	bool generator::valid() {
		zend_generator* gen_ = reinterpret_cast<zend_generator*>(Z_OBJ(value_));
		zend_generator_ensure_initialized(gen_);
		zend_generator_get_current(gen_);
		return EXPECTED(gen_->execute_data != NULL);
	}

	php::value generator::get_return() {
		zend_generator *gen_ = reinterpret_cast<zend_generator*>(Z_OBJ(value_));
		zend_generator_ensure_initialized(gen_);
		if (UNEXPECTED(EG(exception))) {
			return nullptr;
		}
		if(Z_ISUNDEF(gen_->retval)) {
			throw php::exception("connot get return value of a generator that hasn't returned", 0);
			// return nullptr;
		}
		return gen_->retval;
	}
}
