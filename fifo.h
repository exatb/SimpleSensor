/**
 * @file fifo.h
 * @brief Заголовочный файл для работы с FIFO.
 *
 * Этот файл содержит объявление переменных времени и функции для работы с FIFO.
 *
 * @date 19 августа 2024
 * @author Alex
 */

#ifndef INC_FIFO_H_
#define INC_FIFO_H_

#define FIFO_SIZE 64 	///< Размер буфера FIFO в байтах.
#define FIFO_MASK 63 	///< Маска для обеспечения циклического перемещения указателей в буфере FIFO.
#define FIFO_OK 0		///< Операция завершена успешно.
#define FIFO_EMPTY 1	///< Буфер FIFO пуст.
#define FIFO_OVERFLOW 2 ///< Буфер FIFO переполнен.

/**
 * @brief Структура для работы с буфером FIFO.
 */
typedef struct FIFO_BUFF{
	unsigned char buff[FIFO_SIZE]; 	///< Массив для хранения данных в буфере FIFO.
	unsigned char head; 			///< Указатель на голову буфера FIFO.
	unsigned char tail;				///< Указатель на хвост буфера FIFO.
} FIFO_BUFF;

/**
 * @name Экспортируемые функции для работы с буфером FIFO
 * @{
 */
unsigned char FIFO_Read(FIFO_BUFF* buff,unsigned char *data);
unsigned char FIFO_Write(FIFO_BUFF* buff,unsigned char data);
/** @} */

#endif /* INC_FIFO_H_ */
