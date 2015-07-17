SRCS = src/ThingMLMQTT.c 
OBJS = src/ThingMLMQTT.o

EXAMPLE_SRC = src/examples/publish_subscribe.c
EXAMPLE_OBJ = src/examples/publish_subscribe.o
EXAMPLE_BIN = src/examples/publish_subscribe

STATIC_LIB_LOCATION = libtmlmqtt.a
DYNAMIC_LIB_LOCATION = libtmlmqtt.so

PAHO_MQTT_LIB_A = -lpaho-mqtt3a

INSTALL_LIB_DIR = /usr/local/lib
INSTALL_INCLUDE_DIR = /usr/local/include
THINGML_PAHO_DIR = thingmlmqtt

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

install: staticlib dynamiclib
	install -d $(INSTALL_INCLUDE_DIR)/$(THINGML_PAHO_DIR)
	install $(STATIC_LIB_LOCATION) $(INSTALL_LIB_DIR)
	install $(DYNAMIC_LIB_LOCATION) $(INSTALL_LIB_DIR)
	cp -r ./src/*.h $(INSTALL_INCLUDE_DIR)/$(THINGML_PAHO_DIR)
	ldconfig

uninstall:
	rm -rf $(INSTALL_INCLUDE_DIR)/$(THINGML_PAHO_DIR)
	rm -rf $(INSTALL_LIB_DIR)/$(STATIC_LIB_LOCATION)
	rm -rf $(INSTALL_LIB_DIR)/$(DYNAMIC_LIB_LOCATION)

examples : $(EXAMPLE_OBJ)
	$(GCC) -o $(EXAMPLE_BIN) $(GFLAGS) -g $(EXAMPLE_OBJ) $(PAHO_MQTT_LIB_A) $(STATIC_LIB_LOCATION)

clean :
	rm -rf $(OBJS) $(EXAMPLE_OBJ) $(EXAMPLE_BIN) $(DYNAMIC_LIB_LOCATION) $(STATIC_LIB_LOCATION)