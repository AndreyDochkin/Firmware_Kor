#ifndef	encoder_h
#define	encoder_h
#include <avr/io.h>
//_________________________________________
//порт и выводы к которым подключен энкодер скорости
#define PORT_Enc 	PORTA 	
#define PIN_Enc 	PINA
#define DDR_Enc 	DDRA
#define Pin1_Enc 	0
#define Pin2_Enc 	1
#define ENC_MASK	0b00000011
//______________________
//порт и выводы к которым подключен энкодер времени
#define PORT_Enc1 	PORTA 	
#define PIN_Enc1 	PINA
#define DDR_Enc1 	DDRA
#define Pin1_Enc1	2
#define Pin2_Enc1 	3
#define ENC_MASK1	0b00001100
//______________________

#define SPEED_STEP			25//100	//минимальный инкремент энкодера
#define SPEED_LIMIT_UP		5000//максимальное задаваемое значение скорости
#define SPEED_LIMIT_DOWN	500	//минимальное задавемое значение скорости

#define TIME_STEP			1	//минимальный инкремент энкодера
#define TIME_LIMIT_UP		300	//максимальное задаваемое значение времени
#define TIME_LIMIT_DOWN		0	//минимальное задавемое значение времени

void ENC_SPEED_InitEncoder(void);
void ENC_TIME_InitEncoder(void);
#endif  //encoder_h
