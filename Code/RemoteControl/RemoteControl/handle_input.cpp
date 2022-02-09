#include "handle_input.h"
#include "power_control.h"
#include <Arduino.h>
#include "ui_handle.h"
#include "global_defines.h"


bool changing_parameters = false;
void set_changing_parameters(bool set_val) {
    changing_parameters = set_val;
}


bool switch_pressed = false;
int rotary_turned = 0;


long rotary_history_dir[101][2];
int dir_pointer = 0;
long rotary_history[100];
int rotary_history_ptr = 0;

long prev_rotary = 0;
long prev_rotary_read = 0;
void IRAM_ATTR isr_rotary() {
  int dir = 0;

  if (millis() - prev_rotary_read > 20) {
    prev_rotary_read = millis();
    if (digitalRead(ROTARY_A_PIN) == HIGH) {
      if (digitalRead(ROTARY_B_PIN) == LOW) {
        dir--;
      } else {
        dir++;
      }
      rotary_history_dir[dir_pointer][0] = millis();
      rotary_history_dir[dir_pointer][1] = dir;
      dir_pointer++;
      dir_pointer %= 100;
    }
  }


  int rot_delay = 20;
  if (!changing_parameters) {
    rot_delay = 333;
  }
  if (millis() - prev_rotary > rot_delay) {
    prev_rotary = millis();

    int new_dir = 0;
    for (int i = 0; i < 100; i++) {
      if (millis() - rotary_history_dir[i][0] < 700) {
        new_dir += rotary_history_dir[i][1];
      }
    }

    dir = -1;
    if (new_dir > 0) {
      dir = 1;
    }

    rotary_history[rotary_history_ptr] = millis();

    rotary_history_ptr += 1;
    rotary_history_ptr %= 100;

    int n = 0;
    for (int i = 0; i < 100; i++) {
      if (millis() - rotary_history[i] < 1700) {
        n += 1;
      }
    }

    int mult = map(n * n, 1, 1000, 1, 10);

    dir *= mult;
    rotary_turned += dir;

    //Serial.println(mult);
    //Serial.println(dir);

    update_last_input();

  }

}



long prev_switch = 0;
void IRAM_ATTR isr_switch() {

  if (millis() - prev_switch > 200) {
    Serial.println("SWITCH");
    switch_pressed = true;
    update_last_input();
    prev_switch = millis();
  }


}






void check_input() {




  return;
}


bool check_switch() {
  if (switch_pressed) {
    switch_pressed = false;
    return true;
  }
  return false;
}

int get_rotary() {
  int result = rotary_turned;
  rotary_turned = 0;

  return result;

}



void configure_pins() {

  attachInterrupt(ROTARY_A_PIN, isr_rotary, RISING);
  attachInterrupt(SWITCH_PIN, isr_switch, RISING);


}
