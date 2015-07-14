SRCS = src/ThingMLMQTT.c 
OBJS = src/ThingMLMQTT.o

EXAMPLE_SRC = src/examples/publish_subscribe.c
EXAMPLE_OBJ = src/examples/publish_subscribe.o
EXAMPLE_BIN = src/examples/publish_subscribe

STATIC_LIB_LOCATION = libtmlmqtt.a
DYNAMIC_LIB_LOCATION = libtmlmqtt.so

CROSS_COMPILE :=
 
GFLAGS = -pedantic -Wall -Werror -fPIC -g -std=c99 -DDEBUG=1

GCC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar

%.o : %.c
	$(GCC) $(GFLAGS) -c -o $@ $<

all : staticlib dynamiclib examples

staticlib : $(OBJS)
	$(AR) crs $(STATIC_LIB_LOCATION) $(OBJS)

dynamiclib : $(OBJS)
	$(GCC) -shared -rdynamic -o $(DYNAMIC_LIB_LOCATION) $(OBJS)

examples : $(EXAMPLE_OBJ)
	$(GCC) -o $(EXAMPLE_BIN) $(GFLAGS) -g $(EXAMPLE_OBJ) $(STATIC_LIB_LOCATION)

clean :
	rm -rf $(OBJS) $(EXAMPLE_OBJ) $(EXAMPLE_BIN) $(DYNAMIC_LIB_LOCATION) $(STATIC_LIB_LOCATION)