
#include "WiFi.h"

#include "global_defines.h"


#include "handle_wifi.h"



void setup(){

  setCpuFrequencyMhz(240);
  Serial.begin(115200);
  Serial.println("LED CONTROL");


  init_wifi();

}

long prev = 0;
void loop(){

delay(5);


delay(5);

}
