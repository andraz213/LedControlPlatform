#include "ui_handle.h"
#include "WiFi.h"
#include <Wire.h>
#include "esp_adc_cal.h"
#include "power_control.h"
#include "handle_input.h"
#include "global_defines.h"
#include "ui_handle.h"
#include "handle_screen.h"



void setup(){

  Serial.begin(115200);
  Serial.println("LED CONTROL");

  configure_pins();
  blackout_screen();


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
