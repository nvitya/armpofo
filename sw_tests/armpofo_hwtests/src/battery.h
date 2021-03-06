/*
 * battery.h
 *
 *  Created on: 7 Dec 2020
 *      Author: vitya
 */

#ifndef SRC_BATTERY_H_
#define SRC_BATTERY_H_

#include "stdint.h"

void battery_init();

void battery_run();

int battery_get_u_5V();      // USB 5V Input in mV
int battery_get_u_bat();     // Battery Voltage in mV
int battery_get_i_charge();  // Charging / Discharging current in mA
int battery_get_u_charge_sense();

void battery_set_charge_percent(int apercent);
int battery_get_charge_percent();

#endif /* SRC_BATTERY_H_ */
