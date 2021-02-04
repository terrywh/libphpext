#include "cast.h"
#include "class_closure.h"

namespace php {
    // 创建指定长度的字符串
    value create_sstring(std::size_t size, bool persist) {
        // 新创建（不用增加引用计数）
        return size <= 1 ? value(ZSTR_EMPTY_ALLOC(), false) : value(zend_string_alloc(size, persist), false); 
    }
    // 创建字符串（复制）
    value create_pstring(std::string_view s) {
        // 新创建（不用增加引用计数）
        return value(zend_string_init(s.data(), s.size(), true), true);
    }
    // 创建制定名称的对象实例（未调用 __construct 函数）
    value create_object(std::string_view name) {
        // ??? 类名是否应该使用内部持久型字符串
        zend_string* str = zend_string_init_interned(name.data(), name.size(), 1);
        zend_class_entry* ce = zend_lookup_class(str);
        return value(ce);
    }
    // 构建：可调用函数
    value create_callable(std::string_view cb) {
        return value(cb);
    }
    //
    value create_callable(value obj, std::string_view name) {
        value cb = create_array(2);
        php::array& a = php::cast<php::array>(cb);
        a.append(obj);
        a.append(name);
        return cb;
    }
    // 构造回调
    value create_callable(std::function<value (parameters& params)> cb) {
        value v;
        object_init_ex(v, *class_entry<class_closure>::entry());
        class_entry<class_closure>::native( static_cast<zend_object*>(v) )->set(cb);
        return v;
    }
    // 创建数组
    value create_array(std::size_t size) {
        value v;
        ZVAL_ARR(v, _zend_new_array(size));
        // 空数组初始的引用计数标记为 2 故在进行任何更新型操作时会进行分离复制
        // if(size == 0) return const_cast<zend_array*>(&zend_empty_array);
        return v;
    }
}