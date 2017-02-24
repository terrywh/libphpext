.SUFFIXES:

.PHONY: all types types-clean parts parts-clean test test-clean test-ini test-constant test-function

PHP_CONFIG=php-config

TARGET_LIBRARY=libphpext.a
TARGET_VERSION=0.1.0

TYPES_SOURCE=$(wildcard src/types/*.cpp)
TYPES_OBJECT=$(TYPES_SOURCE:%.cpp=%.o)

PARTS_SOURCE=$(wildcard src/parts/*.cpp)
PARTS_OBJECT=$(PARTS_SOURCE:%.cpp=%.o)

BASES_SOURCE=$(wildcard src/bases/*.cpp)
BASES_OBJECT=$(BASES_SOURCE:%.cpp=%.o)

TEST_EXTENSION=phpext.so

CXXFLAGS=-std=c++14 -fPIC -g -O0
INCLUDE=-I/data/vendor/boost/include -I/data/server/php/include/php/Zend -I/data/server/php/include/php/TSRM -I/data/server/php/include/php
LIBRARY=-L/data/vendor/boost/lib


all: ${TARGET_LIBRARY}
	@echo done.

clean: types-clean parts-clean test-clean

${TARGET_LIBRARY}: ${TYPES_OBJECT} ${PARTS_OBJECT} ${BASES_OBJECT}
	ar rcs $@ $^

types: ${TYPES_OBJECT}

types-clean:
	rm -f ${TYPES_OBJECT}

parts: ${PARTS_OBJECT}

parts-clean:
	rm -f ${PARTS_OBJECT}

%.o: %.cpp
	g++ ${CXXFLAGS} ${INCLUDE} -c $^ ${LIBRARY} -o $@

${TEST_EXTENSION}: test/extension.cpp ${TARGET_LIBRARY}
	g++ ${CXXFLAGS} -shared -DEXTENSION_NAME=\"phpext\" -DEXTENSION_VERSION=\"${TARGET_VERSION}\" ${INCLUDE} test/extension.cpp ${TARGET_LIBRARY} ${LIBRARY} -o phpext.so
	cp -f ${TEST_EXTENSION} `${PHP_CONFIG} --extension-dir`

test: ${TEST_EXTENSION}

test-ini: ${TEST_EXTENSION}
	php test/extension_ini.php || exit 0
test-constant: ${TEST_EXTENSION}
	php test/extension_constant.php || exit 0
test-function: ${TEST_EXTENSION}
	php test/extension_function.php || exit 0

test-clean:
	rm -f ${TEST_EXTENSION}
