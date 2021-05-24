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
#include "handle_input.h"
#include "ui_handle.h"
#include "handle_screen.h"


bool no_interrupt = false;


void do_ota(int rot, bool sw){
      check_switch();
      int fake = 0;
      send_other((int) OTA_MODE, &fake, 0);

      draw_OTA(0, "", 0);

    /*  esp_wifi_start();
      Serial.println("\ndsaf");
      esp_wifi_disconnect ();
      Serial.println("\nEnasdf");
      esp_wifi_stop ();
      Serial.println("\nEndsaaasa");
      esp_wifi_deinit ();
      Serial.println("\nEndaaaaaaaaaaaaaaaaaa");
      esp_wifi_start ();
      Serial.println("\nEnfsfd");
      WiFi.mode(WIFI_STA);*/

      // int a = esp_wifi_set_protocol((wifi_interface_t) ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B| WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N);
      esp_wifi_start ();
      WiFi.begin("PSP256", "siol2004");
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        delay(100);
        if(check_switch() && !no_interrupt){
          change_scene((int)MENU);
          return;
        }
      }

      draw_OTA(1, String(WiFi.localIP()), 0);
      ArduinoOTA.setHostname("TadejinLedKontroler");

      check_switch();

      Serial.println(WiFi.localIP());

  ArduinoOTA
    .onStart([]() {
      no_interrupt = true;
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
      //draw_OTA(2, String("Start updating " + type), 0);
      blackout_screen();
    })
    .onEnd([]() {
      Serial.println("\nEnd");
      draw_OTA(2, String("DONE"), 0);
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      no_interrupt = true;
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));

      draw_OTA(3, String("Progress"), (int)(progress / (total / 100)));


    })
    .onError([](ota_error_t error) {
      String rr = "Error " + String(error);
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) rr += String("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) rr += String("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) rr += String("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) rr += String("Receive Failed");
      else if (error == OTA_END_ERROR) rr += String("End Failed");

      draw_OTA(2, rr, 0);
    });

  ArduinoOTA.begin();

  long pr = millis();

  while(true){
      ArduinoOTA.handle();
      if(millis() - pr > 300){
        pr = millis();
      }
      if(check_switch() && !no_interrupt){
        change_scene((int)MENU);
        return;
      }
  }


}
