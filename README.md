# SimpleSensor
Набор функций, реализующих базовый функционал датчика пламени. 

Этот проект предназначен для обучения базовым принципам разработки пожарных и охранных датчиков. 

Работает на базе отладочных плат **NUCLEO-L053R8** и **NUCLEO-F072RB** с подключенным модулем **KY-026**. 

Включает общение по последовательному порту в режиме текстовых команд и протоколирование событий в энергонезависимой памяти. 

![Flame](https://github.com/user-attachments/assets/0cf7b1e0-17c8-406d-ba7e-4b432125cfc6)

Далее будет пошагово описано как сгенерировать проект в **STM32CubeMX** и добавить в него весь необходимый функционал.

## Сборка пустого проекта в STM32CubeMX
Необходимо создать новый проект на базе отладочной платы **NUCLEO-L053R8** или **NUCLEO-F072RB**. 

Запускаем программу **STM32CubeMX**. 

**STM32CubeMX** является визуальным графическим редактором для конфигурирования микроконтроллеров семейства STM32, позволяющим генерировать код на основе языка Си. 

https://www.st.com/en/development-tools/stm32cubemx.html

Для создания проекта выбираем **File→New Project**, или воспользуемся горячей клавишей Ctr+N.

Выбираем **Board Selector**. В строке **Commercial Part Number** (находится в левой части окна раздела **Board Selector**) вводим название отладочной платы — **NUCLEO-L053R8** или **NUCLEO-F072RB**. Из списка **Boadrs List** (располагается в центре окна) выбираем соотвествующую отладочную плату и нажимаем кнопку **Start Project**. Откроется окно **Board Project Options**. Нажимаем кнопку **Yes** (инициализируем всю периферию по умолчанию).

Для **USART2** (вкладка **Connectivity**) в **NVIC Settings** включаем **USART2 global interrupt**. На вкладке **Advanced Settings** в разделе **Driver Selector\USART** выбираем «**LL**». Это означает подключение низкоуровневых библиотек для работы с последовательным портом.

На вкладке **Clock Configuration** для платы **NUCLEO-L053R8** выбираем источник тактирования **HSI** в **System Clock Mux**. Для платы **NUCLEO-F072RB** он выбран по умолчанию. 

На вкладке **Pinout&Configuration** для выводов контроллера **PC0** и **PC1** выбираем режимы работы и наименования:
- **PC0** режим **GPIO mode** выбираем **Output Open Drain**, имя **User Label** указываем **OUT_OD**. 
- **PC1** режим **GPIO mode** выбираем **External Interrupt Mode with Rising edge trigger detection**, имя **User Label** указываем **IN_RISING**. 

Включаем прерывание для соотвествующих ножек контроллера. Во вкладке **NVIC Interrupt Table** для **GPIO** включаем **EXTI line 0 and 1 interrupts**. 

Для сторожевого таймера **IWDG** выбираем **Activated**.

Указываем наименование и путь к файлам проекта. Генерируем код кнопкой **GENERATE CODE**.

## Сборка готового проекта

Открываем ранее созданный проект.

Файлы **cmd.h, fifo.h, log.h, time.h, usart.h** копируем в \Core\Inc\.

Файлы **cmd.c, fifo.c, log.c, time.c, usart.c** копируем в \Core\Src\.

### Корректировка main.h

В файл main.h добавляем следующий код:
```
extern volatile uint8_t alarm_sensor;
```

### Корректировка main.c

В main.c добавляем все заголовки: 
```
#include "cmd.h"
#include "log.h"
#include "fifo.h"
#include "time.h"
#include "usart.h"
```
Добавляем определения:
```
#define IWDG_ENABLE
```
Добавляем переменные:
```
volatile uint8_t alarm_sensor;
```
В функцию **MX_USART2_UART_Init()** добавляем вызов **USART_Init()**.

Добавляем следующие функции:

**Простое преобразование из числа в строку**
```
#define MAX_NUMBER_LEN 11

unsigned char * itoa(int32_t value, unsigned char base){
  static volatile unsigned char res[MAX_NUMBER_LEN+1] = {0};
  static unsigned char str[MAX_NUMBER_LEN] = {0};
  unsigned char resnum = 0;
  if (value<0)
  {
    res[resnum++] = '-';
    value=-value;
  }else
  if (value==0)
  {
    res[0]='0';
    res[1]=0;
    return (unsigned char*)res;
  }
  unsigned char ctr = MAX_NUMBER_LEN-1;
  str[ctr] = 0;
  while ((value != 0) && (ctr-1)){
    str[--ctr] = "0123456789abcdef"[value % base];
    value /= base;
  }
  while (ctr<MAX_NUMBER_LEN-1)
    res[resnum++]=str[ctr++];
  res[resnum] = 0;
  return (unsigned char*)res;
}
```
**Вычитывание состояний из лога и отправка их в последованиельный порт**
```
void ReadAndShowStates()
{
	for (int i=0;i<MAX_EVENTS;i++)
	{
		EVENT e = Read_Event();
		if (!e.time) continue;
		//Write state to terminal
		USART_Write_String((unsigned char*)"time=");
		USART_Write_String(itoa(e.time,10));
		USART_Write_String((unsigned char*)", event=");
		switch (e.data)
		{
		case CMD_LED_ON:
			USART_Write_String((unsigned char*)"LED ON");
			break;
		case CMD_LED_OFF:
			USART_Write_String((unsigned char*)"LED OFF");
			break;
		case CMD_OUT_ON:
			USART_Write_String((unsigned char*)"OUT ON");
			break;
		case CMD_OUT_OFF:
			USART_Write_String((unsigned char*)"OUT OFF");
			break;
		case CMD_BUTTON:
			USART_Write_String((unsigned char*)"BUTTON PRESSED");
			break;
		case CMD_ALARM:
			USART_Write_String((unsigned char*)"ALARM");
			break;
		case CMD_INFINITY:
			USART_Write_String((unsigned char*)"INFINITY CYCLE");
			break;
		default:
			USART_Write_String(itoa(e.data,10));
		}
		USART_Write_String((unsigned char*)"\r\n");
		USART_Send();
		HAL_Delay(10);

#ifdef IWDG_ENABLE
		HAL_IWDG_Refresh(&hiwdg);
#endif
	}
}
```
В функцию main() добавляем следующий код:
```
#ifdef IWDG_ENABLE
  MX_IWDG_Init();
#endif

alarm_sensor = 0;
LL_USART_EnableIT_RXNE(USART2);  
USART_Write_String((unsigned char*)"Ready\r\n");
```
В тело цикла while добавляем следующий код: 
```
if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)==GPIO_PIN_RESET)
{
	USART_Write_String((unsigned char*)"Button pressed\r\n");
	Write_Event(CMD_BUTTON);
	USART_Send();
	HAL_Delay(100);
}

if (alarm_sensor)
{
	USART_Write_String((unsigned char*)"Alarm!!!\r\n");
	Write_Event(CMD_ALARM);
	USART_Send();
	alarm_sensor = 0;
	HAL_Delay(100);
}

uint16_t cmd = Cmd_Read();
if (cmd != CMD_NA)
{
	USART_Write_String((unsigned char*)"cmd=");
	USART_Write_String(itoa(cmd,10));
	USART_Write_String((unsigned char*)"\r\n");

	if (cmd!=CMD_READ_LOG)
			Write_Event(cmd);

	char OK = 1;
	switch (cmd)
	{
		case 	CMD_EEPROM_TEST:
			USART_Write_String((unsigned char*)"Start EEPROM test\r\n");
			USART_Send();

			HAL_StatusTypeDef status = HAL_OK;
			for (uint32_t adr = 0;adr<MAX_EVENTS;adr++)
			{
				status = Write_EEPROM(adr*4, (adr+10)<<16);
				#ifdef IWDG_ENABLE
					HAL_IWDG_Refresh(&hiwdg);
				#endif
				if (status!=HAL_OK) break;
			}

			uint32_t err = 0;
			if (status==HAL_OK){
				for (uint32_t adr = 0;adr<MAX_EVENTS;adr++)
				{
					if (Read_EEPROM(adr*4)!=((adr+10)<<16))
						{
							err = 1;
							break;
						}
				}
			}

			OK = (status==HAL_OK)&&(!err);

			break;
		case	CMD_LED_ON:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			break;
		case	CMD_LED_OFF:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			break;
		case	CMD_OUT_ON:
			HAL_GPIO_WritePin(OUT_OD_GPIO_Port, OUT_OD_Pin, GPIO_PIN_RESET);
			break;
		case	CMD_OUT_OFF:
			HAL_GPIO_WritePin(OUT_OD_GPIO_Port, OUT_OD_Pin, GPIO_PIN_SET);
			break;
		case	CMD_READ_LOG:
			ReadAndShowStates();
			break;
		case	CMD_BUTTON:
			break;
		case	CMD_ALARM:
			alarm_sensor = 1;
			break;
		case	CMD_INFINITY:
			while (1) {};
			break;
		default:
			OK = 0;
	}

	if (OK)
	{
		USART_Write_String((unsigned char*)"OK\r\n");
	}else
		USART_Write_String((unsigned char*)"FAILED\r\n");
}

USART_Send();
HAL_Delay(10);

#ifdef IWDG_ENABLE
HAL_IWDG_Refresh(&hiwdg);
#endif
```

### Корректировка stm32f0xx_it.с 
В файл stm32f0xx_it.с добавляем следующий код:
```
#include "time.h"
#include "usart.h"
```
В функцию **SysTick_Handler** добавляем вызов **TimeShift1ms().**

В функцию **USART2_IRQHandler()** добавляем вызов **USART_IRQ().**

В **EXTI0_1_IRQHandler** добавляем код:
```
alarm_sensor = 1;
```

## Проверка
Компилируем код и запускаем на микроконтроллере. Для взаимодействия с микроконтроллером можно использовать программу ComTest. 

https://github.com/exatb/ComTest

Должна быть корректная реакция на все команды.

![image](https://github.com/user-attachments/assets/f5953525-6a9d-4bb5-adff-49e2da39cb13)

При возникновении пламени в зоне видимости фотодиода должно появляться сообщение Alarm!


