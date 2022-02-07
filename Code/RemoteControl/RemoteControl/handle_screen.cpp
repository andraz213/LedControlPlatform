#include "handle_screen.h"

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(135, 240);

bool inited = false;


void init_screen() {
  if (!inited) {
    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    inited = true;
  }
}


void turn_off_screen() {

  tft.writecommand(0x10); // Sleep (backlight still on ...)
  digitalWrite(TFT_BL, LOW);
  delay(5); // needed!
}

void turn_on_screen() {
  tft.writecommand(0x11); // WAKEUP
  delay(120); // needed! PWR neeeds to stabilize!
  digitalWrite(TFT_BL, HIGH);
}



void draw_splash() {
  init_screen();
}

void draw_menu(String names[], int num, int selected, int battery_percentage) {
  init_screen();

  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);


  for (int i = 0; i < num; i++) {
    tft.drawString(names[i], 4, (i * 23) + 10);
    int w = tft.textWidth(names[i]) + 4;
    int h = tft.fontHeight() + 4;
    tft.drawRect(0, (i * 23) + 8, w, h, tft.color24to16((uint32_t)0x000));
  }

  int w = tft.textWidth(names[selected]) + 4;
  int h = tft.fontHeight() + 4;

  tft.drawRect(0, (selected * 23) + 8, w, h, tft.color24to16((uint32_t)0x00FFAACC));







}

void draw_master(int value) {
  init_screen();


  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);

  int h = 20;
  int w = 135;
  tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00FFFFFF));
  tft.fillRect(1, 101, map(value, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
  tft.fillRect(map(value, 0, 100, 0, 134) + 1, 101, 133 - map(value, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));


}

void draw_RGB(int r, int g, int b, int selected, bool changing) {
  init_screen();

  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);

  tft.drawString("Back", 4, 4);



  int w = tft.textWidth("Back") + 4;
  int h = tft.fontHeight() + 4;
  if (selected == 0) {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00FFAACC));
  } else {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00000));
  }

  tft.setTextSize(1);

  String colors = "(" + String(r) + ", " + String(g) + ", " + String(b) + ")";

  tft.drawString(colors, 4, 220);

  tft.setTextDatum(TL_DATUM);

  uint32_t col = 0;

  col += b;
  col += g * 256;
  col += r * 256 * 256;

  uint32_t col_inv = 0;

  col_inv += 255 - b;
  col_inv += (255 - g) * 256;
  col_inv += (255 - r) * 256 * 256;

  Serial.println(col);
  tft.drawRect(68 - 26, 34, 52, 52, tft.color24to16((uint32_t)col_inv));
  tft.fillRect(68 - 25, 35, 50, 50, tft.color24to16((uint32_t)col));


  for (int i = 0; i < 135; i++) {
    int mapped = map(i, 0, 134, 0, 255);

    uint32_t col = 0;

    col += b;
    col += g * 256;
    col += mapped * 256 * 256;

    tft.drawLine(i, 90, i, 110, tft.color24to16((uint32_t)col));

    /*for(int j = 0; j < 21; j++){
      tft.drawPixel(i, 90 + j, tft.color24to16((uint32_t)col));
      }*/


    col = 0;

    col += b;
    col += mapped * 256;
    col += r * 256 * 256;

    tft.drawLine(i, 130, i, 150, tft.color24to16((uint32_t)col));

    /*for(int j = 0; j < 21; j++){
      tft.drawPixel(i, 130 + j, tft.color24to16((uint32_t)col));
      }*/

    col = 0;

    col += mapped;
    col += g * 256;
    col += r * 256 * 256;

    tft.drawLine(i, 170, i, 190, tft.color24to16((uint32_t)col));

    /*for(int j = 0; j < 21; j++){
      tft.drawPixel(i, 170 + j, tft.color24to16((uint32_t)col));
      }*/


  }

  int mapped_r = map(r, 0, 255, 0, 134);
  int mapped_g = map(g, 0, 255, 0, 134);
  int mapped_b = map(b, 0, 255, 0, 134);
  tft.drawLine(mapped_r, 105, mapped_r, 110, tft.color24to16((uint32_t)0xFFFFFF));
  tft.drawLine(mapped_g, 145, mapped_g, 150, tft.color24to16((uint32_t)0xFFFFFF));
  tft.drawLine(mapped_b, 185, mapped_b, 190, tft.color24to16((uint32_t)0xFFFFFF));

  for (int j = 0; j < 5; j++) {

    tft.drawLine(0, 112 + j, 134, 112 + j, tft.color24to16((uint32_t)0x00000));

    tft.drawLine(0, 152 + j, 134, 152 + j, tft.color24to16((uint32_t)0x00000));

    tft.drawLine(0, 192 + j, 134, 192 + j, tft.color24to16((uint32_t)0x00000));
  }

  if (selected == 1) {
    for (int j = 0; j < 5; j++) {
      if (changing) {
        tft.drawLine(0, 112 + j, 134, 112 + j, tft.color24to16((uint32_t)0x00FFAACC));
      } else {
        tft.drawLine(0, 112 + j, 134, 112 + j, tft.color24to16((uint32_t)0x00666666));
      }
    }
  }

  if (selected == 2) {
    for (int j = 0; j < 5; j++) {
      if (changing) {
        tft.drawLine(0, 152 + j, 134, 152 + j, tft.color24to16((uint32_t)0x00FFAACC));
      } else {
        tft.drawLine(0, 152 + j, 134, 152 + j, tft.color24to16((uint32_t)0x00666666));
      }
    }
  }

  if (selected == 3) {
    for (int j = 0; j < 5; j++) {
      if (changing) {
        tft.drawLine(0, 192 + j, 134, 192 + j, tft.color24to16((uint32_t)0x00FFAACC));
      } else {
        tft.drawLine(0, 192 + j, 134, 192 + j, tft.color24to16((uint32_t)0x00666666));
      }
    }
  }

}

