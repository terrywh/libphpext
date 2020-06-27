#ifndef LIBPHPEXT_THUNK_H
#define LIBPHPEXT_THUNK_H

#include "value.h"
#include "parameter.h"

namespace php {
    // 提供 C++ 的代理函数（转换参数形式）
    class delegate {
    public:
        
        // // 成员方法
        // template <class T, value (T::*FUNCTION)(parameters& params)>
        // static void method_delegate(zend_execute_data* execute_data, zval* return_value) {
        //     parameters params(execute_data);
        //     php::value rv;
        //     try {
        //         if(execute_data->func->common.required_num_args > params.size()) {
        //             throw exception(zend_ce_type_error, "expects at least " + std::to_string(execute_data->func->common.required_num_args) + " parameters, " + std::to_string(ZEND_NUM_ARGS()) + " given");
        //         }
        //         rv = (static_cast<T*>(native( Z_OBJ_P(getThis()) ))->*FUNCTION)(params);
        //     }catch (const exception& e) {
        //         exception::rethrow(e);
        //         return;
        //     }/*catch (const std::exception& e) {
        //         zend_throw_exception(zend_ce_error, e.what(), 0);
        //         return;
        //     } catch(...) {
        //         // 非可控范围的异常继续抛出
        //     }*/
        //     ZVAL_COPY(return_value, rv);
        // }

    }
}

#endif // LIBPHPEXT_THUNK_H
