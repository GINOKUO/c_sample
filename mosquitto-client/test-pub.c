/*
 * test-sub.c
 * Part of the mosquito-test demonstration application
 * Publishes a fixed number of simple messages to a topic 
 * Copyright (c)2016 Kevin Boone. Distributed under the terms of the 
 *  GPL v3.0 
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <json/json.h>

// Server connection parameters
#define MQTT_HOSTNAME "211.75.141.112" 
#define MQTT_PORT 1883
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "admin"
#define MQTT_TOPIC "E181H000027"

/*
 * Start here
 */
int main (int argc, char **argv)
  {
  struct mosquitto *mosq = NULL;
 
  // Initialize the Mosquitto library
  mosquitto_lib_init();

  // Create a new Mosquito runtime instance with a random client ID,
  //  and no application-specific callback data.  
  mosq = mosquitto_new (NULL, true, NULL);
  if (!mosq)
    {
    fprintf (stderr, "Can't initialize Mosquitto library\n");
    exit (-1);
    }

  mosquitto_username_pw_set (mosq, MQTT_USERNAME, MQTT_PASSWORD);

  // Establish a connection to the MQTT server. Do not use a keep-alive ping
  int ret = mosquitto_connect (mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
  if (ret)
    {
    fprintf (stderr, "Can't connect to Mosquitto server\n");
    exit (-1);
    }

    struct json_object *root, *name, *sex, *data, *edu, *prof;
 
	root = json_object_new_object();

	json_object_object_add(root, "Interface", json_object_new_string("zwaveGatewayInit"));
	json_object_object_add(root, "result", json_object_new_string("true")); 

	data = json_object_new_object();
	json_object_object_add(data, "reported", json_object_new_string(json_object_to_json_string(root)));

        mosquitto_publish (mosq, NULL, MQTT_TOPIC, strlen (json_object_to_json_string(data)),json_object_to_json_string(data), 0, false);
	// Decrease counter and free object
	json_object_put(root);
	json_object_put(data);
        //sleep(2);



/* 
root = json_object_new_object();
json_object_object_add(root, "name", json_object_new_string("Brianw"));
json_object_object_add(root, "sex", json_object_new_int(0));
 
data = json_object_new_object();
json_object_object_add(data, "education", json_object_new_string("master"));
json_object_object_add(data, "profession", json_object_new_string("engineer"));
json_object_object_add(root, "data", data);
mosquitto_publish (mosq, NULL, MQTT_TOPIC, strlen (json_object_to_json_string(root)),json_object_to_json_string(root), 0, false);
// Output to string
//printf("%s", json_object_to_json_string(root));
 
// Decrease counter and free object
json_object_put(data);
json_object_put(root);
*/







/* receive json
char buf[] = "{ \"name\": \"bar\", \"sex\": \"bar2\", \"data\": \"bar3\" }";

root = json_tokener_parse(buf);
// Use is_error() to check the result, don't use "j_root == NULL".

 
name = json_object_object_get(root, "name");
sex = json_object_object_get(root, "sex");
data = json_object_object_get(root, "data");
// If parse fail, object is NULL
if (data != NULL) {
  edu = json_object_object_get(data, "education");
  prof= json_object_object_get(data, "profession");
}
 

// Fetch value
printf("name=%s\n", json_object_get_string(name));
printf("sex=%d\n", json_object_get_int(sex));
printf("education=%s\n", json_object_get_string(data));

// Free
json_object_put(root);
*/

 

  // We need a short delay here, to prevent the Mosquito library being
  //  torn down by the operating system before all the network operations
  //  are finished.
  sleep (1);

  // Tidy up
  mosquitto_disconnect (mosq);
  mosquitto_destroy (mosq);
  mosquitto_lib_cleanup();
  
  return 0;
  }

