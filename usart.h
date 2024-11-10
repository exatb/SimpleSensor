/**
 * @file usart.h
 * @brief Заголовочный файл для работы с USART.
 *
 * Этот файл содержит объявления функций и макросов для управления работой USART, включая отправку и прием данных, а также обработку прерываний.
 *
 * @date 19 августа 2024
 * @author Alex
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include "fifo.h"
#include "main.h"

#define USART_OK 0 			///< Код успешного выполнения операций с USART.
#define USART_EMPTY 1		///< Код, означающий, что буфер приема USART пуст.

/**
 * @name Экспортируемые функции для работы с USART
 * @{
 */
void USART_Send();
void USART_Write_Char(unsigned char data);
void USART_Write_String(unsigned char* s);
unsigned char USART_Read_Char(unsigned char *data);
void USART_IRQ(void);
void USART_Init(void);
/** @} */

/**
 * @brief Обработчик ошибок.
 *
 * Эта функция должна быть реализована пользователем для обработки ошибок.
 */
void Error_Handler(void);

#endif /* INC_USART_H_ */
