SRCS = src/ThingMLMQTT.c 
OBJS = src/ThingMLMQTT.o

STATIC_LIB_LOCATION = libtmlmqtt.a
DYNAMIC_LIB_LOCATION = libtmlmqtt.so

CROSS_COMPILE :=
 
GFLAGS = -pedantic -Wall -Werror -fPIC -g -std=c99 -DDEBUG=1

GCC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar

%.o : %.c
	$(GCC) $(GFLAGS) -c -o $@ $<

all : $(OBJ) staticlib dynamiclib

staticlib : $(OBJS)
	$(AR) crs $(STATIC_LIB_LOCATION) $(OBJS)

dynamiclib : $(OBJS)
	$(GCC) -shared -rdynamic -o $(DYNAMIC_LIB_LOCATION) $(OBJS)

clean :
	rm -rf $(OBJS) $(DYNAMIC_LIB_LOCATION) $(STATIC_LIB_LOCATION)