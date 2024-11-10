/**
 * @file time.h
 * @brief Заголовочный файл для работы с системным временем.
 *
 * Этот файл содержит объявление переменных времени и функции для работы с системным временем.
 *
 * @date 19 августа 2024
 * @author Alex
 */

#ifndef INC_TIME_H_
#define INC_TIME_H_

#include "main.h"
#include "inttypes.h"

extern uint16_t time_msec;	///< Глобальная переменная для хранения миллисекунд.
extern uint16_t time_sec; 	///< Глобальная переменная для хранения секунд.

/**
 * @brief Добавляет 1 миллисекунду к системному времени.
 *
 */
void TimeShift1ms(void);

#endif /* INC_TIME_H_ */
