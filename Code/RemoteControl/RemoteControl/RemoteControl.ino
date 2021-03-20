#include "ui_handle.h"
#include "WiFi.h"
#include <Wire.h>
#include "esp_adc_cal.h"
#include "power_control.h"
#include "handle_input.h"
#include "global_defines.h"
#include "ui_handle.h"
#include "handle_screen.h"

#include "handle_wifi.h"



void setup(){
  setCpuFrequencyMhz(240);
  Serial.begin(115200);
  Serial.println("LED CONTROL");

  configure_pins();
  blackout_screen();

  init_wifi();

  send_other((int)WARM_WHITE_MODE, (int *)NULL, 0);
  send_master(5);

  while(1){
    delay(1000);
    setCpuFrequencyMhz(240);
    long start = micros();
    //send_master(12);
    send_off();

    long end = micros();
    Serial.println(end - start);
    setCpuFrequencyMhz(10);
  }

  //delay(1000);

}

long prev = 0;
void loop(){

if(millis() - prev > 50){
  setCpuFrequencyMhz(240);
  handle_idling();
  handle_ui();
  prev = millis();
}

setCpuFrequencyMhz(10);

delay(5);

}
