#ifndef LIBPHPEXT_CLASS_ENTRY_H
#define LIBPHPEXT_CLASS_ENTRY_H

#include "vendor.h"
#include "value.h"
#include "object.h"
#include "parameter.h"
#include "exception.h"
#include "attribute_entry.h"
#include "constant_entry.h"
#include "property_entry.h"
#include "method_entry.h"

namespace php {
    template <typename T1, typename T2>
    constexpr int offset_of(T1 T2::* m) {
        T2 x {};
        return size_t(&(x.*m)) - size_t(&x);
    }

    using constant = constant_entry;
    using attribute = attribute_entry;
    using interface_entry = zend_class_entry**;
    using implement = interface_entry;
    // 类定义（父类，用于容器存储）
    class class_entry_basic {
    private:
        // 多态注册过程 -> register_class_entry()
        virtual void do_register(int module) = 0;
        static void do_register(std::vector<class_entry_basic*> entry, int module);
    public:

        // 通用注册函数
        template <class T>
        static void register_class_entry(int module, class_entry_basic* c);
        // 对象方法代理
        template <class T, value (T::*METHOD)(parameters& params)>
        static void method(zend_execute_data* execute_data, zval* return_value);
        // 虚基类析构
        virtual ~class_entry_basic() {}
        friend class module_entry;
    };
    // 类注册定义实现
    template <class T>
    class class_entry: public class_entry_basic {
    private:
        static zend_class_entry*          entry_;
        static zend_object_handlers     handler_;

