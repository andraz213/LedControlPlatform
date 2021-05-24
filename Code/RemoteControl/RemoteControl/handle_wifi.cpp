#include "handle_wifi.h"
#include <esp_pm.h>
#include <esp_wifi.h>
#include <esp_wifi_internal.h>
#include <esp_wifi_types.h>
#include <esp_now.h>
#include <WiFi.h>


bool inited_wifi = false;
bool wifi_on = false;
bool sent = false;

uint8_t mac_address [6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff} ;
esp_now_peer_info_t peerInfo;



void init_wifi(){

  if(!inited_wifi){


    WiFi.mode(WIFI_STA);

    int a = esp_wifi_set_protocol((wifi_interface_t) ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B| WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N);

    //esp_wifi_set_channel(4, WIFI_SECOND_CHAN_NONE);
    wifi_on = true;

    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  memcpy(peerInfo.peer_addr, mac_address, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;


  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Inited ESP-NOW");
  inited_wifi = true;

  }



}

void send_master(int val){

  uint8_t * data = (uint8_t *) heap_caps_malloc(sizeof(int) * 2, MALLOC_CAP_8BIT);
  int type = (int) MASTER_MESSAGE;

  memcpy(data, (char*)&type, (sizeof(int)));
  memcpy(data + sizeof(int), (char*)&val, (sizeof(int)));

  send_data(data, sizeof(int) * 2);

  free(data);

}

void send_other(int mode, int * params, int n_params){

  int size = n_params + 2;

  uint8_t * data = (uint8_t *) heap_caps_malloc(sizeof(int) * size, MALLOC_CAP_8BIT);
  int type = (int) OTHER_MESSAGE;

  memcpy(data, (char*)&type, (sizeof(int)));
  memcpy(data + sizeof(int), (char*)&mode, (sizeof(int)));
  if(n_params > 0){
    memcpy(data + sizeof(int)*2, (char*)params, (sizeof(int) * n_params));
  }

  send_data(data, sizeof(int) * size);
  free(data);

}

void send_off(){

  uint8_t * data = (uint8_t *) heap_caps_malloc(sizeof(int), MALLOC_CAP_8BIT);
  int type = (int) OFF_MESSAGE;
  memcpy(data, (char*)&type, (sizeof(int)));
  send_data(data, sizeof(int));
  free(data);

}




void turn_on_wifi(){
  if(!wifi_on){
    esp_wifi_start();
    wifi_on = true;
  }
}


void turn_off_wifi(){
  if(wifi_on){
    esp_wifi_stop();
    wifi_on = false;
  }
}




void send_data(uint8_t * data, int size){
  noInterrupts();
  setCpuFrequencyMhz(240);
  init_wifi();
  turn_on_wifi();


  sent = false;

  esp_err_t result = esp_now_send(mac_address, (uint8_t *) data, size);


  while(!sent){
    delayMicroseconds(10);
  }
  turn_off_wifi();
  sent = false;
  interrupts();
}




void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
  return;
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  sent = true;
  Serial.println("sended");
}
