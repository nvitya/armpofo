/*
 * systimer.h
 *
 *  Created on: 15 Nov 2020
 *      Author: vitya
 */

#ifndef SRC_SYSTIMER_H_
#define SRC_SYSTIMER_H_

#include "stdint.h"

extern volatile uint32_t g_sysms;

void systimer_init();

#endif /* SRC_SYSTIMER_H_ */
