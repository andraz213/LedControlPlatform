#ifndef HANDLE_SCREEN_H
#define HANDLE_SCREEN_H

#include <Arduino.h>

void init_screen();

void draw_splash();

void draw_menu(String names[], int num, int selected, int battery_percentage);

void draw_master(int value);

void draw_RGB(int r, int g, int b, int selected, bool changing);

void draw_fireplace(int param1, int param2, int selected, bool changing);

void draw_sunset(int param1, int param2, int selected, bool changing);

void draw_starrynight(int param1, int param2, int selected, bool changing);

void draw_OTA(int phase);

void blackout_screen();





#endif
