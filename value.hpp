#pragma once

namespace php {
	class value {
	protected:
		zval  value_ins;
	public:
		// ---------------------------------------------------------------------
		virtual ~value() {
			zval_ptr_dtor(&value_ins);
		}
		// ---------------------------------------------------------------------
		value() {
			ZVAL_UNDEF(&value_ins);
		}
		value(std::nullptr_t v) {
			ZVAL_NULL(&value_ins);
		}
		value(const zval* v) {
			if(v == nullptr) {
				ZVAL_UNDEF(&value_ins);
			}else{
				ZVAL_COPY(&value_ins, v);
			}
		}
		value(const zend_string* v) {
			ZVAL_STR(&value_ins, const_cast<zend_string*>(v));
			addref();
		}
		value(const zend_object* v) {
			ZVAL_OBJ(&value_ins, const_cast<zend_object*>(v));
			addref();
		}
		value(const zend_array* v) {
			ZVAL_ARR(&value_ins, const_cast<zend_array*>(v));
			addref();
		}
		explicit value(const void* data) {
			ZVAL_PTR(&value_ins, const_cast<void*>(data));
		}
		value(const value& w) {
			ZVAL_COPY(&value_ins, &w.value_ins);
		}
		value(value&& w) {
			ZVAL_COPY_VALUE(&value_ins, &w.value_ins);
			ZVAL_UNDEF(&w.value_ins);	
		}
		// 类型检查构造
		value(const value& w, const TYPE& t); // value_ex.hpp
		value(value&& w, const TYPE& t); // value_ex.hpp
		value(const value& w, const CLASS& ce); // value_ex.hpp
		value(value&& w, const CLASS& ce); // value_ex.hpp
		// 基础类型
		// ---------------------------------------------------------------------
		value(bool v) {
			ZVAL_BOOL(&value_ins, v);
		}
		value(int v) {
			ZVAL_LONG(&value_ins, v);
		}
		value(std::uint32_t v) {
			ZVAL_LONG(&value_ins, v);
		}
		value(std::int64_t v) {
			ZVAL_LONG(&value_ins, v);
		}
		value(std::size_t v) {
			ZVAL_LONG(&value_ins, v);
		}
		value(double v) {
			ZVAL_DOUBLE(&value_ins, v);
		}
		value(const char* str) {
			ZVAL_STRINGL(&value_ins, str, std::strlen(str));
		}
		value(const std::string& str) {
			ZVAL_STRINGL(&value_ins, str.c_str(), str.length());
		}
		// ---------------------------------------------------------------------
		bool empty() const {
			switch(Z_TYPE(value_ins)) {
			case IS_UNDEF:
			case IS_NULL:
			case IS_FALSE:
				return true;
			case IS_LONG:
			case IS_DOUBLE:
				return Z_LVAL(value_ins) == 0;
			case IS_STRING:
				return Z_STRLEN(value_ins) == 0;
			case IS_ARRAY:
				return Z_ARRVAL(value_ins)->nNumOfElements == 0;
			default: // TODO how to determine 'empty' for other types?
				return false;
			}
		}
		std::size_t length() const {
			switch(Z_TYPE(value_ins)) {
				case IS_UNDEF:
				case IS_NULL:
					return 0l;
				case IS_FALSE:
				case IS_TRUE:
					return sizeof(zend_bool);
				case IS_LONG:
					return sizeof(zend_long);
				case IS_DOUBLE:
					return sizeof(double);
				case IS_STRING:
					return Z_STRLEN(value_ins);
				case IS_ARRAY:
					return zend_array_count(Z_ARRVAL(value_ins));
				default: // TODO 其它类型？
					return 0;
			}
		}
		inline std::size_t size() const {
			return length();
		}
		// --------------------------------------------------------------------
		
