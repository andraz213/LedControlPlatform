#ifndef HANDLE_WIFI_H
#define HANDLE_WIFI_H

#include "global_defines.h"
#include <esp_pm.h>
#include <esp_wifi.h>
#include <esp_wifi_internal.h>
#include <esp_wifi_types.h>
#include <esp_now.h>
#include <WiFi.h>




void init_wifi();

void send_master(int val);

void send_other(int mode, int * params, int n_params);

void send_off();

void turn_on_wifi();

void turn_off_wifi();

void send_data(uint8_t * data, int size);

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

#endif
