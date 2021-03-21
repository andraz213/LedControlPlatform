#include "ui_handle.h"
#include <Arduino.h>
#include "global_defines.h"
#include "handle_input.h"
#include "handle_wifi.h"

#include "handle_screen.h"

int current_scene = 0;

int selected_menu_item = 0;

const int num_items = 6;

bool first_time = true;

int master_value = 5;


int inside_menu_item = 0;
bool selecting_menu_item = false;


int rgb_data [3];
int fire_data [2];
int stars_data [2];
int sunset_data [2];

menu_item menuItems[num_items] = {
  {"Master", &handle_RGB},
  {"RGB",  &handle_RGB},
  {"Fire",  &handle_fire},
  {"Sunset",  &handle_sunset},
  {"Stars",  &handle_stars},
  {"OTA",  &handle_ota}
};


void change_scene(int scene){
  blackout_screen();
  current_scene = scene;
  check_switch();
  get_rotary();
  first_time = true;
  delay(10);
  check_switch();
  get_rotary();
  first_time = true;
  blackout_screen();
}

void open_menu(){
  change_scene((int)MENU);
  selected_menu_item = 0;
  Serial.println("em v meniju");
}







void handle_ui(){

  //Serial.println(current_scene);

  switch(current_scene){
    case MASTER_SET : master_set_handler(); break;
    case MENU : menu_handler(); break;
    case MENU_ITEM : handle_menu_item(); break;

  }
}








void menu_handler(){
  Serial.println("sm v menu");
  if((menu_input_handle() || first_time) && current_scene == (int) MENU){
      setCpuFrequencyMhz(240);
    if(first_time){
      blackout_screen();
      first_time = false;
    }
    Serial.println("change");
    String names[num_items];

    for(int i = 0; i< num_items; i++){
      names[i] = String(menuItems[i].display_name);
    }

    draw_menu(names, num_items, selected_menu_item, 0);
  }


}





bool menu_input_handle(){

  bool changed = false;

  if(check_switch()){
    if(selected_menu_item == MASTER_SET){
      change_scene((int) MASTER_SET);
    } else {
      inside_menu_item = 0;
      selecting_menu_item = 0;
      change_scene((int) MENU_ITEM);
    }
    //current_scene = MENU_ITEM;
    return false;
  }

  int rotary_num = get_rotary();

  if(rotary_num > 0){
    selected_menu_item ++;
    changed = true;
  }

  if(rotary_num < 0){
    selected_menu_item += num_items;
    selected_menu_item --;
    changed = true;
  }

  selected_menu_item %= num_items;

  return changed;

}





void master_set_handler(){

int value_got = master_set_input_handle();
if((value_got != 0 || first_time)  && current_scene == (int) MASTER_SET){
    setCpuFrequencyMhz(240);
  if(first_time){
    blackout_screen();
  }

  first_time = false;
  master_value += value_got;

  if(master_value > 100){
    master_value = 100;
  }

  if(master_value < 0){
    master_value = 0;
  }


  draw_master(master_value);

  send_master(master_value);

}





}



int master_set_input_handle(){

  bool changed = false;

  if(check_switch()){
    change_scene((int) MENU);
    return 0;
  }

  int rotary_num = get_rotary();
  return rotary_num;

}






void handle_menu_item(){

  bool sw = check_switch();
  int rot = get_rotary();


  if(sw || rot != 0 || first_time){
      setCpuFrequencyMhz(240);
    if(first_time){
      blackout_screen();
      rot = 1;
      first_time = false;
    }

    Serial.println("lalala ose kul");
    (*menuItems[selected_menu_item].handler)(rot, sw);

  }

}


void handle_RGB(int rot, bool sw){
  Serial.println("v RGB sem hjo");
  if(rot != 0 || sw){
    Serial.println("v RGB sem hsdjo");

    if(inside_menu_item == 0 && sw){
      change_scene((int) MENU);
      return;
    }

    if(sw){
      bool kop = !selecting_menu_item;
      selecting_menu_item = kop;
    }

    Serial.println(sw);

    if(rot != 0 && inside_menu_item > 0 && inside_menu_item < 5 && selecting_menu_item){
      rgb_data[inside_menu_item - 1] += rot;

      if(rgb_data[inside_menu_item - 1] > 255){
        rgb_data[inside_menu_item - 1] = 255;
      }

      if(rgb_data[inside_menu_item - 1] < 0){
        rgb_data[inside_menu_item - 1] = 0;
      }

    }

    if(!selecting_menu_item && rot != 0){
      inside_menu_item += rot;
      inside_menu_item += 4;
      inside_menu_item %= 4;
    }


    draw_RGB(rgb_data[0], rgb_data[1], rgb_data[2], inside_menu_item, selecting_menu_item);

    int rgb_data_to_send [3];

    int max = 0;
    float fact = 0.0;

    for(int i = 0; i<3; i++){
      if(rgb_data[i] > max){
        max = rgb_data[i];
      }
    }

    fact = (float) 255.0 / (float) max;

    for(int i = 0; i<3; i++){
      rgb_data_to_send[i] = (int)((float)rgb_data[i] * fact);
    }

    master_value = (int)floor(((float)max / 255.0)*100.0);

    Serial.println(master_value);
    send_other((int) RGB_MODE, rgb_data, 3);
    send_master(master_value);


  }

}



