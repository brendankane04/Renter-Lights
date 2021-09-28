/*
 * Wifi_Interface.cpp
 *
 *  Created on: Sep 8, 2021
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
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "Wifi_Interface.h"

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      	"Home Network"
#define EXAMPLE_ESP_WIFI_PASS      	"ThanksBrendan!"
#define EXAMPLE_ESP_MAXIMUM_RETRY	10
#define TCPServerIP					"192.168.1.155"
#define MESSAGE "TEST MESSAGE. IF YOU SEE THIS... YAY!!!\n"

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi station";
static const char *TAG_TCP = "TCP Client";

static int s_retry_num = 0;

static void event_handler(void* arg, esp_event_base_t event_base,
								int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(TAG, "retry to connect to the AP");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGI(TAG,"connect to the AP fail");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

void wifi_init_sta(char *ssid, char *password)
{
	//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
	  ESP_ERROR_CHECK(nvs_flash_erase());
	  ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	s_wifi_event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());

	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	//Attach the event handler to the interrupts
	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK
	(
		esp_event_handler_instance_register
		(
			WIFI_EVENT,
			ESP_EVENT_ANY_ID,
			&event_handler,
			NULL,
			&instance_any_id
		)
	);
	ESP_ERROR_CHECK
	(
		esp_event_handler_instance_register
		(
			IP_EVENT,
			IP_EVENT_STA_GOT_IP,
			&event_handler,
			NULL,
			&instance_got_ip
		)
	);


	//Wifi config struct for SSID, password, auth mode, etc.
	wifi_config_t wifi_config = {}; //Initialize to 0
	strcpy((char*) wifi_config.sta.ssid, (char*) ssid);
	strcpy((char*) wifi_config.sta.password, (char*) password);
	wifi_config.sta.pmf_cfg.capable = true;
	wifi_config.sta.pmf_cfg.required = false;


	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
	ESP_ERROR_CHECK(esp_wifi_start() );

	ESP_LOGI(TAG, "wifi_init_sta finished.");

	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
	 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
			WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
			pdFALSE,
			pdFALSE,
			portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
	 * happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
				 ssid, password);
	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
				 ssid, password);
	} else {
		ESP_LOGE(TAG, "UNEXPECTED EVENT");
	}

}

Wifi_Interface::Wifi_Interface(char *ssid, char* password)
{
	//Copy the credentials of the wifi network
	strcpy(this->ssid, ssid);
	strcpy(this->password, password);

	//Call to the reference code
	wifi_init_sta(this->ssid, this->password);
}

void Wifi_Interface::set_target(char *tcp_ip, int tcp_port)
{
	strcpy(this->tcp_ip, tcp_ip);
	this->tcp_port = tcp_port;

}

void Wifi_Interface::send(char *data)
{
	int send_socket;
	ESP_LOGI(TAG_TCP,"tcp_client task started \n");

	//Initialize the socket address
	struct sockaddr_in tcpServerAddr;
	tcpServerAddr.sin_addr.s_addr = inet_addr(this->tcp_ip);
	tcpServerAddr.sin_family = AF_INET;
	tcpServerAddr.sin_port = htons(this->tcp_port);

	//Repeat if there is an error
	while(1)
	{
		//Ensure wifi is connected
		xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);

		//Initialize the socket
		send_socket = socket(AF_INET, SOCK_STREAM, 0);
		if(send_socket < 0)
		{
			ESP_LOGE(TAG_TCP, "... Failed to allocate socket.\n");
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			continue;
		}
		ESP_LOGI(TAG_TCP, "... allocated socket\n");

		//Connect the socket
		if(connect(send_socket, (struct sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr)) != 0)
		{
			ESP_LOGE(TAG_TCP, "... socket connect failed errno=%d \n", errno);
			close(send_socket);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			continue;
		}
		ESP_LOGI(TAG_TCP, "... connected \n");

		//Write to the socket
		if( write(send_socket , MESSAGE , strlen(MESSAGE)) < 0)
		{
			ESP_LOGE(TAG_TCP, "... Send failed \n");
			close(send_socket);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			continue;
		}
		ESP_LOGI(TAG_TCP, "... socket send success");

		//Close the socket
		close(send_socket);
		break;
	}
	ESP_LOGI(TAG_TCP, "...tcp_client task closed\n");
}

//Receive a number of bytes
void Wifi_Interface::recv(char *recv_buf, int size)
{
	int recv_socket, recv_flag;
	ESP_LOGI(TAG_TCP,"tcp_client task started \n");

	//Initialize the socket address
	struct sockaddr_in tcpServerAddr;
	tcpServerAddr.sin_addr.s_addr = inet_addr(this->tcp_ip);
	tcpServerAddr.sin_family = AF_INET;
	tcpServerAddr.sin_port = htons(this->tcp_port);

	//Repeat if there is an error
	while(1)
	{
		//Ensure wifi is connected
		xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);

		//Initialize the socket
		recv_socket = socket(AF_INET, SOCK_STREAM, 0);
		if(recv_socket < 0)
		{
			ESP_LOGE(TAG_TCP, "... Failed to allocate socket.\n");
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			continue;
		}
		ESP_LOGI(TAG_TCP, "... allocated socket\n");

		//Connect the socket
		if(connect(recv_socket, (struct sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr)) != 0)
		{
			ESP_LOGE(TAG_TCP, "... socket connect failed errno=%d \n", errno);
			close(recv_socket);
			vTaskDelay(4000 / portTICK_PERIOD_MS);
			continue;
		}
		ESP_LOGI(TAG_TCP, "... connected \n");

		//Read from the socket
		bzero(recv_buf, size);
		recv_flag = read(recv_socket, recv_buf, sizeof(recv_buf)-1);
		ESP_LOGI(TAG_TCP, "... done reading from socket. Last read return=%d errno=%d\r\n", recv_flag, errno);

		//Close the socket
		close(recv_socket);
		break;
	}
	ESP_LOGI(TAG_TCP, "...tcp_client task closed\n");
}
