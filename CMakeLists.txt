CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
project(libphpext)
# ---------------------------------------------------------------------------------------
# 环境
# ---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INSTALL_PREFIX /data/vendor/phpext-4.0)
set(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT TRUE)
## 编译器
set(GCC_TOOLCHAIN /data/server/gcc CACHE PATH "Compiler: gcc compiler prefix")
## CLANG 编译器（指定 GCC 工具链）
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("--gcc-toolchain=${GCC_TOOLCHAIN}")
    add_link_options("--gcc-toolchain=${GCC_TOOLCHAIN}")
endif()

## PHP
set(VENDOR_PHP /data/server/php-8.0 CACHE PATH "[dep] https://www.php.net/")
execute_process(COMMAND ${VENDOR_PHP}/bin/php-config --includes
    COMMAND sed "s/ *-I/;/g"
    OUTPUT_VARIABLE VENDOR_PHP_INC_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${VENDOR_PHP}/bin/php-config --extension-dir
    OUTPUT_VARIABLE VENDOR_PHP_EXT_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(SYSTEM ${VENDOR_PHP_INC_DIR})
## 静态链接 C++ 库，方便部署
link_libraries(-static-libstdc++)
# ---------------------------------------------------------------------------------------
# 核心库
# ---------------------------------------------------------------------------------------
add_library(phpext STATIC
    src/array.cpp
    src/array_iterator.cpp
    src/callable.cpp
    src/callback.cpp
    src/constant_entry.cpp
    src/conversion.cpp
    src/encoding.cpp
    src/env.cpp
    src/exception.cpp
    src/function_entry.cpp
    src/hash.cpp
    src/ini_entry.cpp
    src/member.cpp
    src/module_entry.cpp
    src/object.cpp
    src/parameter.cpp
    src/property.cpp
    src/property_entry.cpp
    src/string.cpp
    src/string_buffer.cpp
    src/string_builder.cpp
    src/util.cpp
    src/value.cpp)
# 头文件
set_property(TARGET phpext PROPERTY PUBLIC_HEADER src/phpext.h)
set_property(TARGET phpext PROPERTY PRIVATE_HEADER
    src/argument_info.h
    src/array.h
    src/array_iterator.h
    src/callable.h
    src/class_basic.h
    src/class_entry.h
    src/callback.h
    src/constant_entry.h
    src/conversion.h
    src/dependence.h
    src/encoding.h
    src/env.h
    src/exception.h
    src/function_entry.h
    src/hash.h
    src/ini_entry.h
    src/member.h
    src/module_entry.h
    src/object.h
    src/parameter.h
    src/property.h
    src/property_entry.h
    src/string.h
    src/string_buffer.h
    src/string_builder.h
    src/type_code.h
    src/util.h
    src/value.h
    src/value_basic.h
    src/vendor.h)
# 库（安装）
install(TARGETS phpext ARCHIVE PUBLIC_HEADER DESTINATION include/phpext PRIVATE_HEADER DESTINATION include/phpext)
# ---------------------------------------------------------------------------------------
# 示例
# ---------------------------------------------------------------------------------------
add_library(example SHARED
    example/example.cpp)
target_include_directories(example SYSTEM
    PUBLIC /data/vendor/boost-1.73/include)
target_link_libraries(example phpext)
set_property(TARGET example PROPERTY PREFIX "")
## 示例（安装）
add_custom_target(test sudo cp $<TARGET_FILE:example> ${VENDOR_PHP_EXT_DIR}
    BYPRODUCTS ${VENDOR_PHP_EXT_DIR}/example.so
    COMMENT "Installing example.so extension (${VENDOR_PHP_EXT_DIR})")
add_dependencies(test example)

# ---------------------------------------------------------------------------------------
# 预编译
# ---------------------------------------------------------------------------------------
if (CMAKE_BUILD_TYPE MATCHES Release) # 似乎 clangd 无法在存在 pch 的情况下正常工作
    target_precompile_headers(phpext PUBLIC src/vendor.h)
    target_precompile_headers(example PUBLIC src/phpext.h)
endif()


