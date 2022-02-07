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
  void (* handler) (int, bool);
} menu_item;



void handle_ui();

void open_menu();

bool menu_input_handle();

void menu_handler();

void change_scene(int scene);

void master_set_handler();

int master_set_input_handle();

void handle_menu_item();

void handle_RGB(int rot, bool sw);

void handle_fire(int rot, bool sw);

void handle_stars(int rot, bool sw);

void handle_sunset(int rot, bool sw);

void handle_kelvins(int rot, bool sw);

void handle_turbo(int rot, bool sw);

void handle_ota(int rot, bool sw);

#endif
