/*
 * ThingMLMQTT.c
 *
 *  Created on: Jul 13, 2015
 *      Author: vassik
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThingMLMQTT.h"



void connlost(void *context, char *cause) {
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);

	ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	if(thing_context->fn_connlost_callback != NULL) {
		cause = (cause != NULL) ? cause : "unknown";
		thing_context->fn_connlost_callback(thing_context->thing_instance, cause);
	}
}


void onDisconnect(void* context, MQTTAsync_successData* response) {
	printf("Successful disconnection\n");

	ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	if(thing_context->fn_ondisconnet_callback != NULL)
			thing_context->fn_ondisconnet_callback(thing_context->thing_instance);
}


void onSend(void* context, MQTTAsync_successData* response) {
	printf("Message with token value %d delivery confirmed\n", response->token);

	ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	if(thing_context->fn_onsend_callback != NULL)
		thing_context->fn_onsend_callback(thing_context->thing_instance, response->token);
}


void onConnectFailure(void* context, MQTTAsync_failureData* response) {
	printf("Connect failed, rc %d\n", response ? response->code : 0);

	ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	if(thing_context->fn_onconnfail_callback != NULL)
		thing_context->fn_onconnfail_callback(thing_context->thing_instance, response ? response->code : 0);
}


void onConnect(void* context, MQTTAsync_successData* response) {
	printf("Successful connection\n");

	ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	if(thing_context->fn_onconn_callback != NULL)
		thing_context->fn_onconn_callback(thing_context->thing_instance);
}


int onMessageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message) {
	 printf("Message arrived\n");

	 ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	 if(thing_context->fn_onmsgrcv_callback == NULL)
		 return 1;

	 int message_length = message->payloadlen;
	 char* payload = malloc(message_length + 1);
	 char* payloadptr = message->payload;
	 for(int i=0; i<message->payloadlen; i++)
		 payload[i] = (*payloadptr++);
	 payload[message_length] = '\0';

	 int topicName_length = strlen(topicName);
	 char* topic = malloc(topicName_length + 1);
	 for(int i=0; i<topicName_length; i++)
		 topic[i] = (*topicName++);
	 topic[topicName_length] = '\0';

	 MQTTAsync_freeMessage(&message);
	 MQTTAsync_free(topicName);

	 thing_context->fn_onmsgrcv_callback(thing_context->thing_instance, topic, payload);
	 return 1;
}


void onSubscribe(void* context, MQTTAsync_successData* response) {
	printf("Subscribe succeeded\n");

	ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	if(thing_context->fn_onsub_callback != NULL)
		thing_context->fn_onsub_callback(thing_context->thing_instance);
}


void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Subscribe failed, rc %d\n", response ? response->code : 0);

	ThingMLMQTTContext* thing_context = (ThingMLMQTTContext*) context;

	if(thing_context->fn_onsubfail_callback != NULL)
		thing_context->fn_onsubfail_callback(thing_context->thing_instance, response ? response->code : 0);
}

void nullset_thingml_mqtt_context(ThingMLMQTTContext* context) {
	context->paho_client = NULL;
	context->client_id = NULL;
	context->fn_connlost_callback = NULL;
	context->fn_onconn_callback = NULL;
	context->fn_onconnfail_callback = NULL;
	context->fn_ondisconnet_callback = NULL;
	context->fn_onmsgrcv_callback = NULL;
	context->fn_onsend_callback = NULL;
	context->fn_onsub_callback = NULL;
	context->fn_onsubfail_callback = NULL;
	context->thing_instance = NULL;
}

void create_mqtt_client(MQTTAsync* client, const char* serverURI, const char* clientId, ThingMLMQTTContext* context) {
	MQTTAsync_create(client, serverURI, clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	context->paho_client = *client;
	context->client_id = clientId;
}


void connect_mqtt_client(ThingMLMQTTContext* context) {
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = context;

	MQTTAsync_setCallbacks(context->paho_client, context, connlost, onMessageArrived, NULL);

	int rc;
	if ((rc = MQTTAsync_connect(context->paho_client, &conn_opts)) != MQTTASYNC_SUCCESS)
		printf("Failed to start connect, return code %d\n", rc);
}


void disconnect_mqtt_client(ThingMLMQTTContext* context) {
	int rc;
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	disc_opts.onSuccess = onDisconnect;
	if ((rc = MQTTAsync_disconnect(context->paho_client, &disc_opts)) != MQTTASYNC_SUCCESS)
		printf("Failed to start disconnect, return code %d\n", rc);
}


void subscribe_mqtt_client(ThingMLMQTTContext* context, const char* topic, int qos) {
	int rc;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = context;

	if ((rc = MQTTAsync_subscribe(context->paho_client, topic, qos, &opts)) != MQTTASYNC_SUCCESS)
		printf("Failed to start subscribe, return code %d\n", rc);
}


void publish_mqtt_client(ThingMLMQTTContext* context, const char* topic, char* message, int qos) {
	int rc;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;

	opts.onSuccess = onSend;
	opts.context = context;
	pubmsg.payload = message;
	pubmsg.payloadlen = strlen(message);
	pubmsg.qos = qos;
	pubmsg.retained = 0;

	if ((rc = MQTTAsync_sendMessage(context->paho_client, topic, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
		printf("Failed to start sendMessage, return code %d\n", rc);
}
