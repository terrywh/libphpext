.SUFFIXES:

.PHONY: all test install clean

PHP_PREFIX?=/usr/local/php
PHP_CONFIG=${PHP_PREFIX}/bin/php-config

TARGET_LIBRARY=libphpext.a
TARGET_VERSION=2.0.0

SOURCES=$(shell find ./ -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
DEPENDS=$(SOURCES:%.cpp=%.d) phpext.hpp.d
HEADERX=phpext.hpp.gch

TARGETX=phpext.so

CXX=clang++
CXXFLAGS?= -g -O0
CXXFLAGS+= -std=c++11 -fPIC
LDFLAGS?=
LDFLAGS+=-shared
INCLUDE=$(shell ${PHP_CONFIG} --includes | sed 's/-I/-isystem/g')
LIBRARY=

all:

test: ${TARGETX}

install: ${TARGETX}
	sudo rm -f `${PHP_CONFIG} --extension-dir`/phpext.so
	sudo cp phpext.so `${PHP_CONFIG} --extension-dir`/phpext.so
clean:
	rm -f ${HEADERX} ${TARGETX} ${OBJECTS} ${DEPENDS}
	rm -f `find ./ -name "*.o"` `find ./ -name "*.d"`
-include $(DEPENDS)
${TARGETX}: ${HEADERX} ${OBJECTS}
	${CXX} ${LDFLAGS} ${LDFLAGS} ${LIBRARY} ${OBJECTS} ${LIBRARY} -o $@
${HEADERX}:
	${CXX} -x c++-header ${CXXFLAGS} ${INCLUDE} -MMD -MP -c phpext.hpp -o $@
%.o: %.cpp ${HEADERX}
	${CXX} ${CXXFLAGS} ${CXXFLAGS_EXTRA} ${INCLUDE} -MMD -MP -c $< -o $@

