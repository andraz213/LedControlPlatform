#include "power_control.h"
#include <Arduino.h>
#include "handle_input.h"
#include "global_defines.h"
#include "ui_handle.h"
#include "handle_screen.h"
#include "handle_wifi.h"

long woke_up_at = -100000;


void configure_power_vontrol();

void configure_power_off(){

  // send off signal
  send_off();

  // turn off screen
  turn_off_screen();

  Serial.println("going into deep sleep");
  delay(10);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0);
  //esp_sleep_enable_timer_wakeup(10000 * 1000);
  esp_deep_sleep_start();

}

void configure_power_on(){

  // turn off screen

  Serial.println("going into light sleep");
  turn_off_screen();
  delay(10);
  //esp_sleep_enable_timer_wakeup(10000 * 1000);
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_light_sleep_start();
  if(digitalRead(SWITCH_PIN) == HIGH){
    open_menu();
  }

  if(digitalRead(ROTARY_A_PIN) == HIGH){

    change_scene((int) MASTER_SET);
    Serial.println("woke up from light sleep DUE TO ROTARY");
  }

  if(digitalRead(POWER_PIN) == HIGH){
    Serial.println("woke up from light sleep DUE TO POWER_PIN");
  }
  woke_up_at = millis();
  Serial.println("woke up from light sleep");
  check_input();
  check_wakeup();

  turn_on_screen();

  blackout_screen();


}

void handle_idling(){

  if(digitalRead(POWER_PIN) == HIGH){
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



void check_wakeup(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER  : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }


}
