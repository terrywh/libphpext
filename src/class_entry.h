#ifndef LIBPHPEXT_CLASS_ENTRY_H
#define LIBPHPEXT_CLASS_ENTRY_H

#include "string.h"
#include "constant_entry.h"
#include "argument_info.h"
#include "parameter.h"
#include "exception.h"
// #include "property_entry.h"
#include "function_entry.h"

namespace php {
    // 类定义（父类，用于容器存储）
    class class_entry_basic {
    private:
        // 多态注册过程 -> register_class_entry()
        virtual void do_register() = 0;
    public:

        // 通用注册函数
        template <class T>
        static void register_class_entry(class_entry_basic* c);
        // 对象方法代理
        template <class T, value (T::*METHOD)(parameters& params)>
        static void method(zend_execute_data* execute_data, zval* return_value);
        // 虚基类析构
        virtual ~class_entry_basic() {}

        friend class module_entry;
    };
    // 方法可见性定义
    enum class method_visibility {
        PUBLIC  = ZEND_ACC_PUBLIC,
        PRIVATE = ZEND_ACC_PRIVATE,
        PROTECTED = ZEND_ACC_PROTECTED,
    };
    template <class T>
    class class_entry: public class_entry_basic {
    private:
        static zend_class_entry*         entry_;
        static zend_object_handlers    handler_;

//        std::vector<property_entry>      property_;
        function_entries method_;

