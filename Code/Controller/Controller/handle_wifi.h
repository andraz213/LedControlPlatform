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

void handle_master(int val);


void handle_other(int type, int * params, int n_params);

void handle_off();

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

#endif
