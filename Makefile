.SUFFIXES:

.PHONY: all types types-clean parts parts-clean test test-clean test-ini test-constant test-function

PHP_CONFIG=php-config

TARGET_LIBRARY=libphpext.a
TARGET_VERSION=0.1.0

SOURCES=$(wildcard types/*.cpp) $(wildcard parts/*.cpp) $(wildcard bases/*.cpp) $(wildcard funcs/*.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)

TEST_EXTENSION=phpext.so

CXXFLAGS=-std=c++14 -fPIC -g -O0
INCLUDE=`${PHP_CONFIG} --includes`
LIBRARY=/usr/local/gcc6/lib64/libstdc++.a


all: ${TARGET_LIBRARY}
	@echo done.

clean:
	rm -f ${TARGET_LIBRARY} ${OBJECTS}

${TARGET_LIBRARY}: ${OBJECTS}
	ar rcs $@ $^

%.o: %.cpp
	g++ ${CXXFLAGS} ${INCLUDE} -c $^ -o $@

${TEST_EXTENSION}: test/extension.cpp ${TARGET_LIBRARY}
	g++ ${CXXFLAGS} ${INCLUDE} -DEXTENSION_NAME=\"phpext\" -DEXTENSION_VERSION=\"0.1.0\" -c test/extension.cpp -o test/extension.o
	g++ -shared test/extension.o ${TARGET_LIBRARY} ${LIBRARY} -o phpext.so

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
