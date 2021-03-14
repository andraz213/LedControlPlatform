#ifndef HANDLE_UI_H
#define HANDLE_UI_H
#include "global_defines.h"
#include <Arduino.h>

enum scenes {
  MASTER_SET,
  MENU,
  MENU_ITEM
};


typedef struct {
  char display_name[20];
  void * handler;
} menu_item;



void handle_ui();

void open_menu();



#endif
