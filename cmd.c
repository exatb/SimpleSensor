/*
 * cmd.c
 *
 *  Created on: Aug 21, 2024
 *      Author: Alex
 */

#include "cmd.h"


#define HASH_INIT 5381

volatile uint16_t hash = HASH_INIT;

/**
 * @brief Добавляет данные в хеш с использованием алгоритма DJB2.
 *
 * @param data Данные, которые необходимо добавить в хеш.
 */void Add2Hash(uint16_t data)
{
	//Hash function DJB2 by Dan Bernstein
	hash = (hash << 5) + hash + data;
}

/**
* @brief Сбрасывает значение хеша на инициализирующее.
*
* Сбрасывает значение глобальной переменной hash на значение HASH_INIT.
*/
static inline void Reset_Hash()
{
	hash = HASH_INIT;
}

/**
 * @brief Читает данные из USART и обновляет хеш, пока не будет получен символ '\r'.
 *
 * Если получен символ '\r', возвращает текущее значение хеша.
 * Если получен символ '\n', сбрасывает хеш.
 * В случае, если данных нет или они некорректны, возвращает CMD_NA.
 *
 * @return Значение хеша, если получен символ '\r', иначе CMD_NA.
 */
uint16_t Cmd_Read()
{
	unsigned char data;
	while (USART_Read_Char(&data)!=USART_EMPTY)
	{
		if (data=='\r') return hash;
		else
			Add2Hash(data);

		if (data=='\n') Reset_Hash();

		return CMD_NA;
	}
	return CMD_NA;
}






