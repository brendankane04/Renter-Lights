/*
 * MQTT.h
 *
 *  Created on: Jul 7, 2022
 *      Author: brend
 */

#ifndef MQTT_H_
#define MQTT_H_


#include "mqtt_client.h"


#define CONFIG_BROKER_URL "mqtt://testserver.local:1883"


esp_mqtt_client_handle_t mqtt_init();


#endif /* MAIN_MQTT_H_ */
