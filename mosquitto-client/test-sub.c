/*
 * test-sub.c
 * Part of the mosquito-test demonstration application
 * Consumes messages from an MQTT topic indefinitely
 * Copyright (c)2016 Kevin Boone. Distributed under the terms of the 
 *  GPL v3.0 
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <json/json.h>
#include "utility.h"
#include "mqttTopicProvider.h"

// Server connection parameters
#define MQTT_HOSTNAME "127.0.0.1" 
#define MQTT_PORT 1883
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "admin"
#define MQTT_TOPIC gMqttTopic

struct mosquitto *mosq = NULL;
struct json_object *payload, *payload2, *payload3, *payload4, *state, *desired,*fun, *data; 
struct json_object *interface, *res;

static char gMqttTopic[256] = {0};

/*
 * my_message_callback. 
 * Called whenever a new message arrives
 */
void my_message_callback(struct mosquitto *mosq, void *obj, 
    const struct mosquitto_message *message)
  {
  // Note: nothing in the Mosquitto docs or examples suggests that we
  //  must free this message structure after processing it.
    printf ("gino Tpoic:%s\n ",(char *)message->topic);
    printf ("gino InComing message: %s\n", (char *)message->payload);

   //parse mqtt payload
	/*
		receive format ex:{"state":{"desired":{"data":{"function":"getZwaveGatewayInit"}}}}
	*/
 	payload = json_tokener_parse((char *)message->payload);
	state = json_object_object_get(payload, "state");
	if(state == NULL) {
		printf("state is null\n");
	} else {
		printf("state is = %s\n",json_object_get_string(state));

		payload2 = json_tokener_parse(json_object_get_string(state));
		desired = json_object_object_get(payload2, "desired");
		if(desired == NULL) {
			printf("desired is null\n");
		} else {		
			printf("desired is = %s\n",json_object_get_string(desired));
		}

		payload3 = json_tokener_parse(json_object_get_string(desired));
		data = json_object_object_get(payload3, "data");	
		if(data == NULL) {
			printf("data is null\n");
		} else {
			printf("data is = %s\n",json_object_get_string(data));
		}

		payload4 = json_tokener_parse(json_object_get_string(data));
		fun = json_object_object_get(payload4, "function");	
		if(fun == NULL) {
			printf("fun is null\n");
		} else {
			printf("fun is = %s\n",json_object_get_string(fun));
		}
	}
	
	//if payload == keyword do something
	/*
		send format ex: {"reprot":{"Interface":"zwaveGatewayInit","result":"false"}}
	*/
	if(strcmp(json_object_get_string(fun),"getZwaveGatewayInit") == 0) {
		interface = json_object_new_object();
		json_object_object_add(interface, "Interface", json_object_new_string("zwaveGatewayInit"));
		json_object_object_add(interface, "result", json_object_new_string("true")); 
		res = json_object_new_object();
		json_object_object_add(res, "reported", json_object_new_string(json_object_to_json_string(interface)));
		mosquitto_publish (mosq, NULL, MQTT_TOPIC, strlen (json_object_to_json_string(res)),json_object_to_json_string(res), 0, false);
		
	} else if(strcmp(json_object_get_string(fun),"getDeviceList") == 0) {
		interface = json_object_new_object();
		json_object_object_add(interface, "Interface", json_object_new_string("getDeviceList"));
		json_object_object_add(interface, "deviceList", json_object_new_string("[]")); 
		//json_object_object_add(interface, "deviceList", json_object_new_string("[{\"brand\":\"\",\"nodeId\":\"1\",\"deviceType\":\"\",\"name\":\"1\",\"category\":\"unknown\",\"room\":\"My Home\",\"isFavorite\":\"0\"},{\"brand\":\"\",\"nodeId\":\"47\",\"deviceType\":\"\",\"name\":\"47\",\"category\":\"unknown\",\"room\":\"My Home\",\"isFavorite\":\"0\",\"timestamp\":1540522685231}]")); 
		res = json_object_new_object();
		json_object_object_add(res, "reported", json_object_new_string(json_object_to_json_string(interface)));
		mosquitto_publish (mosq, NULL, MQTT_TOPIC, strlen (json_object_to_json_string(res)),json_object_to_json_string(res), 0, false);

	} else if(strcmp(json_object_get_string(fun),"getRecentDeviceList") == 0) {
		interface = json_object_new_object();
		json_object_object_add(interface, "Interface", json_object_new_string("getRecentDeviceList"));
		json_object_object_add(interface, "deviceList", json_object_new_string("[{\"brand\":\"\",\"nodeId\":\"47\",\"deviceType\":\"\",\"name\":\"47\",\"category\":\"unknown\",\"room\":\"My Home\",\"isFavorite\":\"0\",\"timestamp\":1540522685231}]")); 
		res = json_object_new_object();
		json_object_object_add(res, "reported", json_object_new_string(json_object_to_json_string(interface)));
		mosquitto_publish (mosq, NULL, MQTT_TOPIC, strlen (json_object_to_json_string(res)),json_object_to_json_string(res), 0, false);
		
	} else
		printf("payload is not support\n");

	// Free
	json_object_put(payload);
	json_object_put(payload2);
	json_object_put(payload3);
	json_object_put(payload4);
	json_object_put(interface);
	json_object_put(res);


}


void * test(void * arg)
  {    

    while(1)
    {
      	mosquitto_publish (mosq, NULL, MQTT_TOPIC, strlen ("Hello gino"), "Hello gino", 0, false);
      //printf("TEST thread!!\n");
      //mosquitto_publish (mosq, NULL, MQTT_TOPIC, strlen ("Hello gino"), "Hello gino2", 0, false);
      //sleep(2);
    }

    return NULL;
  }

/*
 * Start here
 */
int main (int argc, char **argv)
  {
    if (0 != getMqttTopic(gMqttTopic, sizeof(gMqttTopic))) {
        return -1;
    }

    if (0 != startMqttTopicProviderTask(gMqttTopic)) {
        return -1;
    }
  //pthread_t pth;
 
  // Initialize the Mosquitto library
  mosquitto_lib_init();

  // Create a new Mosquito runtime instance with a random client ID,
  //  and no application-specific callback data.  
  mosq = mosquitto_new (NULL, true, NULL);
  if (!mosq)
    {
    fprintf (stderr, "Can't init Mosquitto library\n");
    exit (-1);
    }

  // Set up username and password
  //mosquitto_username_pw_set (mosq, MQTT_USERNAME, MQTT_PASSWORD);

  // Establish a connection to the MQTT server. Do not use a keep-alive ping
  int ret = mosquitto_connect (mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
  if (ret)
    {
    fprintf (stderr, "Can't connect to Mosquitto server\n");
    exit (-1);
    }


// Subscribe to the specified topic. Multiple topics can be
      //  subscribed, but only one is used in this simple example.
      //  Note that we don't specify what to do with the received
      //  messages at this point
       ret = mosquitto_subscribe(mosq, NULL, MQTT_TOPIC, 0);
      if (ret)
        {
        fprintf (stderr, "Can't subscribe to Mosquitto server\n");
        exit (-1);
        }
     
      // Specify the function to call when a new message is received
      mosquitto_message_callback_set (mosq, my_message_callback);

      //pthread_create(&pth, NULL, test, NULL);



      ret=mosquitto_loop_forever(mosq, -1, 1);

  
  // Tody up. In this simple example, this point is never reached. We can
  //  force the mosquitto_loop_forever call to exit by disconnecting
  //  the session in the message-handling callback if required.
 // mosquitto_destroy (mosq);
 // mosquitto_lib_cleanup();
  
  return 0;
  }

