.SUFFIXES:

.PHONY: all types

TYPES_SOURCE=$(wildcard src/types/*.cpp)
TYPES_OBJECT=$(TYPES_SOURCE:%.cpp=%.o)

CXXFLAGS=-std=c++14 -g -O0
INCLUDES=-I/data/vendor/boost/include -I/data/server/php/include/php/Zend -I/data/server/php/include/php/TSRM -I/data/server/php/include/php

all:
	echo ${TYPES_SOURCE}
	echo ${TYPES_OBJECT}

types: ${TYPES_OBJECT}
types-clean:
	rm -f ${TYPES_OBJECT}

%.o: %.cpp
	g++ ${CXXFLAGS} ${INCLUDES} -c $^ -o $@
