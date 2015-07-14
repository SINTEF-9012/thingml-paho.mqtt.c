/*
 * publish_subscribe.c
 *
 *  Created on: Jul 14, 2015
 *      Author: vassik
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../ThingMLMQTT.h"
#include "MQTTAsync.h"

void connection_callback(void* _instance, ...) {
	printf("connection callback is called...\n");
}

void connection_fail_callback(void* _instance, ...) {
	printf("connection fail callback is called...\n");
}

void connection_lost_callback(void* _instance, ...) {
	char* reason;
	va_list valist;
	va_start(valist, _instance);

	reason = va_arg(valist, char*);

	va_end(valist);

	printf("connection lost callback is lost, reason %c...\n", *reason);
}

void message_arrived_callback(void* _instance, ...) {
	char* message;
	char* topic;
	va_list valist;
	va_start(valist, _instance);

	topic = va_arg(valist, char*);
	message = va_arg(valist, char*);

	va_end(valist);

	printf("message arrived callback is called: topic '%s' and payload '%s'\n", topic, message);
	free(message);
	free(topic);
}

void subscribe_callback(void* _instance, ...) {
	printf("subscribe callback is called\n");
}

void subscribe_fail_callback(void* _instance, ...) {
	int reason;
	va_list valist;
	va_start(valist, _instance);

	reason = va_arg(valist, int);

	va_end(valist);
	printf("subscribe fail callback is called with the reason %d\n", reason);
}

void message_published_callback(void* _instance, ...) {
	printf("message published callback is called with the reason\n");
}

int main(int argc, char* argv[]) {
	int ch;
	MQTTAsync client = NULL;
	const char* serverURI = "tcp://m2m.eclipse.org:1883";
	const char* clientId = "pub_sub_thingml_client";
	const char* topic = "/sintef/vassik/hello";
	char* message = "hello world!";
	int qos = 1;
	ThingMLMQTTContext* context = malloc(sizeof(ThingMLMQTTContext));

	do {
		ch = getchar();
		switch (ch) {
			case 'n' : {
				printf("Initializing new client...\n");
				MQTTAsync_destroy(&client);
				nullset_thingml_mqtt_context(context);
				create_mqtt_client(&client, serverURI, clientId, context);
			}; break;
			case 'c' : {
				printf("Connecting client...\n");
				context->fn_onconn_callback = connection_callback;
				context->fn_connlost_callback = connection_lost_callback;
				context->fn_onconnfail_callback = connection_fail_callback;
				context->fn_onmsgrcv_callback = message_arrived_callback;

				connect_mqtt_client(context);
			}; break;
			case 's' : {
				printf("Subscribe to the topic '%s'...\n", topic);

				context->fn_onsub_callback = subscribe_callback;
				context->fn_onsubfail_callback = subscribe_fail_callback;

				subscribe_mqtt_client(context, topic, qos);
			}; break;
			case 'p' : {
				printf("Publishing message '%s' on the topic '%s'...\n", message, topic);

				context->fn_onsend_callback = message_published_callback;

				publish_mqtt_client(context, topic, message, qos);
			}; break;
		}
	} while (ch != 'q');

	printf("Goodbye! %p, %p\n", (void*) client, (void*) context);
	MQTTAsync_destroy(&client);
	free(context);
	return 1;
}
