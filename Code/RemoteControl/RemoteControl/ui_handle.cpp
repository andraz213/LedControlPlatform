#include "ui_handle.h"
#include <Arduino.h>
#include "global_defines.h"

int current_scene = 0;

menu_item menuItems[5] = {
  {"RGB", (void*) 0},
  {"Fire", (void*) 0},
  {"Sunset", (void*) 0},
  {"StarryNight", (void *) 0},
  {"OTA", (void*) 0}
};

void open_menu(){
  current_scene = MENU;
  Serial.println("em v meniju");
}


void handle_ui(){








}
