OTHER_LIBS = -lpthread -lrt
BOOST_LIBS = -DBOOST_ALL_DYN_LINK -lboost_thread -lboost_system -lboost_program_options -lboost_filesystem -lboost_log_setup -lboost_log -lboost_date_time

ALL_LIBS = $(OTHER_LIBS) #$(BOOST_LIBS)

BINARY = test

BINARY_TOOL = /usr/bin/g++
STATIC_TOOL = ar
DYAMIC_TOOL = gcc
CC = $(BINARY_TOOL)
CXX = $(BINARY_TOOL)

CFLAGS = 
CXXFLAGS = -g -Wall -Werror #$(OTHER_LIBS) $(BOOST_LIBS)
LDFLAGS = -g $(ALL_LIBS)

STATIC_FLAGS = -rc
DYAMIC_FLAGS = -fpic -fPIC -o2 $(ALL_LIBS) $(OTHER_LIBS)

CFLAGS +=
CXXFLAGS += -Wno-unused-local-typedefs $(ALL_LIBS)
LDFLAGS +=

SOURCES = $(wildcard *.c) $(wildcard *.cpp)
OBJS := $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.cpp, %.o, $(wildcard *.cpp))
DEPES_C := $(patsubst %.c, %.d, $(wildcard *.c))
DEPES_CPP := $(patsubst %.cpp, %.d, $(wildcard *.cpp))
DEPES := $(DEPES_C) $(DEPES_CPP)

all: binary

binary: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(BINARY)

sinclude $(DEPES)

$(DEPES_C): %.d :%.c
	@echo "create depend for c file"
	gcc -MM $(CFLAGS) $< > $@; \
		sed 's,\($*\)\.o[ :]*,\l.o $@ : ,g' $@ -i;\
		sed '/boost/d' $@ -i;

$(DEPES_CPP): %.d :%.cpp
	@echo "create depend for cpp file"
	gcc -MM $(CFLAGS) $< > $@; \
		sed 's,\($*\)\.o[ :]*,\l.o $@ : ,g' $@ -i;\
		sed '/boost/d' $@ -i;

.PHONY: clean clean_objs clean_bin clean_depes
clean:
	-rm -rf $(OBJS) $(BINARY) $(DEPES_C) $(DEPES_CPP)

clean_objs:
	-rm -rf $(OBJS)

clean_bin:
	-rm -rf $(BINARY)

clean_depes:
	-rm -rf $(DEPES)

