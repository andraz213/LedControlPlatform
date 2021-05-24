#include "handle_wifi.h"
#include <esp_pm.h>
#include <esp_wifi.h>
#include <esp_wifi_internal.h>
#include <esp_wifi_types.h>
#include <esp_now.h>
#include <WiFi.h>
#include "global_defines.h"
#include "handle_led.h"


bool inited_wifi = false;
bool wifi_on = false;
bool sent = false;

uint8_t mac_address [6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff} ;
esp_now_peer_info_t peerInfo;

int current_mode_wifi = 0;

void init_wifi(){

  if(!inited_wifi){

    WiFi.mode(WIFI_STA);
    int a = esp_wifi_set_protocol((wifi_interface_t) ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B| WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N);

    wifi_on = true;

    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);



  Serial.println("Inited ESP-NOW");
  inited_wifi = true;

  }

}



void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){

  int type = 0;
  memcpy(&type, (char*)incomingData, (sizeof(int)));

  char * data = (char*) (incomingData + sizeof(int));
  int datalen = len - sizeof(int);

  Serial.println(type);
  Serial.println((int) MASTER_MESSAGE);
  if(type == (int) MASTER_MESSAGE){
    int val = 0;
    memcpy(&val, data, sizeof(int));
    handle_master(val);
    return;

  }

  if(type == (int) OTHER_MESSAGE){
    int other_type = 0;
    memcpy( &other_type, data, sizeof(int));
    current_mode_wifi = other_type;
    datalen -= sizeof(int);
    handle_other(other_type, (int*)(data + sizeof(int)), datalen / sizeof(int));
    return;



  }

    if(type == (int) OFF_MESSAGE){
      handle_off();
      return;
    }


  return;
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  sent = true;
  Serial.println("sended");
}


void handle_master(int val){
  Serial.println(val);
  set_master(val);

}


void handle_other(int type, int * params, int n_params){
  Serial.println(type);
  Serial.println("N_params" + String(n_params));
  for(int i = 0; i<n_params; i++){
    Serial.println("param N " + String(i)+": "+ String(params[i]));
  }

  recieve_data(type, params, n_params);

}

void handle_off(){
  if(current_mode_wifi != (int)SUNSET_MODE){
    handle_master(0);
  }

}
