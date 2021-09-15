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

/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t s_wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

int s_retry_num = 0;
int max_tries = 10;


void event_handler
(
	void* arg,
	esp_event_base_t event_base,
	int32_t event_id,
	void* event_data
)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < max_tries)
        {
            esp_wifi_connect();
            s_retry_num++;
            printf("retry to connect to the AP");
        } else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        printf("connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        printf("got ip: IPSTR, IP2STR(&event->ip_info.ip)"); //TODO: see if this compiles properly
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

Wifi_Interface::Wifi_Interface()
{
	//Just the constructor
}

void Wifi_Interface::init()
{
	esp_err_t err;
	uint8_t ssid[32] = "Home Network";
	uint8_t password[64] = "Thanks Brendan!";
	max_tries = 10;



	//Initialize the lwIP interface & start the task
	err = esp_netif_init();
	ESP_ERROR_CHECK(err);

	//Initialize the event loop
	err = esp_event_loop_create_default();
	ESP_ERROR_CHECK(err);

	//The default station network interface
	esp_netif_create_default_wifi_sta();

	//Initialize the wifi system
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	err = esp_wifi_init(&cfg);
	ESP_ERROR_CHECK(err);

	//Wifi handler instances
	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;

	//Register the "any_id" handler
	err = esp_event_handler_instance_register
	(
		WIFI_EVENT,
		ESP_EVENT_ANY_ID,
		&event_handler,
		NULL,
		&instance_any_id
	);
	ESP_ERROR_CHECK(err);

	//Register the "got_ip" handler
	err = esp_event_handler_instance_register
	(
		IP_EVENT,
		IP_EVENT_STA_GOT_IP,
		&event_handler,
		NULL,
		&instance_got_ip
	);
	ESP_ERROR_CHECK(err);

	//Wifi config struct for SSID, password, auth mode, etc.
	wifi_config_t wifi_config;
	stat_config_t sta_config;

	sta_config.ssid = ssid;
	sta_config.password = password;

	wifi_config.sta = sta_config
    wifi_config_t wifi_config =
    {
        .sta =
        {
			.ssid = ssid,
            .password = password,
//			 .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg =
            {
                .capable = true,
                .required = false
            },
        },
    };
    //Set the wifi mode to station (basically the device connecting to the router)
    err = esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(err);
    //Configure the wifi with the previous struct
    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    ESP_ERROR_CHECK(err);

    //Start the wifi connection
    err = esp_wifi_start();
    ESP_ERROR_CHECK(err);

    //Set the event bits which indicate either a connection or too many failures
    EventBits_t bits = xEventGroupWaitBits
    (
    	s_wifi_event_group,
		WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
		pdFALSE,
		pdFALSE,
		portMAX_DELAY
	);

    //Printf an success or fail message
    if (bits & WIFI_CONNECTED_BIT)
        printf("connected to ap SSID:%s password:%s", ssid, password);
    else if (bits & WIFI_FAIL_BIT)
        printf("Failed to connect to SSID:%s, password:%s", ssid, password);
    else
    	printf("UNEXPECTED EVENT");

    //Unregister the event handlers, so they won't be pocessed anymore
    err = esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip);
    ESP_ERROR_CHECK(err);
	err = esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id);
	ESP_ERROR_CHECK(err);
	vEventGroupDelete(s_wifi_event_group);

}

