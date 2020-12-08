/*
 * battery_armpofo.cpp
 *
 *  Created on: 7 Dec 2020
 *      Author: vitya
 */

#include "battery.h"
#include "hwadc.h"
#include "hwpwm.h"
#include "clockcnt.h"

#define MAX_CHARGE_PWM      0xA000

#define BAT_3V3_REF           3304

#define BAT_CHARGE_ZERO       1825
#define BAT_ISENSE_mOHM        110
#define BAT_ISENSE_AMP          50

#define BAT_CHARGE_FREQ     400000  // 400 kHz
#define BAT_CHARGE_DUTY     0x2000  // 90 mA

THwAdc         g_adc_bat;
THwPwmChannel  g_bat_charge_pwm;

int battery_get_u_5V()      // USB 5V Input in mV
{
	return ((g_adc_bat.ChValue(10) * BAT_3V3_REF * 2) >> 16);
}

int battery_get_u_bat()     // Battery Voltage in mV
{
	return ((g_adc_bat.ChValue(1) * BAT_3V3_REF * 2) >> 16);
}

int battery_get_u_charge_sense()
{
	unsigned uchg = ((g_adc_bat.ChValue(0) * BAT_3V3_REF) >> 16);
	int udiff = int(uchg) - BAT_CHARGE_ZERO;
	return udiff;
}

int battery_get_i_charge()  // Charging / Discharging current in mA
{
	unsigned uchg = ((g_adc_bat.ChValue(0) * BAT_3V3_REF) >> 16);
	int udiff = int(uchg) - BAT_CHARGE_ZERO;

	//return udiff;
	return (udiff * 1000) / (BAT_ISENSE_AMP * BAT_ISENSE_mOHM);
}

void battery_init()
{
	hwpinctrl.PinSetup(PORTNUM_C, 2, PINCFG_ANALOGUE);  // ADC3/INP0:  I-CHARGE, charging / discharging (only on battery) current
	hwpinctrl.PinSetup(PORTNUM_C, 3, PINCFG_ANALOGUE);  // ADC3/INP1:  BATTERY Voltage, divided by 2 with resistors
	hwpinctrl.PinSetup(PORTNUM_C, 0, PINCFG_ANALOGUE);  // ADC3/INP10: USB 5V input sense, divided by 2 with resistors

	g_adc_bat.sampling_time_ns = 1000;
	g_adc_bat.Init(3, (1 << 0) | (1 << 1) | (1 << 10));

	// init the battery charger PWM
	g_bat_charge_pwm.frequency = BAT_CHARGE_FREQ;
	g_bat_charge_pwm.inverted = true;
  g_bat_charge_pwm.Init(8, 2, 0);  // TIM8/CH2
  battery_charge_duty(0);
  g_bat_charge_pwm.Enable();

  // init the pin only after the output was set to high !
	hwpinctrl.PinSetup(PORTNUM_C, 7, PINCFG_OUTPUT | PINCFG_AF_3 | PINCFG_OPENDRAIN);  // TIM8/CH2: Charge PWM
	//battery_charge_duty(0xA000);

	battery_charge_duty(BAT_CHARGE_DUTY); // this is light 70 mA charging
	//battery_charge_duty(0); // this is light 70 mA charging
}

void battery_charge_duty(uint16_t aduty)
{
	if (aduty > MAX_CHARGE_PWM)
	{
		aduty = MAX_CHARGE_PWM;
	}

	// the output is inverted, so 0 = no charge, 0xffff = full on - which would burns the electronic!
  g_bat_charge_pwm.SetOnClocks((g_bat_charge_pwm.periodclocks * aduty) >> 16);
}

void battery_run()
{

}
