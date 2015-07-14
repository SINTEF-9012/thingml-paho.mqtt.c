/*
 * publish_subscribe.c
 *
 *  Created on: Jul 14, 2015
 *      Author: vassik
 */

#include <stdio.h>
#include <stdlib.h>

#include "../ThingMLMQTT.h"
#include "MQTTAsync.h"

void connection_callback(void* _instance, ...) {
	printf("connection callback is called...\n");
}

void connection_fail_callback(void* _instance, ...) {
	printf("connection fail callback is called...\n");
}


int main(int argc, char* argv[]) {
	int ch;
	MQTTAsync client;
	const char* serverURI = "tcp://m2m.eclipse.org:1883";
	const char* clientId = "pub_sub_thingml_client";
	//const char* topic = "/sintef/vassik/hello";
	//char* date = "hello world!";
	ThingMLMQTTContext* context = malloc(sizeof(ThingMLMQTTContext));

	do {
		ch = getchar();
		switch (ch) {
			case 'n' : {
				printf("Initializing new client...\n");
				create_mqtt_client(client, serverURI, clientId, context);
			}; break;
			case 'c' : {
				printf("Connecting client...\n");

			}; break;
		}
	} while (ch != 'q');

	printf("Goodbye!\n");

	MQTTAsync_destroy(&client);
	free(context);
	return 1;
}
