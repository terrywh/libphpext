local version = {major = "4", minor = "0", patch = "0"}
 
version.raw = format("%s.%s.%s", version.major, version.minor, version.patch)
version.ver = format("%s.%s", version.major, version.minor)

set_project("phpext")
set_version(version.major .. "." .. version.minor .. "." .. version.patch)
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
    add_files("src/*.cpp")
    add_defines( format("LIBPHPEXT_VERSION=\"%s\"", version.raw) )
    add_options("vendor-php")
    set_installdir( format("/data/vendor/phpext-%s", version.ver) )
    add_headerfiles("src/*.h", {prefixdir="phpext"})
    add_headerfiles("phpext.h")