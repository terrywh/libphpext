.SUFFIXES:

# 依赖
VENDOR_PHP?=/data/server/php-7.2.22

# 项目
SOURCES=$(shell find ./src -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
DEPENDS=$(SOURCES:%.cpp=%.d)
TARGETX=libphpext.a
VERSION=3.0.1
TARGETY=phpext.so

# 编译
CXX=g++
INCLUDES=$(shell ${VENDOR_PHP}/bin/php-config --includes | sed 's/-I/-isystem/g')
CXXFLAGS?= -g -O2
CXXFLAGS+= -std=c++11 -fPIC ${INCLUDES}
LDFLAGS?=
LDFLAGS+=-shared

# 安装
PREFIX?=/data/vendor/phpext-${VERSION}

.PHONY: all test install clean
all: ${TARGETX} ${TARGETY}
var:
	@echo ${OBJECTS}
test: ${TARGETY}
	sudo rm -f `${VENDOR_PHP}/bin/php-config --extension-dir`/${TARGETY}
	sudo cp ${TARGETY} `${VENDOR_PHP}/bin/php-config --extension-dir`/${TARGETY}
install: ${TARGETX}
	rm -rf ${PREFIX}/include/phpext ${PREFIX}/lib/${TARGETX}
	mkdir -p ${PREFIX}/include/phpext
	mkdir -p ${PREFIX}/lib
	cp -f --preserve=timestamps src/*.h ${PREFIX}/include/phpext
	cp -f ${TARGETX} ${PREFIX}/lib
clean:
	rm -f ${HEADERX} ${TARGETX} ${OBJECTS} ${DEPENDS}
	rm -f `find ./ -name "*.o"` `find ./ -name "*.d"`
-include $(DEPENDS)
${TARGETX}: ${OBJECTS}
	ar rcs $@ $^
${TARGETY}: ./test/extension.o ${TARGETX}
	${CXX} -shared $< ${LDFLAGS} -L. -lphpext -static-libstdc++ -o $@
%.o: %.cpp
	echo ${CXXFLAGS}
	${CXX} ${CXXFLAGS} -MMD -MP -DPHPEXT_LIB_VERSION=${VERSION} -c $< -o $@
