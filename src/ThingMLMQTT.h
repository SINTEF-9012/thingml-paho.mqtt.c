/*
 * ThingMLMQQT.h
 *
 *  Created on: Jul 13, 2015
 *      Author: vassik
 */

#ifndef SRC_THINGMLMQTT_H_
#define SRC_THINGMLMQTT_H_

#include "MQTTAsync.h"
#include "ThingMLMQTTUtility.h"

void create_mqtt_client(MQTTAsync client, const char* serverURI, const char* clientId, ThingMLMQTTContext* context);
void connect_mqtt_client(ThingMLMQTTContext* context);
void disconnect_mqtt_client(ThingMLMQTTContext* context);
void subscribe_mqtt_client(ThingMLMQTTContext* context, const char* topic, int qos);
void publish_mqtt_client(ThingMLMQTTContext* context, const char* topic, char* message, int qos);

/*callbacks*/
void connlost(void *context, char *cause);
void onDisconnect(void* context, MQTTAsync_successData* response);
void onSend(void* context, MQTTAsync_successData* response);
void onConnectFailure(void* context, MQTTAsync_failureData* response);
void onConnect(void* context, MQTTAsync_successData* response);
int onMessageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
void onSubscribe(void* context, MQTTAsync_successData* response);
void onSubscribeFailure(void* context, MQTTAsync_failureData* response);

#endif /* SRC_THINGMLMQTT_H_ */
