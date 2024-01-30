#include "UART.h"

// Macros for RTS control
#define RTS_DDR     DDRC
#define RTS_PORT    PORTC
#define RTS_PIN     PC0

// UART initialization function
void UART0_Init(void)
{
    // Configure RTS pin for RS-485 control
    RTS_DDR |= (1 << RTS_PIN);   // Set RTS_PIN as an output
    RTS_PORT &= ~(1 << RTS_PIN); // Set RTS_PIN low (transmit mode)
    
    // Set UART baud rate to 9600 (for F_CPU = 16MHz)
    UBRR0H = 0;
    UBRR0L = 0x67; // 9600 baud
    
    // Enable RX and TX, 8 data bits, no parity, 1 stop bit
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    // 1 stop bit
    UCSR0C &= ~(1 << USBS0);
    
    _delay_us(10);
}

// Receive a byte from UART
unsigned char UART0RX(void)
{
    if (!receiveMode)
    {
        // Switch to receive mode and disable transmit
        RTS_LOW;
        cbi(UCSR0B, TXEN0);
        sbi(UCSR0B, RXEN0);
        receiveMode = 1;
    }

    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

// Transmit a byte via UART
void UART0TX(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0)));

    if (receiveMode)
    {
        // Switch to transmit mode and enable transmit
        cbi(UCSR0B, RXEN0);
        sbi(UCSR0B, TXEN0);
        RTS_HIGH; // Set RTS high (receive mode off)
        receiveMode = 0;
    }

    UDR0 = c;

    while (!(UCSR0A & (1 << TXC0)));
}
