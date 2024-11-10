/*
 * fifo.c
 *
 *  Created on: 2 дек. 2022 г.
 *      Author: Alex
 */

#include "fifo.h"

/**
 * @brief Записывает данные в буфер FIFO.
 *
 * Записывает один байт данных в буфер FIFO, если в нем есть свободное место.
 * В случае успеха возвращает FIFO_OK, в случае переполнения - FIFO_OVERFLOW.
 *
 * @param buff Указатель на структуру буфера FIFO.
 * @param data Данные, которые нужно записать в буфер.
 *
 * @return FIFO_OK, если запись прошла успешно. FIFO_OVERFLOW, если буфер переполнен.
 */
unsigned char FIFO_Write(FIFO_BUFF* buff,unsigned char data)
{
	unsigned char tmp = (buff->head+1)&FIFO_MASK;
	if (tmp!=buff->tail)
	{
		buff->buff[tmp]=data;
		buff->head=tmp;
		return FIFO_OK;
	}else
		return FIFO_OVERFLOW;
}

/**
 * @brief Читает данные из буфера FIFO.
 *
 * Читает один байт данных из буфера FIFO, если в нем есть данные.
 * В случае успеха возвращает FIFO_OK, а также данные через указатель на переменную.
 * Если буфер пуст, возвращает FIFO_EMPTY.
 *
 * @param buff Указатель на структуру буфера FIFO.
 * @param data Указатель на переменную, в которую будут записаны прочитанные данные.
 *
 * @return FIFO_OK, если чтение прошло успешно. FIFO_EMPTY, если буфер пуст.
 */
unsigned char FIFO_Read(FIFO_BUFF* buff,unsigned char *data)
{
	if (buff->head!=buff->tail)
	{
		buff->tail=(buff->tail+1)&FIFO_MASK;
		*data=buff->buff[buff->tail];
		return FIFO_OK;
	}else
		return FIFO_EMPTY;
}



