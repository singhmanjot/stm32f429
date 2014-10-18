/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_low_power.h"

void TM_LOWPOWER_SleepUntilInterrupt(uint8_t systick) {
	/* Disable systick */
	if (systick) {
		TM_DELAY_DisableSystick();
	}
	
	/* Wait for interrupt command */
	__WFI();
	
	/* At this stage, MCU will be in sleep mode, and when interrupt wake it up */
	/* it will begin here */
	
	/* Enable systick back */
	if (systick) {
		TM_DELAY_EnableSystick();
	}
}

void TM_LOWPOWER_SleepUntilEvent(void) {
	/* We don't need systick disable, because systick does not make an event */
	
	/* Wait for interrupt */
	__WFE();
}

void TM_LOWPOWER_Standby(void) {
	/* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);
	
	/* Go to standy mode */
	PWR_EnterSTANDBYMode();
}

uint8_t TM_LOWPOWER_StandbyReset(void) {
	/* Check Standby Flag */
	if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET) {
		/* Clear StandBy flag */
		PWR_ClearFlag(PWR_FLAG_SB);
		
		/* Reset was from wakeup from standy */
		return 1;
	}
	/* Reset was not from standby */
	return 0;
}

void TM_LOWPOWER_StopUntilInterrupt(void) {
	/* Go to STOP mode */
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
	
	/* After wakeup, call system init to enable PLL as clock source */
	SystemInit();
}

void TM_LOWPOWER_StopUntilEvent(void) {
	/* Go to STOP mode */
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
	
	/* After wakeup, call system init to enable PLL as clock source */
	SystemInit();
}

extern void TM_LOWPOWER_EnableWakeUpPin(void) {
	/* Enable Wakeup pin, PA0 */
	PWR_WakeUpPinCmd(ENABLE);
}

extern void TM_LOWPOWER_DisableWakeUpPin(void) {
	/* Disable Wakeup pin, PA0 */
	PWR_WakeUpPinCmd(DISABLE);
}
