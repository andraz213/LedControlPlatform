#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include "esp_adc_cal.h"
#include "power_control.h"
#include "handle_input.h"





void setup(){

  Serial.begin(115200);
  Serial.println("LED CONTROL");

  configure_pins();


}

void loop(){

handle_idling();

}
