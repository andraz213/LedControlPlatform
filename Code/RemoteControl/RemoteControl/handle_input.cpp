#include "handle_input.h"
#include "power_control.h"
#include <Arduino.h>


long prev_rotary = 0;
void IRAM_ATTR isr_rotary() {

  if(millis() - prev_rotary > 20){
    if(digitalRead(32) == HIGH){
      if(digitalRead(13) == HIGH){
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      update_last_input();
      prev_rotary = millis();
    }

  }

}

long prev_switch = 0;
void IRAM_ATTR isr_switch() {

  if(millis() - prev_switch > 100){
    Serial.println("SWITCH");
    update_last_input();
    prev_switch = millis();
  }


}


void check_input(){




  return;
}



void configure_pins(){
  pinMode(13, INPUT_PULLDOWN);
  attachInterrupt(32, isr_rotary, RISING);
  attachInterrupt(37, isr_switch, RISING);


}
