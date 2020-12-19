/*
 * powersave.h
 *
 *  Created on: 6 Dec 2020
 *      Author: vitya
 */

#ifndef SRC_POWERSAVE_H_
#define SRC_POWERSAVE_H_

#define POWERSAVE_WAKEUP_PERIOD_MS  250

void cpu_soft_reset();

void rtc_start();

void enter_low_power();
void standby_wakeup_test();

#endif /* SRC_POWERSAVE_H_ */
