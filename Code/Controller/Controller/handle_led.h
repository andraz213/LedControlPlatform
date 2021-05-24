#ifndef HANDLE_LED_H
#define HANDLE_LED_H

typedef struct {
  bool done;
  int position;
  float distance;
  float initial_distance;
  float speed;
  float strength;
} star_data;


bool init_led();

void cycle_leds();

void recieve_data(int type, int * params, int n_params);

void set_master(int val);

void apply_master();

void apply_rgbw_correction();

void display();

void calculate_next_step();

void reset_data();

void calculate_warm_white();

void calculate_rgb();

void calculate_fire();

void setNewColor(int pix);

void calculate_temp();

void calculate_stars();

void calculate_turbo();

#endif
