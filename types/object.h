#pragma once

namespace php {
	class value;
	class object {
	private:
		zend_object* obj_;
		// !!! 仅允许调用类成员函数
		static value __call(zend_object* obj, const char* name, std::size_t len, int argc, zval argv[], bool silent);
		object(zend_class_entry* ce);
	public:
		~object();
		void reset();
		object(): obj_(nullptr) {}
		object(zend_object* obj, bool create = false);
		static object clone(const object& obj);
		object(const object& obj);
		object(object&& obj);
		// 构造
		template<class T>
		static object create() {
			return object(class_entry<T>::create_object(), true);
		}
		static object create(const std::string& name);
		value& prop(const char* name, std::size_t len);
		inline value& prop(const std::string& name) {
			return prop(name.c_str(), name.length());
		}
		// !!! 仅允许调用类成员函数
		inline value call(const std::string& name) {
			return __call(obj_, name.c_str(), name.length(), 0, nullptr, false);
		}
		inline value scall(const std::string& name, bool silent = false) {
			return __call(obj_, name.c_str(), name.length(), 0, nullptr, true);
		}
		template <typename ...Args>
		inline value call(const std::string& name, const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __call(obj_, name.c_str(), name.length(), sizeof...(Args), (zval*)params, false);
		}
		template <typename ...Args>
		inline value scall(const std::string& name, const Args&... argv) {
			value params[] = { static_cast<value>(argv)... };
			return __call(obj_, name.c_str(), name.length(), sizeof...(Args), (zval*)params, true);
		}
		bool is_instance_of(const std::string& class_name) const;
		template<class T>
		inline bool is_instance_of() const {
			return obj_->handlers == &class_entry<T>::handlers_;
		}
		template <class T>
		inline T* native() {
			return class_wrapper<T>::from_obj(obj_);
		};
		inline zend_object* data() {
			return obj_;
		}
		inline std::uint32_t addref() {
			return ++GC_REFCOUNT(obj_);
		}
		inline std::uint32_t delref() {
			return --GC_REFCOUNT(obj_);
		}
		object& operator=(const object& cb);
		object& operator=(object&& cb);
		friend class value;
	};
}
