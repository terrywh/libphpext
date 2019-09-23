#define VERSION_STRING(x) #x

namespace php {
    const char* version() {
        return VERSION_STRING(PHPEXT_LIB_VERSION);
    }
} 