#include "handle_led.h"
#include <Adafruit_NeoPixel.h>
#include "global_defines.h"
#include <Arduino.h>
#include "handle_ota.h"

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


long prev_cycle = 0;
void cycle_leds(){

  Serial.println(1000 / (millis() - prev_cycle));

  prev_cycle = millis();
  calculate_next_step();

  if(current_mode != (int) WARM_WHITE_MODE && current_mode != (int) OTA_MODE ){
    apply_master();
    apply_rgbw_correction();
  }

  if(current_mode == (int) TURBO_MODE){
    calculate_turbo();
  }
  display();

  if(current_mode == (int) OTA_MODE ){
    do_ota(0, false);
  }
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

  if(type == (int) OTA_MODE){
    Serial.println("ota");
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
  int avg = 0;
  if(init_led()){
    Serial.print("brightness: ");
    Serial.println(pixels.getBrightness());
    for(int i = 0; i<240; i++){
      pixels.setPixelColor(i, pixels.Color(to_display_rgbw[i][0], to_display_rgbw[i][1], to_display_rgbw[i][2], to_display_rgbw[i][3]));
      avg += to_display_rgbw[i][0] + to_display_rgbw[i][1] + to_display_rgbw[i][2] + to_display_rgbw[i][3];
    }
    pixels.show();
  }

  Serial.print("AVG: ");
  Serial.println( avg / 960);
}

void calculate_next_step(){

  switch(current_mode){
    case (int) WARM_WHITE_MODE : calculate_warm_white(); break;
    case (int) RGB_MODE : calculate_rgb(); break;
    case (int) FIRE_MODE : calculate_fire(); break;
    case (int) TEMPERATURE_MODE : calculate_temp(); break;
    case (int) STARS_MODE : calculate_stars(); break;
    case (int) OTA_MODE : calculate_warm_white(); set_master(10); break;
    /*case (int) SUNSET_MODE : break;*/
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
    update_interval = random((100 - fire_data[0])*10) + 50;
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

      int plus_g = map(fire_data[1], 0, 100, 35, 0);
      int plus_b = map(fire_data[1], 0, 100, 10, 0);

      int r = map(n, 0, 1023, 0, 130);
      if (n > 200)
        g = map(n, 200, 1023, 0, plus_g);
      // g = map(n * n, 100 * 100, 1023 * 1023, 0, 255);
      if (n > 755)
        b = map(n, 755, 1023, 0, plus_b);

        if(pix%2 == 0){
        r = 50;
        g = 10 + plus_g*0.3;
        b = 2 + plus_b*0.1;
        }
      prev[pix][0] = current[pix][0];
      prev[pix][1] = current[pix][1];
      prev[pix][2] = current[pix][2];
      next[pix][0] = map(r, 0, 130, 0, 255);
      next[pix][1] = map(g, 0, 130, 0, 255);
      next[pix][2] = map(b, 0, 130, 0, 255);


    }


void calculate_temp(){
  float red = 0.0;
  float green = 0.0;
  float blue = 0.0;
  float temp = (float)kelvins;

  red = 255;

  green = temp;
  green = 99.4708025861 * log(green) - 161.1195681661;


  if( temp <= 19){

    blue = 0;

  } else {

    blue = temp-10;
    blue = 138.5177312231 * log(blue) - 305.0447927307;

  }

  if(red < 0){
    red = 0;
  }

  if(red > 255){
    red = 255;
  }

  if(green < 0){
    green = 0;
  }

  if(green > 255){
    green = 255;
  }

  if(blue < 0){
    blue = 0;
  }

  if(blue > 255){
    blue = 255;
  }

  for(int i = 0; i<240; i++){
    raw_rgb[i][0] = red;
    raw_rgb[i][1] = green;
    raw_rgb[i][2] = blue;
  }


}


const int num_stars = 24;
// kje, kok bliz(močno), kok hitr, kera faza
star_data stars[num_stars];

long prev_stars = 0;

bool first_stars = true;

void calculate_stars(){

  for(int i = 0; i<240; i++){
    raw_rgb[i][0] = 0.0;
    raw_rgb[i][1] = 0.0;
    raw_rgb[i][2] = 0.0;
  }

  int calc_num = num_stars * stars_data[0] / 100;

  if(first_stars){
    calc_num = num_stars;
  }

  for(int i = 0; i<calc_num; i++){
    if(stars[i].done == true || first_stars){
      stars[i].done = false;
      stars[i].position = random(240);
      stars[i].distance = 400.0;
      stars[i].initial_distance = stars[i].distance;
      stars[i].speed = random(50 + stars_data[1]) + 10;
      stars[i].strength = random(255);
    }


      for(int j = 0; j<3; j++){

        int rp = j*10;

        float dist_to_star = abs(stars[i].distance)*abs(stars[i].distance);


        float brightness = stars[i].strength - (stars[i].strength * dist_to_star / pow(stars[i].initial_distance, 2)); //  map(dist_to_star, 0, 130, stars[i].strength, 0);// / dist_to_star;
        brightness /= ((j*2)+1);

        if(j == 0){
          raw_rgb[stars[i].position][0] += brightness;
          raw_rgb[stars[i].position][1] += brightness;
          raw_rgb[stars[i].position][2] += brightness;
        } else {

          int pos = stars[i].position + j;
          pos %= 240;

          raw_rgb[pos][0] += brightness;
          raw_rgb[pos][1] += brightness;
          raw_rgb[pos][2] += brightness;

          pos = stars[i].position - j + 240;
          pos %= 240;

          raw_rgb[pos][0] += brightness;
          raw_rgb[pos][1] += brightness;
          raw_rgb[pos][2] += brightness;

        }

        stars[i].distance -= stars[i].speed * (millis() - prev_stars) / 400.0;

        if(stars[i].distance < -400){
          stars[i].done = true;
        }
  }


    }
    prev_stars = millis();
        first_stars = false;

        for(int i = 0; i<240; i++){
          int val = raw_rgb[i][0];
          if(val> 255){
            val = 255.0;
          }
          int calc = map(val*val, 0, 255*255, 0, 255);
            raw_rgb[i][0] = calc;
            raw_rgb[i][1] = calc;
            raw_rgb[i][2] = calc * 0.8;
          }


}




void calculate_turbo(){
  int val = map(master * master, 0, 100 * 100, 0, 220);
  int b = map(master * master, 0, 100 * 100, 0, 200);
  int ww = map(master * master, 0, 100 * 100, 0, 255);
  for(int i = 0; i<240; i++){
    to_display_rgbw[i][0] = (int)val;
    to_display_rgbw[i][1] = (int)val;
    to_display_rgbw[i][2] = (int)b;
    to_display_rgbw[i][3] = (int)ww;
  }
}
