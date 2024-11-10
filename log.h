/**
 * @file log.h
 * @brief Заголовочный файл для работы с EEPROM и протоколом событий.
 *
 * Этот файл содержит объявление переменных времени и функции для работы с EEPROM и протоколом событий.
 *
 * @date 19 августа 2024
 * @author Alex
 */

#ifndef INC_LOG_H_
#define INC_LOG_H_

#include "main.h"

/**
 * @brief Структура для хранения событий.
 */
#pragma pack(push, 1)
typedef struct EVENT{
	uint16_t time; ///< Время события.
	uint16_t data; ///< Данные события.
}EVENT;
#pragma pack(pop)

#define MAX_EVENTS 8 ///< Максимальное количество событий.
#define MAX_EVENT_ADDR (MAX_EVENTS*sizeof(EVENT) - 1) ///< Максимальный адрес для хранения событий.

/**
 * @name Экспортируемые функции для работы с журналом событий
 * @{
 */
void Write_Event(uint16_t data);
EVENT Read_Event();
/** @} */

/**
 * @name Экспортируемые функции для работы с EEPROM
 * @{
 */
HAL_StatusTypeDef Write_EEPROM(uint32_t address, uint32_t data);
uint32_t Read_EEPROM (uint32_t address);
/** @} */

/**
 * @brief Обработчик ошибок (реализуется пользователем).
 */
void Error_Handler(void);

#endif /* INC_LOG_H_ */
