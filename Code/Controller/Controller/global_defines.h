#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H


#define ROTARY_A_PIN     32
#define ROTARY_B_PIN     13
#define SWITCH_PIN       37
#define POWER_PIN        33



enum messages {
  MASTER_MESSAGE,
  OTHER_MESSAGE,
  OFF_MESSAGE
};


enum other_modes {
  WARM_WHITE_MODE,
  RGB_MODE,
  SUNSET_MODE,
  STARS_MODE,
  FIRE_MODE,
  TEMPERATURE_MODE
};



#endif