void draw_fireplace(int param1, int param2, int selected, bool changing) {
  init_screen();


  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);



  tft.setTextSize(2);

  tft.drawString("Back", 4, 4);

  tft.drawString("FIREPLACE", 4, 35);


  int w = tft.textWidth("Back") + 4;
  int h = tft.fontHeight() + 4;
  if (selected == 0) {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00FFAACC));
  } else {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00000));
  }


  h = 20;
  w = 135;

  tft.drawString("Intensity", 4, 77);
  tft.drawString("Phase", 4, 150 - 23);

  tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00111111));
  tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00111111));
  tft.fillRect(1, 101, map(param1, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
  tft.fillRect(map(param1, 0, 100, 0, 134) + 1, 101, 133 - map(param1, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));


  tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00111111));
  tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00111111));
  tft.fillRect(1, 161, map(param2, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
  tft.fillRect(map(param2, 0, 100, 0, 134) + 1, 161, 133 - map(param2, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));

  if (selected == 1) {
    if (changing) {
      tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00FFAACC));
      tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00FFAACC));
    } else {
      tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00666666));
      tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00666666));
    }

  }

  if (selected == 2) {
    if (changing) {
      tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00FFAACC));
      tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00FFAACC));
    } else {
      tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00666666));
      tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00666666));
    }

  }

}

void draw_sunset(int param1, int param2, int selected, bool changing) {
  init_screen();




  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);



  tft.setTextSize(2);

  tft.drawString("Back", 4, 4);

  tft.drawString("SUNSET", 4, 35);


  int w = tft.textWidth("Back") + 4;
  int h = tft.fontHeight() + 4;
  if (selected == 0) {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00FFAACC));
  } else {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00000));
  }


  h = 20;
  w = 135;

  tft.drawString("Time delay", 4, 77);
  tft.drawString("Phase", 4, 150 - 23);

  tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00111111));
  tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00111111));
  tft.drawString(String(param1) + " min", 4, 101);

  tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00111111));
  tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00111111));
  tft.fillRect(1, 161, map(param2, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
  tft.fillRect(map(param2, 0, 100, 0, 134) + 1, 161, 133 - map(param2, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));

  if (selected == 1) {
    if (changing) {
      tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00FFAACC));
      tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00FFAACC));
    } else {
      tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00666666));
      tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00666666));
    }

  }

  if (selected == 2) {
    if (changing) {
      tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00FFAACC));
      tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00FFAACC));
    } else {
      tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00666666));
      tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00666666));
    }

  }





}

