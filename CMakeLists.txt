CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(libphpext)
set(POSITION_INDEPENDENT_CODE ON) # -fPIC

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
    src/constant_entry.cpp
    src/conversion.cpp
    src/exception.cpp
    src/function_entry.cpp
    src/module_entry.cpp
    src/object.cpp
    src/string.cpp)
set_property(TARGET phpext PROPERTY PUBLIC_HEADER
    src/extension.h)
set_property(TARGET phpext PROPERTY PRIVATE_HEADER
    src/argument_info.h
    src/class_entry.h
    src/constant_entry.h
    src/constants.h
    src/conversion.h
    src/dependence.h
    src/exception.h
    src/extension.h
    src/forward.h
    src/function_entry.h
    src/ini_entry.h
    src/module_entry.h
    src/object.h
    src/parameter.h
    src/string.h
    src/value.h)

add_library(example SHARED
    example/example.cpp)
add_dependencies(example phpext)
set_property(TARGET example PROPERTY PREFIX "")

install(TARGETS phpext ARCHIVE PUBLIC_HEADER PRIVATE_HEADER)

## 安装示例扩展
add_custom_target(test sudo cp $<TARGET_FILE:example> ${VENDOR_PHP_EXTENSION_DIR}
    BYPRODUCTS ${VENDOR_PHP_EXTENSION_DIR}/example.so
    COMMENT "Install example extension")
add_dependencies(test example)