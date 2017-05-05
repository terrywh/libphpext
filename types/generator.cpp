#include "../phpext.h"

namespace php {
	generator::~generator() {
		zend_object* obj = (zend_object*)gen_;
		if(obj != nullptr && --GC_REFCOUNT(obj) == 0) {
			zend_objects_destroy_object(obj);
		}
	}
	generator::generator(zend_object* obj)
	: gen_((zend_generator*)obj) {
		++GC_REFCOUNT(obj);
	}
	generator::generator(const generator& obj)
	: gen_(obj.gen_) {
		++GC_REFCOUNT((zend_object*)gen_);
	}
	generator::generator(generator&& obj)
	: gen_(obj.gen_) {
		obj.gen_ = nullptr;
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	static void inline zend_generator_ensure_initialized(zend_generator *generator) {
		if (UNEXPECTED(Z_TYPE(generator->value) == IS_UNDEF) && EXPECTED(generator->execute_data) && EXPECTED(generator->node.parent == NULL)) {
			generator->flags |= ZEND_GENERATOR_DO_INIT;
			zend_generator_resume(generator);
			generator->flags &= ~ZEND_GENERATOR_DO_INIT;
			generator->flags |= ZEND_GENERATOR_AT_FIRST_YIELD;
		}
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	static void zend_generator_throw_exception(zend_generator *generator, zval *exception) {
		zend_execute_data *original_execute_data = EG(current_execute_data);

		/* if we don't stop an array/iterator yield from, the exception will only reach the generator after the values were all iterated over */
		if (UNEXPECTED(Z_TYPE(generator->values) != IS_UNDEF)) {
			zval_ptr_dtor(&generator->values);
			ZVAL_UNDEF(&generator->values);
		}

		/* Throw the exception in the context of the generator. Decrementing the opline
		 * to pretend the exception happened during the YIELD opcode. */
		EG(current_execute_data) = generator->execute_data;
		generator->execute_data->opline--;
		if (exception) {
			zend_throw_exception_object(exception);
		} else {
			zend_throw_exception_internal(NULL);
		}
		generator->execute_data->opline++;
		EG(current_execute_data) = original_execute_data;
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	value generator::current() {
		value rv;
		zend_generator_ensure_initialized(gen_);
		zend_generator* root = zend_generator_get_current(gen_);
		if (EXPECTED(gen_->execute_data != NULL && Z_TYPE(root->value) != IS_UNDEF)) {
			zval *value = &root->value;
			ZVAL_DEREF(value);
			ZVAL_COPY((zval*)&rv, value);
		}
		return std::move(rv);
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	void generator::next() {
		zend_generator_ensure_initialized(gen_);
		zend_generator_resume(gen_);
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	void generator::send(const php::value& v) {
		zend_generator_ensure_initialized(gen_);
		/* The generator is already closed, thus can't send anything */
		if (UNEXPECTED(!gen_->execute_data)) {
			return;
		}
		zend_generator* root = zend_generator_get_current(gen_);
		/* Put sent value in the target VAR slot, if it is used */
		if (root->send_target) {
			ZVAL_COPY(root->send_target, (zval*)&v);
		}
		zend_generator_resume(gen_);
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	void generator::throw_exception(const php::value& e) {
		zval exception;
		ZVAL_DUP(&exception, const_cast<zval*>(reinterpret_cast<const zval*>(&e)));
		zend_generator_ensure_initialized(gen_);
		if (gen_->execute_data) {
			zend_generator *root = zend_generator_get_current(gen_);
			zend_generator_throw_exception(root, &exception);
			zend_generator_resume(gen_);
		} else {
			/* If the generator is already closed throw the exception in the
			 * current context */
			zend_throw_exception_object(&exception);
		}
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	void generator::throw_exception(const std::string& msg, int code) {
		php::object e = php::object::create("Exception");
		e.call("__construct", msg, code);
		throw_exception(e);
	}
	// 代码来自 php 源码 zend/zend_generators.c 相关位置
	bool generator::valid() {
		zend_generator_ensure_initialized(gen_);
		zend_generator_get_current(gen_);
		return EXPECTED(gen_->execute_data != NULL);
	}
}
