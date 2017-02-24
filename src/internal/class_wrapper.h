namespace phpext {
	namespace internal {
		template <class T>
		class class_object {
		public:
			T*          cpp;
			zend_object php;
			inline static class_object<T>* from_object(zend_object* object) {
				return reinterpret_cast<class_object<T>*>( ((char*)object) - offsetof(class_object<T>, php) );
			}
		};

#define PHP_CORE_DOCUMENT_COMMENT_PREFIX "php-core"

		template <class T>
		class class_wrapper {
		public:
			class_wrapper(zend_class_entry* entry)
			:class_entry(entry) {
				std::memcpy(&object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
				// @zend_class_entry => Zend/zend.h:131
				class_entry->create_object = object_create;
				object_handlers.offset   = sizeof(std::nullptr_t);
				object_handlers.free_obj = object_free;
				// 这里利用 doc_comment ( zend_class_entry 唯一不会被设置覆盖的字段) 将上下问指针放入
				// 由于系统会访问 doc_comment ，故必须为合法的 zend_string 结构
				entry->info.user.doc_comment = zend_string_alloc(sizeof(PHP_CORE_DOCUMENT_COMMENT_PREFIX) + 1 + sizeof(this), true);
				zend_sprintf(entry->info.user.doc_comment->val, "%s\0", PHP_CORE_DOCUMENT_COMMENT_PREFIX);
				class_wrapper<T>* self = this;
				std::memcpy(entry->info.user.doc_comment->val + sizeof(PHP_CORE_DOCUMENT_COMMENT_PREFIX) + 1, reinterpret_cast<char*>(&self), sizeof(this));
			}
			// @zend_class_entry => Zend/zend.h:131
			zend_class_entry*    class_entry;
			// @zend_object_handlers => Zend/zend_object_handlers.h:124
			zend_object_handlers object_handlers;
		private:
			static inline class_wrapper* from_class_entry(zend_class_entry* entry) {
				return *reinterpret_cast<class_wrapper**>(entry->info.user.doc_comment->val + sizeof(PHP_CORE_DOCUMENT_COMMENT_PREFIX) + 1);
			}
			static zend_object* object_create(zend_class_entry* entry) {
				class_wrapper* wrapper = from_class_entry(entry);
				if(wrapper == nullptr) {
					zend_error(E_ERROR, "'php-core' cannot create object of type '%.*s'", entry->name->len, entry->name->val);
					return nullptr;
				}
				class_object<T>* co = (class_object<T>*)emalloc(sizeof(class_object<T>) + sizeof(T) + zend_object_properties_size(entry));
				co->cpp = (T*)((char*)co + sizeof(class_object<T>));
				new (&co->cpp)T; // 调用构造函数
				zend_object_std_init(&co->php, entry);
				object_properties_init(&co->php, entry);
				co->php.handlers = &wrapper->object_handlers;
				// C++ 端对 PHP 对象的引用
				co->cpp->_internal_set(&co->php);
				return &co->php;
			}
			static void object_free(zend_object *object) {
				class_object<T>* co = class_object<T>::from_object(object);
				T* t = co->cpp;
				t->~T(); // 调用析构函数
				efree(co);
			}
		};
	}
}