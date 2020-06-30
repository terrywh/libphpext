CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(libphpext)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC

if(DEFINED ENV{VENDOR_PHP})
    set(VENDOR_PHP $ENV{VENDOR_PHP})
else()
    set(VENDOR_PHP /data/server/php-8.0)
endif()
execute_process(COMMAND ${VENDOR_PHP}/bin/php-config --extension-dir
    OUTPUT_VARIABLE VENDOR_PHP_EXTENSION_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(
    ${VENDOR_PHP}/include/php
    ${VENDOR_PHP}/include/php/main
    ${VENDOR_PHP}/include/php/TSRM
    ${VENDOR_PHP}/include/php/Zend
    ${VENDOR_PHP}/include/php/ext
    ${VENDOR_PHP}/include/php/ext/date/lib
)
link_libraries(-static-libstdc++)

add_library(phpext STATIC
        src/array.cpp
        src/array_iterator.cpp
        src/callable.cpp
        src/callback.cpp
        src/constant_entry.cpp
        src/conversion.cpp
        src/env.cpp
        src/exception.cpp
        src/function_entry.cpp
        src/ini_entry.cpp
        src/module_entry.cpp
        src/object.cpp
        src/property.cpp
        src/property_entry.cpp
        src/string.cpp
        src/string_buffer.cpp
        src/string_builder.cpp)
set_property(TARGET phpext PROPERTY PUBLIC_HEADER src/phpext.h)
set_property(TARGET phpext PROPERTY PRIVATE_HEADER
        src/argument_info.h
        src/array.h
        src/array_iterator.h
        src/callable.h
        src/class_entry.h
        src/callback.h
        src/constant_entry.h
        src/conversion.h
        src/dependence.h
        src/env.h
        src/exception.h
        src/function_entry.h
        src/ini_entry.h
        src/module_entry.h
        src/object.h
        src/parameter.h
        src/property.h
        src/property_entry.h
        src/string.h
        src/string_buffer.h
        src/string_builder.h
        src/type_code.h
        src/value.h
        src/vendor.h)
target_precompile_headers(phpext PUBLIC src/vendor.h)

add_library(example SHARED
    example/example.cpp)
target_link_libraries(example phpext)
set_property(TARGET example PROPERTY PREFIX "")
target_precompile_headers(example PUBLIC src/phpext.h)

install(TARGETS phpext ARCHIVE PUBLIC_HEADER PRIVATE_HEADER)

## 安装示例扩展
add_custom_target(test sudo cp $<TARGET_FILE:example> ${VENDOR_PHP_EXTENSION_DIR}
    BYPRODUCTS ${VENDOR_PHP_EXTENSION_DIR}/example.so
    COMMENT "Installing example.so extension")
add_dependencies(test example)
