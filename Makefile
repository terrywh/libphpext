.SUFFIXES:

# 依赖
VENDOR_PHP?=/data/vendor/php-7.0.30

# 项目
SOURCES=$(shell find ./src -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
DEPENDS=$(SOURCES:%.cpp=%.d)
TARGETX=libphpext.a
VERSION=1.0.0
TARGETY=phpext.so

# 编译
CXX=clang++
INCLUDES=$(shell ${VENDOR_PHP}/bin/php-config --includes | sed 's/-I/-isystem/g')
CXXFLAGS?= -g -O0
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
	sudo rm -rf ${PREFIX}/include/phpext ${PREFIX}/lib/${TARGETX}
	sudo mkdir -p ${PREFIX}/include/phpext
	sudo mkdir -p ${PREFIX}/lib
	sudo cp -f src/*.h ${PREFIX}/include/phpext
	sudo cp -f ${TARGETX} ${PREFIX}/lib
clean:
	rm -f ${HEADERX} ${TARGETX} ${OBJECTS} ${DEPENDS}
	rm -f `find ./ -name "*.o"` `find ./ -name "*.d"`
-include $(DEPENDS)
${TARGETX}: ${OBJECTS}
	ar rcs $@ $^
${TARGETY}: ./test/extension.o ${TARGETX}
	${CXX} -shared $< ${LDFLAGS} -L. -lphpext -o $@
%.o: %.cpp
	${CXX} ${CXXFLAGS} -MMD -MP -c $< -o $@