		TYPE typeof() const {
			return TYPE(&value_ins);
		}
		bool typeof(const TYPE& t) const {
			zend_uchar t_ = Z_TYPE(value_ins);
			return t == t_ // 类型相同
				|| (t == TYPE::BOOLEAN && (t_ == IS_TRUE || t_ == IS_FALSE))
				|| (t == TYPE::CALLABLE && zend_is_callable(const_cast<zval*>(&value_ins), IS_CALLABLE_CHECK_SYNTAX_ONLY, nullptr));
		}
		CLASS classof() const {
			assert(typeof(TYPE::OBJECT));
			return CLASS(Z_OBJCE(value_ins));
		}
		bool instanceof(const CLASS& c) const {
			return typeof(TYPE::OBJECT) && instanceof_function(Z_OBJCE(value_ins), c);
		}
		// ---------------------------------------------------------------------
		operator bool() const {
			return !empty();
		}
		operator int() const {
			assert(typeof(TYPE::INTEGER));
			return Z_LVAL(value_ins);
		}
		operator std::int64_t() const {
			assert(typeof(TYPE::INTEGER));
			return Z_LVAL(value_ins);
		}
		operator std::size_t() const {
			assert(typeof(TYPE::INTEGER));
			return Z_LVAL(value_ins);
		}
		operator float() const {
			assert(typeof(TYPE::FLOAT));
			return Z_DVAL(value_ins);
		}
		operator double() const {
			assert(typeof(TYPE::FLOAT));
			return Z_DVAL(value_ins);
		}
		operator std::string() const {
			assert(typeof(TYPE::STRING));
			return std::string(Z_STRVAL(value_ins), Z_STRLEN(value_ins));
		}
		operator zval*() const {
			return const_cast<zval*>(&value_ins);
		}
		operator zend_string*() const {
			assert(typeof(TYPE::STRING));
			return Z_STR(value_ins);
		}
		operator zend_object*() const {
			assert(typeof(TYPE::OBJECT));
			return Z_OBJ(value_ins);
		}
		operator zend_array*() const {
			assert(typeof(TYPE::ARRAY));
			return Z_ARR(value_ins);
		}
		operator zend_class_entry*() const {
			assert(typeof(TYPE::OBJECT));
			return Z_OBJCE(value_ins);
		}
		template <typename POINTER_TYPE>
		POINTER_TYPE* ptr() const {
			assert(typeof(TYPE::POINTER));
			return reinterpret_cast<POINTER_TYPE*>(Z_PTR(value_ins));
		}
		// 转换
		// ---------------------------------------------------------------------
		bool to_bool() {
			convert_to_boolean(&value_ins);
			return typeof() == TYPE::TRUE;
		}
		long to_long(int base = 10) {
			convert_to_long(&value_ins);
			return Z_LVAL(value_ins);
		}
		double to_double() {
			convert_to_double(&value_ins);
			return Z_DVAL(value_ins);
		}
		std::string to_string() {
			convert_to_string(&value_ins);
			return std::string(Z_STRVAL(value_ins), Z_STRLEN(value_ins));
		}
		// 引用
		// ---------------------------------------------------------------------
		std::uint32_t addref() const {
			if(Z_REFCOUNTED(value_ins)) {
				return ++GC_REFCOUNT(Z_COUNTED(value_ins));
			}
			return 1;
		}
		std::uint32_t delref() {
			if(Z_REFCOUNTED(value_ins)) {
				return --GC_REFCOUNT(Z_COUNTED(value_ins));
			}
			return 1;
		}
		// 赋值
		// -------------------------------------------------------------------
		value& operator =(const value& v) {
			zval_ptr_dtor(&value_ins);
			ZVAL_COPY(&value_ins, &v.value_ins);
			return *this;
		}
		value& operator =(value&& v) {
			zval_ptr_dtor(&value_ins);
			ZVAL_COPY_VALUE(&value_ins, &v.value_ins);
			ZVAL_UNDEF(&v.value_ins);
			return *this;
		}
		// 带类型检查
		void assign(const value& v, const TYPE& t);
		void assign(value&& v, const TYPE& t);
		void assign(const value& v, const CLASS& t);
		void assign(value&& v, const CLASS& t);
		void ptr(void* p) {
			zval_ptr_dtor(&value_ins);
			ZVAL_PTR(&value_ins, p);
		}
		// ---------------------------------------------------------------------
		bool operator ==(const value& v) const {
			return Z_PTR(value_ins) == Z_PTR(v.value_ins);
		}
		bool operator !=(const value& v) const {
			return Z_PTR(value_ins) != Z_PTR(v.value_ins);
		}
		// --------------------------------------------------------------------
	};
}
