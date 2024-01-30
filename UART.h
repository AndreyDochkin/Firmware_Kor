#include "avrlibdefs.h"
#include "avrlibtypes.h"
#include <avr/io.h>
#include <util/delay.h>

#define RTS_PIN		PE2    
#define RTS_DDR		DDRE
#define RTS_PORT	PORTE

#define RTS_HIGH   RTS_PORT |= (1<<RTS_PIN); 
#define RTS_LOW	   RTS_PORT &= ~( 1<<RTS_PIN ); 

void UART0_Init(void);
uint8_t isReceivedByte(void);
unsigned char UART0RX(void); 
void UART0TX(unsigned char c); 

