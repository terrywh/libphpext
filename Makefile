.SUFFIXES:

.PHONY: all types types-clean parts parts-clean test test-clean test-ini test-constant test-function

PHP_CONFIG=php-config

TARGET_LIBRARY=libphpext.a
TARGET_VERSION=0.2.0

SOURCES=$(wildcard types/*.cpp) $(wildcard parts/*.cpp) $(wildcard bases/*.cpp) $(wildcard funcs/*.cpp) extension.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)

TEST_EXTENSION=phpext.so


CXX?=/usr/local/gcc-7.1.0/bin/g++
CXXFLAGS?= -g -O0
LDFLAGS?=-Wl,-rpath="/usr/local/gcc-7.1.0/lib64"
LDFLAGS_DEFAULT=-shared -u get_module
CXXFLAGS_DEFAULT=-std=c++11 -fPIC
INCLUDE=`${PHP_CONFIG} --includes`
LIBRARY=


all: ${TARGET_LIBRARY}
	@echo done.

clean:
	rm -f vendor.h.gch ${TARGET_LIBRARY} ${OBJECTS}

${TARGET_LIBRARY}: vendor.h.gch ${OBJECTS}
	ar rcs $@ $^
vendor.h.gch: vendor.h
	${CXX} -x c++ ${CXXFLAGS} ${CXXFLAGS_DEFAULT} ${INCLUDE} -c $^ -o $@
%.o: %.cpp
	${CXX} ${CXXFLAGS} ${CXXFLAGS_DEFAULT} ${INCLUDE} -c $^ -o $@

${TEST_EXTENSION}: test/extension.cpp ${TARGET_LIBRARY}
	${CXX} ${CXXFLAGS} ${CXXFLAGS_DEFAULT} ${INCLUDE} -DEXTENSION_NAME=\"phpext\" -DEXTENSION_VERSION=\"${TARGET_VERSION}\" -c test/extension.cpp -o test/extension.o
	${CXX} ${LDFLAGS} ${LDFLAGS_DEFAULT} ${LIBRARY} test/extension.o ${TARGET_LIBRARY} -o ${TEST_EXTENSION}

test: ${TEST_EXTENSION}

test-install:
	cp -f ${TEST_EXTENSION} `${PHP_CONFIG} --extension-dir`

test-ini: ${TEST_EXTENSION}
	php test/extension_ini.php || exit 0
test-constant: ${TEST_EXTENSION}
	php test/extension_constant.php || exit 0
test-function: ${TEST_EXTENSION}
	php test/extension_function.php || exit 0

test-clean:
	rm -f ${TEST_EXTENSION}
