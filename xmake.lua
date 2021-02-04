local version = "4.0.0"
local versions = version:split(".",{plain = true}) -- lua array index start from 1
 
set_project("phpext")
set_version(version)
set_languages("c17","c++17")
add_rules("mode.debug")
add_rules("mode.release")

option("vendor-php")
    set_default("/data/server/php-8.0")
    after_check(function(option)
        local vendor_php_includes, _ = os.iorun("$(vendor-php)/bin/php-config --includes")
        local vendor_php_libs, _ = os.iorun("$(vendor-php)/bin/php-config --libs")
        option:add("cxflags", vendor_php_includes)
        option:add("ldflags", vendor_php_libs)
    end)
    
target("phpext")
    set_kind("static")
    set_pcxxheader("phpext.h")
    add_files("src/*.cpp")
    add_cxflags("-fPIC")
    add_defines( format("LIBPHPEXT_VERSION=\"%s\"", version) )
    add_options("vendor-php")
    set_installdir( format("/data/vendor/phpext-%s.%s", versions[1], versions[2]) )
    add_headerfiles("src/*.h", {prefixdir="phpext"})
    add_headerfiles("phpext.h")

target("example1")
    set_kind("shared")
    add_files("example/example1.cpp")
    add_deps("phpext")
    add_options("vendor-php")
    on_load(function(target)
        local vendor_php_extension_dir, _ = os.iorun("$(vendor-php)/bin/php-config --extension-dir")
        target:set("installdir", vendor_php_extension_dir)
    end)
    set_filename("example1.so")

target("example2")
    set_kind("shared")
    add_files("example/example2.cpp")
    add_deps("phpext")
    add_options("vendor-php")
    on_load(function(target)
        local vendor_php_extension_dir, _ = os.iorun("$(vendor-php)/bin/php-config --extension-dir")
        target:set("installdir", vendor_php_extension_dir)
    end)
    set_filename("example2.so")