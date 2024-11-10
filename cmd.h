/**
 * @file log.h
 * @brief Заголовочный файл для работы с командами
 *
 * Этот файл содержит объявление переменных времени и функции для работы с командами.
 *
 * @date 19 августа 2024
 * @author Alex
 */

#ifndef INC_CMD_H_
#define INC_CMD_H_

#include "usart.h"
#include "inttypes.h"

/**
 * @brief Команды для управления устройством.
 */
#define CMD_NA          0      ///< Команда не определена.
#define CMD_EEPROM_TEST 37100  ///< Тестирование EEPROM.
#define CMD_LED_ON      38838  ///< Включение светодиода.
#define CMD_LED_OFF     36276  ///< Выключение светодиода.
#define CMD_READ_LOG    4866   ///< Чтение журнала событий.
#define CMD_OUT_ON      24505  ///< Включение внешнего устройства.
#define CMD_OUT_OFF     22039  ///< Выключение внешнего устройства.
#define CMD_BUTTON      8385   ///< Команда нажатия кнопки.
#define CMD_ALARM       1298   ///< Сигнал тревоги.
#define CMD_INFINITY    41231  ///< Команда бесконечного цикла.

/**
 * @brief Читает команду из входного потока.
 *
 * @return Код команды, если она распознана, или CMD_NA, если команда не определена.
 */
uint16_t Cmd_Read();

#endif /* INC_CMD_H_ */