        zend_string*                     cname_;
        std::vector<zend_class_entry**>  iface_;
        std::vector<constant_entry>   constant_;
        // 实际创建的对象形式
        struct class_memory_t {
            T           cpp;
            zend_object obj; // 注意 obj 会超量分配和访问（故须放置在末尾）
        };
        // handler:
        static zend_object* create_object(zend_class_entry *entry) {
            assert(entry == entry_); // 构造函数访问错误
            // 内存分配
            class_memory_t* m = reinterpret_cast<class_memory_t*>(ecalloc(1,
                    sizeof(class_memory_t) + zend_object_properties_size(entry_)));
            // PHP 对象初始化
            zend_object_std_init(&m->obj, entry_);
            object_properties_init(&m->obj, entry_);
            m->obj.handlers = &handler_;
            // CPP 对象初始化
            (new (&m->cpp) T());
            return &m->obj;
        }
        // obj -> cpp
        static inline T* native(zend_object* obj) {
            return &reinterpret_cast<class_memory_t*>( reinterpret_cast<char*>(obj) - handler_.offset )->cpp;
        }
        // handler:
        static void free_object(zend_object* obj) {
            // std::printf("destory_object: %s %08x\n", obj->ce->name->val, obj);
            class_memory_t* m = reinterpret_cast<class_memory_t*>( reinterpret_cast<char*>(obj) - handler_.offset );
            m->cpp.~T(); // CPP 对象析构函数调用
            zend_object_std_dtor(obj); // PHP 对象销毁
            // 内存由 Zend 引擎自动释放
        }
        // handler:
        static zend_object* clone_object(zend_object *obj) {
            // 参见：zend_objects_clone_obj()
            // 内存分配
            class_memory_t* n = reinterpret_cast<class_memory_t*>(ecalloc(1,
                      sizeof(class_memory_t) + zend_object_properties_size(entry_)));
            zend_object_std_init(&n->obj, entry_);
            // 代替 object_properties_init(&m->obj, entry_); 初始化未 UNDEF 即可
            if (n->obj.ce->default_properties_count) {
                zval *p = n->obj.properties_table;
                zval *end = p + n->obj.ce->default_properties_count;
                do {
                    ZVAL_UNDEF(p);
                    p++;
                } while (p != end);
            }
            // 赋值属性值
            zend_objects_clone_members(&n->obj, obj);
            // 复制 C++ 对象（使用拷贝构造）
            class_memory_t* m = reinterpret_cast<class_memory_t*>( reinterpret_cast<char*>(obj) - handler_.offset );
            new (&m->cpp) T(m->cpp);

            return &n->obj;
        }
    public:
        using CLASS_TYPE = T;
        // 用于继承（扩展）实现
        static zend_class_entry* entry() {
            return entry_;
        }
        class_entry(zend_string* name)
                : cname_(name) {
            // 标准的处理器
            std::memcpy(&handler_, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

            handler_.offset    = XtOffsetOf(class_memory_t, obj);
            handler_.free_obj  = class_entry::free_object;
            // 允许复制的对象
            if constexpr (std::is_copy_constructible<T>::value)
                handler_.clone_obj = class_entry::clone_object;
            else
                handler_.clone_obj = nullptr;
            // TODO
            // handler_.read_property
            // handler_.write_property

            entry_ = nullptr;
        }
        // 移动构造（用于放入容器的过程）
        class_entry(class_entry&& entry) = default;

        // 实现接口（注意二级指针，在构建时实际获取 class_entry 的指针指向，否则可能还未初始化）
        class_entry& implements(zend_class_entry** pce) {
            iface_.push_back(pce);
            return *this;
        }
        // 定义常量
        class_entry& define(std::string_view name, const php::value& data) {
            constant_.push_back({name, data});
            return *this;
        }
//        class_entry& property(property_entry&& entry) {
//            entry_properties.emplace_back(std::move(entry));
//            return *this;
//        }
        // 声明方法
        template <value (T::*METHOD)(parameters& params) >
        class_entry& declare(std::string_view name, std::initializer_list<argument_info> pi,
                             return_info&& ri, refer* name_ref = nullptr) {
            zend_string* zn = zend_string_init_interned(name.data(), name.size(), true);
            if(name_ref) *name_ref = zn; // 函数名称字符串的引用
            method_ += function_entry(class_entry_basic::method<T, METHOD>, zn, std::move(ri), std::move(pi));
            return *this;
        }
        // 声明方法
        template <value (T::*METHOD)(parameters& params) >
        class_entry& declare(std::string_view name, std::initializer_list<argument_info> pi, refer* name_ref = nullptr) {
            declare(name, std::move(pi), {}, name_ref);
        }
        // 声明方法
        template <value (T::*METHOD)(parameters& params) >
        class_entry& declare(std::string_view name, refer* name_ref = nullptr) {
            declare(name, {}, {}, name_ref);
        }
        void do_register() override {

            return class_entry_basic::register_class_entry<T>(this);
        }
        friend class class_entry_basic;
    };
    template <class T>
    zend_object_handlers class_entry<T>::handler_;
    template <class T>
    zend_class_entry*    class_entry<T>::entry_;
    template <class T>
    void class_entry_basic::register_class_entry(class_entry_basic *c) {
        class_entry<T>* e = static_cast<class_entry<T>*>(c);
        // 参考：INIT_CLASS_ENTRY_EX
        zend_class_entry ce;
        memset(&ce, 0, sizeof(zend_class_entry));
		ce.name = e->cname_;
		ce.info.internal.builtin_functions = e->method_;
        // 注册类
        ce.create_object = class_entry<T>::create_object;
        class_entry<T>::entry_ = zend_register_internal_class_ex(&ce, nullptr);

        for(auto& f : e->iface_) // 接口实现
            zend_do_implement_interface(class_entry<T>::entry_, *f);
        e->iface_.clear();
        for(auto& c : e->constant_) // 常量注册
            zend_declare_class_constant_ex(class_entry<T>::entry_, c.name, &c.value, ZEND_ACC_PUBLIC, c.comment);
        e->constant_.clear();
    }
    // 对象方法代理
    template <class T, value (T::*FUNCTION)(parameters& params)>
    void class_entry_basic::method(zend_execute_data* execute_data, zval* return_value) {
        parameters params(execute_data);
        value& rv = *reinterpret_cast<value*>(return_value);
        try {
            if(execute_data->func->common.required_num_args > params.size()) {
                throw argument_count_error("Expects at least "
                                + std::to_string(execute_data->func->common.required_num_args) + " parameter(s), "
                                + std::to_string(ZEND_NUM_ARGS()) + " given");
            }
            rv = ( static_cast<T*>(class_entry<T>::native( Z_OBJ_P(getThis()) ))->*FUNCTION )(params);
        }
        catch (const throwable& e) {
            rethrow(e);
            return;
        }
        // 非预期范围的异常继续在 C++ 侧抛出
    }
}

#endif // LIBPHPEXT_CLASS_ENTRY_H
