#include "power_control.h"
#include <Arduino.h>
#include "handle_input.h"


long woke_up_at = 0;


void configure_power_vontrol();

void configure_power_off(){

  // send off signal

  // turn off screen

  Serial.println("going into deep sleep");
  delay(1000);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0);

  esp_deep_sleep_start();

}

void configure_power_on(){

  // turn off screen

  Serial.println("going into light sleep");
  delay(10);

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_light_sleep_start();
  woke_up_at = millis();
  if(digitalRead(32) == HIGH){
    Serial.println("rotary");
  }
  Serial.println("woke up from light sleep");


  check_input();

}

void handle_idling(){

  if(digitalRead(33) == HIGH){
    configure_power_off();
  }

  if(time_since_wakeup() > 5000){
    configure_power_on();
  }

}


void update_last_input(){

  woke_up_at = millis();
}

long time_since_wakeup(){
  return millis() - woke_up_at;
}
