#include "handle_ota.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <esp_pm.h>
#include <esp_wifi.h>
#include <esp_wifi_internal.h>
#include <esp_wifi_types.h>
#include <esp_now.h>
#include <WiFi.h>
#include "handle_wifi.h"
#include "handle_led.h"



void do_ota(int rot, bool sw){

      esp_wifi_start ();
      WiFi.begin("PSP256", "siol2004");
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        delay(100);
      }

      ArduinoOTA.setHostname("LediceStrop");

  ArduinoOTA
    .onStart([]() {

    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {

      set_master((int)(progress / (total / 100)));
      calculate_warm_white();
      display();

      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));


    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.printf("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.printf("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.printf("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.printf("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.printf("End Failed");

    });

  ArduinoOTA.begin();
  long start = millis();
  while(millis() - start < (3 * 60 * 1000)){
      ArduinoOTA.handle();

      int val = (millis() % 2000) / 10;
      if(val > 100){
        val = 200 - val;
      }
      set_master(val);
      calculate_warm_white();
      display();
  }

  ESP.restart();


}
