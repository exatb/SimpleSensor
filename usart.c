/*
 * usart.c
 *
 *  Created on: Aug 19, 2024
 *      Author: Alex
 */

#include "usart.h"

FIFO_BUFF usart2_tx; ///< Буфер FIFO для передачи данных по USART2.
FIFO_BUFF usart2_rx; ///< Буфер FIFO для приема данных по USART2.

/**
 * @brief Включает прерывание передачи данных для USART2.
 *
 * Активирует передачу данных по USART2 через прерывание.
 */
void USART_Send()
{
    LL_USART_EnableIT_TXE(USART2);
}

/**
 * @brief Записывает один символ в буфер передачи USART2.
 *
 * Добавляет байт данных в буфер `usart2_tx`. Если буфер переполнен, вызывается функция обработки ошибок.
 *
 * @param data Данные (символ) для записи в буфер передачи.
 */
void USART_Write_Char(unsigned char data)
{
	if (FIFO_Write(&usart2_tx,data)!=FIFO_OK)
		Error_Handler();
}

/**
 * @brief Записывает строку в буфер передачи USART2.
 *
 * Последовательно записывает каждый символ строки в буфер передачи `usart2_tx`.
 *
 * @param s Указатель на строку для записи.
 */
void USART_Write_String(unsigned char* s){
	while (*s) USART_Write_Char(*s++);
}

/**
 * @brief Читает один символ из буфера приема USART2.
 *
 * Читает один байт данных из буфера `usart2_rx`. Возвращает `USART_OK` при успешном чтении, или `USART_EMPTY`, если буфер пуст.
 *
 * @param data Указатель на переменную для сохранения прочитанного байта.
 * @return USART_OK, если данные успешно прочитаны. USART_EMPTY, если буфер пуст.
 */
unsigned char USART_Read_Char(unsigned char *data)
{
	if (FIFO_Read(&usart2_rx,data)!=FIFO_EMPTY)
		return USART_OK;
	else
		return USART_EMPTY;
}

/**
 * @brief Обработчик глобального прерывания USART2.
 *
 * Обрабатывает прерывания приема и передачи данных для USART2. При приеме данных записывает их в буфер `usart2_rx`. При передаче данных извлекает их из буфера `usart2_tx` и передает через USART2.
 */
void USART_IRQ(void)
{
	  uint8_t data;

	  if (LL_USART_IsEnabledIT_RXNE(USART2) && LL_USART_IsActiveFlag_RXNE(USART2))
	  {
	    data = LL_USART_ReceiveData8(USART2);

	    if (FIFO_Write(&usart2_rx, data) == FIFO_OVERFLOW)
	    {
	    	Error_Handler();
	    }

	    if (FIFO_Write(&usart2_tx, data) == FIFO_OVERFLOW)
	    {
	    	Error_Handler();
	    }

	  }

	  if (LL_USART_IsEnabledIT_TXE(USART2) && LL_USART_IsActiveFlag_TXE(USART2))
	  {
	    if (FIFO_Read(&usart2_tx, &data) == FIFO_OK)
	    {
	      LL_USART_TransmitData8(USART2, data);
	    }
	    else
	    {
	      LL_USART_DisableIT_TXE(USART2);
	    }
	  }
}

/**
 * @brief Инициализация логики работы с USART.
 *
 * Инициализирует буферы FIFO для передачи и приема данных по USART2.
 *
 * @retval None
 */
void USART_Init(void)
{
  /* Prepare FIFO buffers */
  usart2_tx.head=0;
  usart2_tx.tail=0;
  usart2_rx.head=0;
  usart2_rx.tail=0;
}

