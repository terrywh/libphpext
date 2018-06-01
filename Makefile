.SUFFIXES:

.PHONY: all types types-clean parts parts-clean test test-clean test-ini test-constant test-function

PHP_PREFIX?=/data/server/php-7.0.30
PHP_CONFIG=${PHP_PREFIX}/bin/php-config

TARGET_LIBRARY=libphpext.a
TARGET_VERSION=2.0.0

SOURCES=$(wildcard types/*.cpp) $(wildcard parts/*.cpp) $(wildcard bases/*.cpp) $(wildcard funcs/*.cpp) extension.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
DEPENDS=$(SOURCES:%.cpp=%.d)

TEST_EXTENSION=phpext.so


CXX?=/usr/local/gcc/bin/g++
CXXFLAGS?= -O2
LDFLAGS?=-Wl,-rpath="/usr/local/gcc/lib64"
LDFLAGS_EXTRA=-shared -u get_module
CXXFLAGS_EXTRA=-std=c++11 -fPIC
INCLUDE=$(shell ${PHP_CONFIG} --includes | sed 's/-I/-isystem/g')
LIBRARY=


all: ${TARGET_LIBRARY}
	@echo done.

${TARGET_LIBRARY}: vendor.h.gch ${OBJECTS}
	ar rcs $@ $^
vendor.h.gch: vendor.h
	${CXX} -x c++ ${CXXFLAGS} ${CXXFLAGS_EXTRA} ${INCLUDE} -c $^ -o $@
%.o: %.cpp vendor.h.gch
	${CXX} ${CXXFLAGS} ${CXXFLAGS_EXTRA} ${INCLUDE} -MMD -MP -c $< -o $@
-include $(DEPENDS)
${TEST_EXTENSION}: test/extension.cpp ${TARGET_LIBRARY}
	${CXX} ${CXXFLAGS} ${CXXFLAGS_EXTRA} ${INCLUDE} -DEXTENSION_NAME=\"phpext\" -DEXTENSION_VERSION=\"${TARGET_VERSION}\" -c test/extension.cpp -o test/extension.o
	${CXX} ${LDFLAGS} ${LDFLAGS_EXTRA} ${LIBRARY} test/extension.o ${TARGET_LIBRARY} -o ${TEST_EXTENSION}

clean:
	rm -f vendor.h.gch ${TARGET_LIBRARY} ${OBJECTS} ${DEPENDS}
test: CXXFLAGS = -O0 -g
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