void handle_fire(int rot, bool sw){


  Serial.println("v fireplace sem hjo");
  if(rot != 0 || sw){
    Serial.println("v fireplace sem hsdjo");

    if(inside_menu_item == 0 && sw){
      change_scene((int) MENU);
      return;
    }

    if(sw){
      bool kop = !selecting_menu_item;
      selecting_menu_item = kop;
    }

    Serial.println(sw);

    if(rot != 0 && inside_menu_item > 0 && inside_menu_item < 4 && selecting_menu_item){
      fire_data[inside_menu_item - 1] += rot;

      if(fire_data[inside_menu_item - 1] > 100){
        fire_data[inside_menu_item - 1] = 100;
      }

      if(fire_data[inside_menu_item - 1] < 0){
        fire_data[inside_menu_item - 1] = 0;
      }

    }

    if(!selecting_menu_item && rot != 0){
      inside_menu_item += rot;
      inside_menu_item += 3;
      inside_menu_item %= 3;
    }

      draw_fireplace(fire_data[0], fire_data[1], inside_menu_item, selecting_menu_item);
      send_other((int) FIRE_MODE, fire_data, 2);
      master_value = 50;
      send_master(master_value);
  }


}


void handle_stars(int rot, bool sw){


  Serial.println("v stars sem hjo");
  if(rot != 0 || sw){
    Serial.println("v fireplace sem hsdjo");

    if(inside_menu_item == 0 && sw){
      change_scene((int) MENU);
      return;
    }

    if(sw){
      bool kop = !selecting_menu_item;
      selecting_menu_item = kop;
    }

    Serial.println(sw);

    if(rot != 0 && inside_menu_item > 0 && inside_menu_item < 4 && selecting_menu_item){
      stars_data[inside_menu_item - 1] += rot;

      if(  stars_data[inside_menu_item - 1] > 100){
          stars_data[inside_menu_item - 1] = 100;
      }

      if(  stars_data[inside_menu_item - 1] < 0){
          stars_data[inside_menu_item - 1] = 0;
      }
    }

    if(!selecting_menu_item && rot != 0){
      inside_menu_item += rot;
      inside_menu_item += 3;
      inside_menu_item %= 3;
    }

      draw_starrynight(stars_data[0], stars_data[1], inside_menu_item, selecting_menu_item);
      send_other((int) STARS_MODE, stars_data, 2);
      master_value = 50;
      send_master(master_value);
  }


}

void handle_sunset(int rot, bool sw){

  Serial.println("v sunset sem hjo");
  if(rot != 0 || sw){
    Serial.println("v sunset sem hsdjo");

    if(inside_menu_item == 0 && sw){
      change_scene((int) MENU);
      return;
    }

    if(sw){
      bool kop = !selecting_menu_item;
      selecting_menu_item = kop;
    }

    Serial.println(sw);

    if(rot != 0 && inside_menu_item > 0 && inside_menu_item < 4 && selecting_menu_item){
      sunset_data[inside_menu_item - 1] += rot;

      if(fire_data[inside_menu_item - 1] > 100){
        sunset_data[inside_menu_item - 1] = 100;
      }

      if(fire_data[inside_menu_item - 1] < 0){
        sunset_data[inside_menu_item - 1] = 0;
      }
    }

    if(!selecting_menu_item && rot != 0){
      inside_menu_item += rot;
      inside_menu_item += 3;
      inside_menu_item %= 3;
    }

      draw_sunset(sunset_data[0], sunset_data[1], inside_menu_item, selecting_menu_item);
      send_other((int) SUNSET_MODE, sunset_data, 2);
      master_value = 50;
      send_master(master_value);
  }


}

void handle_ota(int rot, bool sw){
  change_scene((int) MENU);
}
