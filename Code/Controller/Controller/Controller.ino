
#include "WiFi.h"

#include "global_defines.h"

#include "handle_led.h"
#include "handle_wifi.h"



void setup(){

  setCpuFrequencyMhz(240);
  Serial.begin(115200);
  Serial.println("LED CONTROL");


  init_wifi();
  init_led();

}


void loop(){

delay(5);

cycle_leds();
delay(5);

}
