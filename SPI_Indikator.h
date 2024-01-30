#ifndef SPI_INDIKATOR_H_
#define SPI_INDIKATOR_H_
//#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
// Define Soft-SPI connections
#define SPI0_DDR	DDRB		
#define SPI0_PORT	PORTB		
#define SPI0_MOSI   (1<<PB0)
#define SPI0_CS		(1<<PB1)
#define SPI0_SCK	(1<<PB2)

#define SPI1_DDR	DDRB	
#define SPI1_PORT	PORTB	
#define SPI1_MOSI   (1<<PB3)
#define SPI1_CS		(1<<PB4)
#define SPI1_SCK	(1<<PB5)

void SPI_Init(void);

void SPI0_Write_Byte(unsigned short num);
void SPI0_Display_INIT(void);
void SPI0_Display_speed(unsigned int speed);
void SPI0_Display_time(unsigned int time);

void SPI1_Write_Byte(unsigned short num);
void SPI1_Display_INIT(void);
void SPI1_Display_realSpeed(unsigned int realSpeed);

#endif /* SPI_INDIKATOR_H_ */