        zend_string*                      cname_; // 名称
        std::uint32_t                     cflag_; // 标记
        std::vector<zend_class_entry**>   iface_; // 接口
        zend_array*                       cattr_; // 特征
        std::vector<constant_entry>    constant_; // 常量
        std::vector<property_entry>    property_; // 属性
        std::vector<zend_function_entry> method_; // 方法
        // 实际创建的对象形式
        struct class_memory_t {
            T           cpp;
            zend_object obj; // 注意 obj 会超量分配和访问（故须放置在末尾）
        };
        // 同步 C++ 中的成员引用
        static void sync_property_refer(class_memory_t* m) {
            void *i;
            ZEND_HASH_FOREACH_PTR(&m->obj.ce->properties_info, i) {
                zend_property_info* info = reinterpret_cast<zend_property_info*>(i);
                // 注意 _:sync_ 作为特殊标记识别同步型属性，用户注释使用此内容可能出现异常
                if(!info->doc_comment || std::strncmp(info->doc_comment->val, "_:sync_", 7) != 0) continue;
                // 将 CPP 对象中的数据同步过来
                std::size_t offset;
                std::memcpy(&offset, &info->doc_comment->val[8], sizeof(std::size_t));

                property_refer* v = reinterpret_cast<property_refer*>( (char*)&m->cpp + offset );
                zval*   p = OBJ_PROP(&m->obj, info->offset);
                // 参考 object_properties_init_ex 对属性的初始化过程
                // 已声明的属性实际位于 obj.property_table 中，在 obj.properties 数组（哈系表）中存在 INDIRECT 引用
                // 所以，其实际位置在对象生存周期内不会改变
                property_refer_traits::ptr(v, p);
            } ZEND_HASH_FOREACH_END();
        }
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
            sync_property_refer(m); // 同步 C++ 成员
            return &m->obj;
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
            sync_property_refer(m); // 同步 C++ 成员
            return &n->obj;
        }
    public:
        // 用于继承（扩展）实现
        static zend_class_entry** entry() {
            return &entry_;
        }
        // obj -> cpp
        static inline T* native(zend_object* obj) {
            return &reinterpret_cast<class_memory_t*>( reinterpret_cast<char*>(obj) - handler_.offset )->cpp;
        }
        // cpp -> obj
        static inline object* native(const T* cpp) {
            return reinterpret_cast<object*>(&reinterpret_cast<class_memory_t*>( const_cast<T*>(cpp) )->obj);
        }
        class_entry(zend_string* name, std::uint32_t flag)
                : cname_(name)
                , cflag_(flag)
                , cattr_(nullptr) {
            // 标准的处理器
            std::memcpy(&handler_, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

            handler_.offset    = offsetof(class_memory_t, obj);
            handler_.free_obj  = class_entry::free_object;

            // 允许复制的对象
            if constexpr (std::is_copy_constructible<T>::value)
                handler_.clone_obj = class_entry::clone_object;
            else
                handler_.clone_obj = nullptr;
        }
        // 元数据
        // 可使用 attribute(...) 构建
        class_entry& operator -(attribute_entry a) {
            a.finalize(&cattr_);
            return *this;
        }
        
        // 实现接口（注意二级指针，在构建时实际获取 class_entry 的指针指向，否则可能还未初始化）
        // 可使用 implement(...) 构建
        class_entry& operator -(interface_entry pce) {
            iface_.push_back(pce);
            return *this;
        }
        // 类常量
        class_entry& operator-(constant c) {
            constant_.push_back(std::move(c));
            return *this;
        }
        // 类属性
        // 可使用 prop(...) 或 static_prop(...) 构建
        class_entry& operator-(property_entry p) {
            property_.push_back(std::move(p));
            return *this;
        }
        // 声明方法
        // 使用 static_method<...>(...) 或 method<...>(...) 生成
        class_entry& operator -(zend_function_entry f) {
            method_.push_back(f);
            return *this;
        }

        // 执行注册
        void do_register(int module) override {
            return class_entry_basic::register_class_entry<T>(module, this);
        }
        friend class class_entry_basic;
    };
    template <class T>
    zend_object_handlers class_entry<T>::handler_;
    template <class T>
    zend_class_entry*    class_entry<T>::entry_ = nullptr;
    template <class T>
    void class_entry_basic::register_class_entry(int module, class_entry_basic *c) {
        class_entry<T>* e = static_cast<class_entry<T>*>(c);
        // 参考：INIT_CLASS_ENTRY_EX
        zend_class_entry ce, *pce;
        memset(&ce, 0, sizeof(zend_class_entry));
		ce.name = e->cname_;
        ce.info.internal.builtin_functions = e->method_.data();
		ce.info.internal.builtin_functions = function_entry::finalize(e->method_);
        ce.create_object = class_entry<T>::create_object;
        // 注册过程调用 zend_initialize_class_data() 会重置部分设置
        pce = class_entry<T>::entry_ = zend_register_internal_class(&ce);
        pce->ce_flags  |= e->cflag_; // 额外的标记
        pce->attributes = e->cattr_; // 特征描述

        for(auto& f : e->iface_) // 接口实现
            zend_do_implement_interface(pce, *f);
        e->iface_.clear();
        constant_entry::do_register(e->constant_, pce); // 常量注册
        property_entry::do_register(e->property_, pce); // 属性注册
    }

    // 对象方法代理
    template <class T, value (T::*M)(parameters& params)>
    void class_entry_basic::method(zend_execute_data* execute_data, zval* return_value) {
        parameters params(execute_data);
        value& rv = *reinterpret_cast<value*>(return_value);
        try {
            if(execute_data->func->common.required_num_args > params.size()) {
                throw argument_count_error("Expects at least "
                                + std::to_string(execute_data->func->common.required_num_args) + " parameter(s), "
                                + std::to_string(ZEND_NUM_ARGS()) + " given");
            }
            rv = (static_cast<T*>( class_entry<T>::native(Z_OBJ_P(getThis())) )->*M)(params);
        }
        catch (const throwable& e) {
            php_rethrow(e);
            return;
        }
        // 非预期范围的异常继续在 C++ 侧抛出
    }
}

#endif // LIBPHPEXT_CLASS_ENTRY_H
