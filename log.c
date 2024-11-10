/*
 * log.c
 *
 *  Created on: Aug 19, 2024
 *      Author: Alex
 */

#include "log.h"
#include "time.h"

#ifndef __STM32L0xx_HAL_H
/// Эмуляция EEPROM в ОЗУ, если МК не из серии STM32L0xx.
static uint8_t EEPROM_EMU[MAX_EVENTS*sizeof(EVENT)];
#endif

/**
 * @brief Записывает данные во флэш-память EEPROM.
 *
 * Если микроконтроллер из серии STM32L0xx, данные записываются в реальную EEPROM память.
 * В противном случае, данные записываются в эмуляцию EEPROM в ОЗУ.
 *
 * @param address Адрес в EEPROM, по которому нужно записать данные.
 * @param data Данные для записи.
 *
 * @return HAL_OK при успешной записи или код ошибки HAL в случае неудачи.
 */
HAL_StatusTypeDef Write_EEPROM(uint32_t address, uint32_t data)
{
#ifdef __STM32L0xx_HAL_H
	HAL_StatusTypeDef status = HAL_FLASHEx_DATAEEPROM_Unlock();
	if (status == HAL_OK)
	{
		status = HAL_FLASHEx_DATAEEPROM_Erase (address + DATA_EEPROM_BASE);
	}
	if (status == HAL_OK)
	{
		status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, address + DATA_EEPROM_BASE, data);
	}
	HAL_FLASHEx_DATAEEPROM_Lock();

	return status;
#else
	*(uint32_t*)(&EEPROM_EMU[address])=data;
	return HAL_OK;
#endif
}

/**
 * @brief Читает данные из флэш-памяти EEPROM.
 *
 * Если микроконтроллер из серии STM32L0xx, данные считываются из реальной EEPROM памяти.
 * В противном случае, данные считываются из эмуляции EEPROM в ОЗУ.
 *
 * @param address Адрес в EEPROM, с которого нужно прочитать данные.
 *
 * @return Прочитанные данные.
 */
uint32_t Read_EEPROM (uint32_t address)
{
#ifdef __STM32L0xx_HAL_H
	return (*(uint32_t *)(address + DATA_EEPROM_BASE));
#else
	return *(uint32_t*)(&EEPROM_EMU[address]);
#endif
}

volatile uint32_t event_head = 0; ///< Глобальная переменная для хранения указателя на начало очереди событий.
volatile uint32_t event_tail = 0; ///< Глобальная переменная для хранения указателя на конец очереди событий.

/**
 * @brief Объединение структуры EVENT с 32-битным целым числом.
 */
typedef union EVENT32{
	uint32_t ui32;	///< 32-битное представление события.
	EVENT event;	///< Структура события.
}EVENT32;

/**
 * @brief Записывает событие в EEPROM.
 *
 * Создает событие с заданными данными и текущим временем, после чего записывает его в EEPROM.
 * При переполнении происходит циклическая перезапись событий.
 * При ошибке записи вызывает Error_Handler.
 *
 * @param data Данные события для записи.
 */
void Write_Event(uint16_t data)
{
	EVENT32 ev;
	ev.event.data = data;
	ev.event.time = time_sec;
	uint32_t tmp = event_head+4;
	if (tmp>=MAX_EVENT_ADDR) tmp=0;

	if (Write_EEPROM(event_head, ev.ui32)!=HAL_OK)
		Error_Handler();
	else
		event_head = tmp;
}

/**
 * @brief Читает событие из EEPROM.
 *
 * Считывает событие из EEPROM и обновляет указатель на конец очереди событий.
 *
 * @return Прочитанное событие.
 */
EVENT Read_Event()
{
	EVENT32 ev;

	ev.ui32 = Read_EEPROM(event_tail);

	uint32_t tmp = event_tail+4;
	if (tmp>=MAX_EVENT_ADDR) tmp=0;
	event_tail = tmp;

	return ev.event;
}


