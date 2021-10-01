/*
 * Wifi_Interface.h
 *
 *  Created on: Sep 12, 2021
 *      Author: brend
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#ifndef MAIN_WIFI_INTERFACE_H_
#define MAIN_WIFI_INTERFACE_H_


class Wifi_Interface
{
	private:
		//Name of the wifi network
		char ssid[64];
		//Password to connect
		char password[64];
		char tcp_ip[64];
		int  tcp_port;

	public:
		//Connect to a wifi network
		Wifi_Interface(char *ssid, char *password);
		//Connect to a wifi newtork & choose a default port
		Wifi_Interface(char *ssid, char* password, char *tcp_ip, int tcp_port);
		//Set the target device ip & port you're talking to
		void set_target(char*, int);
		//Send a message via TCP
		void send(char *data);
		//Receive a message via TCP, fills the 'recv_buf' with 'size' bytes
		void recv(char*, int);
};

#endif /* MAIN_WIFI_INTERFACE_H_ */
