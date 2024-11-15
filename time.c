/*
 * time.c
 *
 *  Created on: Aug 19, 2024
 *      Author: Alex
 */

#include "time.h"

uint16_t time_msec; ///< Глобальная переменная для хранения времени в миллисекундах.
uint16_t time_sec;	///< Глобальная переменная для хранения времени в секундах.

/**
 * @brief Добавляет 1 миллисекунду к глобальному времени.
 *
 * Увеличивает переменную `time_msec` на 1. Если она достигает 1000, то сбрасывает ее в 0 и увеличивает значение `time_sec` на 1, что эквивалентно переходу на следующую секунду.
 */
void TimeShift1ms(void)
{
  time_msec++;
  if (time_msec>=1000)
  {
	  time_msec=0;
	  time_sec++;
  }
}
