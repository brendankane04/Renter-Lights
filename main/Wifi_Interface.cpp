/*
 * Wifi_Interface.cpp
 *
 *  Created on: Sep 8, 2021
 *      Author: brend
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "Wifi_Interface.h"

Wifi_Interface::Wifi_Interface()
{
	esp_err_t err;

	//Initialize the netif interface
	err = esp_netif_init();
	ESP_ERROR_CHECK(err);

	//Initialize the event loop
	err = esp_event_loop_create_default();
	ESP_ERROR_CHECK(err);

	//Initialize the wifi system
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	err = esp_wifi_init(&cfg);
	ESP_ERROR_CHECK(err);
}
