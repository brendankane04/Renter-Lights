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
		char ssid[64];
		char password[64];
		char tcp_ip[64];
		int  tcp_port;

	public:
		Wifi_Interface(char *ssid, char *password);
		Wifi_Interface(char *ssid, char* password, char *tcp_ip, int tcp_port);
		void set_target(char*, int);
		void send(char *data);
		void recv(char*, int);
};

#endif /* MAIN_WIFI_INTERFACE_H_ */
