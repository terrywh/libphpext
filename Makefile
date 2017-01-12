.SUFFIXES:

.PHONY: all types types-test

TYPES_SOURCE=$(wildcard src/types/*.cpp)
TYPES_OBJECT=$(TYPES_SOURCE:%.cpp=%.o)
TYPES_SOURCE_TEST=$(wildcard test/types_*.cpp)
TYPES_OBJECT_TEST=$(TYPES_SOURCE_TEST:%.cpp=%.unit)

CXXFLAGS=-std=c++14 -g -O0
INCLUDE=-I/data/vendor/boost/include -I/data/server/php/include/php/Zend -I/data/server/php/include/php/TSRM -I/data/server/php/include/php
LIBRARY=
LIBRARY_TEST=-L/data/vendor/boost/lib -lboost_unit_test_framework

all:
	echo ${TYPES_SOURCE}
	echo ${TYPES_OBJECT}

types: ${TYPES_OBJECT}
types-clean:
	rm -f ${TYPES_OBJECT} ${TYPES_OBJECT_TEST}
types-test: ${TYPES_OBJECT_TEST}
# 由于依赖于 php 可执行文件才能运行，这里只能测试代码编译没有问题
	@echo done
%.o: %.cpp
	g++ ${CXXFLAGS} ${INCLUDE} -c $^ ${LIBRARY} -o $@

test/%.unit: test/%.cpp
	g++ ${CXXFLAGS} ${INCLUDE} -c $^ ${LIBRARY} ${LIBRARY_TEST} -o /dev/null
