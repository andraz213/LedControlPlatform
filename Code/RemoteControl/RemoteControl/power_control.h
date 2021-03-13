#ifndef POWER_CONTROL_H
#define POWER_CONTROL_H

#define BUTTON_PIN_BITMASK 0x2300000000


void configure_power_vontrol();

void configure_power_off();

void configure_power_on();

void handle_idling();


long time_since_wakeup();

void update_last_input();


#endif