void draw_starrynight(int param1, int param2, int selected, bool changing) {
  init_screen();




  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);



  tft.setTextSize(2);

  tft.drawString("Back", 4, 4);

  tft.drawString("STARS ", 4, 35);


  int w = tft.textWidth("Back") + 4;
  int h = tft.fontHeight() + 4;
  if (selected == 0) {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00FFAACC));
  } else {
    tft.drawRect(2, 2, w, h, tft.color24to16((uint32_t)0x00000));
  }


  h = 20;
  w = 135;

  tft.drawString("Density", 4, 77);
  tft.drawString("Size", 4, 150 - 23);

  tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00111111));
  tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00111111));
  tft.fillRect(1, 101, map(param1, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
  tft.fillRect(map(param1, 0, 100, 0, 134) + 1, 101, 133 - map(param1, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));


  tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00111111));
  tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00111111));
  tft.fillRect(1, 161, map(param2, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
  tft.fillRect(map(param2, 0, 100, 0, 134) + 1, 161, 133 - map(param2, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));

  if (selected == 1) {
    if (changing) {
      tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00FFAACC));
      tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00FFAACC));
    } else {
      tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00666666));
      tft.drawRect(0, 99, w + 2, h, tft.color24to16((uint32_t)0x00666666));
    }

  }

  if (selected == 2) {
    if (changing) {
      tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00FFAACC));
      tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00FFAACC));
    } else {
      tft.drawRect(0, 160, w, h, tft.color24to16((uint32_t)0x00666666));
      tft.drawRect(0, 159, w + 2, h, tft.color24to16((uint32_t)0x00666666));
    }

  }


}


void draw_turbo() {


  init_screen();


  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);

  tft.drawString("Back", 4, 4);

  tft.drawString("TURBO ACTIVE", 4, 50);

  tft.setTextSize(1);
  tft.drawString("Ne met predolg przgan,", 4, 80);
  tft.drawString("ker je to turbo in je ", 4, 90);
  tft.drawString("res sam premocno", 4, 100);
}


void draw_kelvins(int kelvins) {



  init_screen();


  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);

  int h = 20;
  int w = 135;
  tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00FFFFFF));
  tft.fillRect(1, 101, map(kelvins, 20, 60, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
  tft.fillRect(map(kelvins, 20, 60, 0, 134) + 1, 101, 133 - map(kelvins, 20, 60, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));


  tft.drawString(String(kelvins * 100) + "K", 4, 77);



}

void draw_OTA(int phase, String data, int perc) {
  init_screen();
  //tft.fillScreen(TFT_BLACK);
  tft.setRotation(2);
  //tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);

  // connecting to wifi
  if (phase == 0) {
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Back", 4, 4);
    tft.drawString("Connecting to WiFi", 4, 105);
    return;
  }

  if (phase == 1) {
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Back", 4, 4);
    tft.drawString("Connected to WiFi", 4, 105);
    tft.drawString("IP:" + data, 4, 120);
    return;

  }

  tft.drawString("Back", 4, 4);

  if (phase == 2) {

    tft.fillScreen(TFT_BLACK);
    tft.drawString(data, 0, 105);
    return;
  }

  if (phase == 3) {

    tft.drawString(data, 0, 85);

    int h = 20;
    int w = 135;
    tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00FFFFFF));
    tft.fillRect(1, 101, map(perc, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
    tft.fillRect(map(perc, 0, 100, 0, 134) + 1, 101, 133 - map(perc, 0, 100, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));


    return;
  }

  /*
    int h = 20;
    int w = 135;
    tft.drawRect(0, 100, w, h, tft.color24to16((uint32_t)0x00FFFFFF));
    tft.fillRect(1, 101, map(kelvins, 20, 60, 0, 134), h - 2, tft.color24to16((uint32_t)0x0088AAFF));
    tft.fillRect(map(kelvins, 20, 60, 0, 134) + 1, 101, 133 - map(kelvins, 20, 60, 0, 134), h - 2, tft.color24to16((uint32_t)0x0000));


  */



}


void blackout_screen() {

  init_screen();

  tft.fillScreen(TFT_BLACK);

}
