#include "handle_input.h"
#include "power_control.h"
#include <Arduino.h>
#include "ui_handle.h"
#include "global_defines.h"


bool switch_pressed = false;
int rotary_turned = 0;


long prev_rotary = 0;
void IRAM_ATTR isr_rotary() {

  if(millis() - prev_rotary > 20){
    if(digitalRead(ROTARY_A_PIN) == HIGH){
      if(digitalRead(ROTARY_B_PIN) == HIGH){
        Serial.println("DOWN");
        rotary_turned--;
      } else {
        rotary_turned++;
        Serial.println("UP");
      }

      Serial.println(rotary_turned);
      update_last_input();
      prev_rotary = millis();
    }

  }

}

long prev_switch = 0;
void IRAM_ATTR isr_switch() {

  if(millis() - prev_switch > 100){
    Serial.println("SWITCH");
    switch_pressed = true;
    update_last_input();
    prev_switch = millis();
  }


}






void check_input(){




  return;
}


bool check_switch(){
  if(switch_pressed){
    switch_pressed = false;
    return true;
  }
  return false;
}

int get_rotary(){
  int result = rotary_turned;
  rotary_turned = 0;

  return result;

}



void configure_pins(){
  pinMode(ROTARY_B_PIN, INPUT_PULLDOWN);
  attachInterrupt(ROTARY_A_PIN, isr_rotary, RISING);
  attachInterrupt(SWITCH_PIN, isr_switch, RISING);


}
