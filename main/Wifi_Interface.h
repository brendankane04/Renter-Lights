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
		//IP address of current target in IPv4
		char tcp_ip[64];
		//Current target port
		int  tcp_port;

	private:
		//Connect to a wifi network
		Wifi_Interface(char *ssid, char *password);

	public:
		//Return the instance of the object
		static Wifi_Interface& get_instance(char *ssid, char *password);
		//Set the target device ip & port you're talking to. Must be called before sending or receiving any data
		void set_target(char*, int);
		//Send a message via TCP
		void send(char *data, int len);
		//Send a string via TCP
		void send_str(char *data);
		//Receive a message via TCP, fills the 'recv_buf' with 'size' bytes
		void recv(char*, int);
};


#endif /* MAIN_WIFI_INTERFACE_H_ */
