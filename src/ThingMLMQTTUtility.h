/*
 * ThingMLMQTTUtility.h
 *
 *  Created on: Jul 13, 2015
 *      Author: vassik
 */

#ifndef SRC_THINGMLMQTTUTILITY_H_
#define SRC_THINGMLMQTTUTILITY_H_

typedef void (*pthingMLMQQTPaho)(void* _instance, ...);

typedef struct {
	pthingMLMQQTPaho fn_connlost_callback;
	pthingMLMQQTPaho fn_ondisconnet_callback;
	pthingMLMQQTPaho fn_onsend_callback;
	pthingMLMQQTPaho fn_onconnfail_callback;
	pthingMLMQQTPaho fn_onconn_callback;
	pthingMLMQQTPaho fn_onmsgrcv_callback;
	pthingMLMQQTPaho fn_onsub_callback;
	pthingMLMQQTPaho fn_onsubfail_callback;
	void* thing_instance;
	void* paho_client;
	const char* client_id;

} ThingMLMQTTContext;

#endif /* SRC_THINGMLMQTTUTILITY_H_ */
