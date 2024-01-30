#ifndef	encoder_h
#define	encoder_h
#include <avr/io.h>
//_________________________________________
//���� � ������ � ������� ��������� ������� ��������
#define PORT_Enc 	PORTA 	
#define PIN_Enc 	PINA
#define DDR_Enc 	DDRA
#define Pin1_Enc 	0
#define Pin2_Enc 	1
#define ENC_MASK	0b00000011
//______________________
//���� � ������ � ������� ��������� ������� �������
#define PORT_Enc1 	PORTA 	
#define PIN_Enc1 	PINA
#define DDR_Enc1 	DDRA
#define Pin1_Enc1	2
#define Pin2_Enc1 	3
#define ENC_MASK1	0b00001100
//______________________

#define SPEED_STEP			25//100	//����������� ��������� ��������
#define SPEED_LIMIT_UP		5000//������������ ���������� �������� ��������
#define SPEED_LIMIT_DOWN	500	//����������� ��������� �������� ��������

#define TIME_STEP			1	//����������� ��������� ��������
#define TIME_LIMIT_UP		300	//������������ ���������� �������� �������
#define TIME_LIMIT_DOWN		0	//����������� ��������� �������� �������

void ENC_SPEED_InitEncoder(void);
void ENC_TIME_InitEncoder(void);
#endif  //encoder_h
