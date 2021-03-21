#include "handle_led.h"
#include <Adafruit_NeoPixel.h>
#include "global_defines.h"
#include <Arduino.h>

#define PIN        14

#define NUMPIXELS 240

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

bool led_inited = false;

int master = 0;

float raw_rgb[240][3];

int to_display_rgbw[240][4];

int rgb_data[3] = {125, 125, 125};
int fire_data [2];
int stars_data [2];
int sunset_data [2];
int kelvins = 27;

int current_mode = (int) WARM_WHITE_MODE;


bool init_led(){
  if(!led_inited){

      pixels.begin();
      pixels.setBrightness(255);
      pixels.fill(0x00000000, 0, 240);
      led_inited = true;

    }

    return led_inited;
}

void cycle_leds(){
  calculate_next_step();

  if(current_mode != (int) WARM_WHITE_MODE){
    apply_master();
    apply_rgbw_correction();
  }
  display();
}

void recieve_data(int type, int * params, int n_params){

  if(current_mode != type){
    reset_data();
    Serial.println("HEJV DRUGI SI ANE");
    current_mode = type;
  }

  if(type == (int) RGB_MODE){
    for(int i = 0; i<3; i++){
      rgb_data[i] = params[i];
    }
  }

  if(type == (int) SUNSET_MODE){
    for(int i = 0; i<2; i++){
      sunset_data[i] = params[i];
    }
  }

  if(type == (int) STARS_MODE){
    for(int i = 0; i<2; i++){
      stars_data[i] = params[i];
    }
  }

  if(type == (int) FIRE_MODE){
    for(int i = 0; i<2; i++){
      fire_data[i] = params[i];
    }
  }

  if(type == (int) TEMPERATURE_MODE){
    kelvins = params[0];
  }

}

void set_master(int val){
  master = val;
}

void apply_master(){

  for(int i = 0; i<240; i++){
    for(int j = 0;j<3; j++){
      raw_rgb[i][j] *= (float)(master / 100.0);
    }
  }
}

void apply_rgbw_correction(){
  for(int i = 0; i<240; i++){
    float r = raw_rgb[i][0];
    float g = raw_rgb[i][1];
    float b = raw_rgb[i][2];
    float ww = 0.0;
    float fakk = 10.0;

    if(r / 255.0 < fakk){
      fakk = r / 255.0;
    }

    if(g / 140.0 < fakk){
      fakk = g / 140.0;
    }

    if(b / 36.0 < fakk){
      fakk = b / 36.0;
    }

    r -= 255.0 * fakk;
    g -= 140.0 * fakk;
    b -= 36.0 * fakk;
    ww = 110.0 * fakk;

    to_display_rgbw[i][0] = (int)r;
    to_display_rgbw[i][1] = (int)g;
    to_display_rgbw[i][2] = (int)b;
    to_display_rgbw[i][3] = (int)ww;
  }
}

void display(){
  if(init_led()){
    for(int i = 0; i<240; i++){
      pixels.setPixelColor(i, pixels.Color(to_display_rgbw[i][0], to_display_rgbw[i][1], to_display_rgbw[i][2], to_display_rgbw[i][3]));
    }
    pixels.show();
  }
}

void calculate_next_step(){

  switch(current_mode){
    case (int) WARM_WHITE_MODE : calculate_warm_white(); break;
    case (int) RGB_MODE : calculate_rgb(); break;
    case (int) FIRE_MODE : calculate_fire(); break;
    
    /*case (int) SUNSET_MODE : break;
    case (int) STARS_MODE : break;

    case (int) TEMPERATURE_MODE : break;*/


  }




}

void reset_data(){
  return;
}


void calculate_warm_white(){
  int val = map(master*master, 0, 100*100, 0, 255);
  Serial.println(val);

  for(int i = 0; i<240; i++){
    to_display_rgbw[i][0] = 0;
    to_display_rgbw[i][1] = 0;
    to_display_rgbw[i][2] = 0;
    to_display_rgbw[i][3] = val;
  }
}


void calculate_rgb(){

  for(int i = 0; i<240; i++){
    raw_rgb[i][0] = rgb_data[0];
    raw_rgb[i][1] = rgb_data[1];
    raw_rgb[i][2] = rgb_data[2];
  }
}




int prev [240][3];
int current [240][3];
int next[240][3];
long prevupd = 0;
int update_interval = 150;


void calculate_fire(){


  if(millis() - prevupd > update_interval){
    prevupd = millis();
    update_interval = random(150) + 50;
    for(int i = 0; i<240; i++){
      setNewColor(i);
      }
    }

    int offset = (millis() - prevupd);

    int perc = map(offset, 0, update_interval, 0, 100);


    for(int i = 0; i<240; i++){
      current[i][0] = (next[i][0]-prev[i][0])*perc/100 + prev[i][0];
      current[i][1] = (next[i][1]-prev[i][1])*perc/100 + prev[i][1];
      current[i][2] = (next[i][2]-prev[i][2])*perc/100 + prev[i][2];

      raw_rgb[i][0] = current[i][0];
      raw_rgb[i][1] = current[i][1];
      raw_rgb[i][2] = current[i][2];
      }



}

    void setNewColor(int pix) {
      int n = random(1023);

      int g = 0;
      int b = 0;

      int r = map(n, 0, 1023, 0, 130);
      if (n > 200)
        g = map(n, 200, 1023, 0, 25);
      // g = map(n * n, 100 * 100, 1023 * 1023, 0, 255);
      if (n > 755)
        b = map(n, 755, 1023, 0, 3);

        if(pix%2 == 0){
        r = 50;
        g = 15;
        b = 2;
        }
      prev[pix][0] = current[pix][0];
      prev[pix][1] = current[pix][1];
      prev[pix][2] = current[pix][2];
      next[pix][0] = r;
      next[pix][1] = g;
      next[pix][2] = b;


    }
