#include "type.h"

namespace php {
    //
    base_type operator|(const type_code& code, const type_attr& attr) {
        return base_type(code) | attr;
    }
    // 构建 union type
    // 参见 zend_API.c:2457 内部函数暂不支持此种类型注册内部函数
    // type_list operator |(const type_code& code1, const type_code& code2) {
    //     type_list tl;
    //     tl.types_.push_back(base_type{code1});
    //     tl.types_.push_back(base_type{code2});
    //     return tl;
    // }
    // type_list operator |(const user_type& ut1, const user_type& ut2) {
    //     type_list tl;
    //     tl.types_.push_back(ut1);
    //     tl.types_.push_back(ut2);
    //     return tl;
    // }
    // 加入 union type
    // type_list operator |(const type_list& tl1,  const user_type& ut2) {
    //     type_list tl2 {tl1};
    //     tl2.types_.push_back( ut2 );
    //     return tl2;
    // }
}
