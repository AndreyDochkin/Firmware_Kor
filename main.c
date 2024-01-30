#include <avr/io.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "avrlibdefs.h"
#include "main.h"
#include "UART.h"
#include "encoder.h"
#include "SPI_Indikator.h"
#include "CRC8.h"

// Encoder state and data
volatile unsigned char EncState_speed = 0;
volatile unsigned int EncData_speed = 1500;
volatile unsigned char EncState_time = 0;
volatile unsigned int EncData_time = 40;
volatile unsigned int Time_Save = 40;
int NewState, OldState, Vol, upState, downState;

// Flags
volatile char Flag_Enc_speed_change = 1;
volatile char Flag_send_speed = 1;
volatile char Flag_first_start = 0;
volatile char Flag_button_ON = 0;
volatile char Flag_show_time = 0;
volatile char Flag_show_speed = 0;
volatile unsigned int temp = 0;

// Initialization sequence
const unsigned char initSequence[] = {0xE6, 0x01, 0x51, 0x00, 0x18};

void PortInit(void);
void InitTimer(void);
void SendCmd(unsigned char cmd, unsigned char data);
void SendSpeed(int speed);
int GetSpeed(void);

void main()
{
    // Initialization
    ENC_SPEED_InitEncoder();
    ENC_TIME_InitEncoder();
    InitTimer();
    PortInit();
    SPI_Init();
    SPI1_Display_INIT();
    SPI0_Display_INIT();
    UART0_Init();
    SPI1_Display_realSpeed(0);

    // Delay for bldc contoller start
    _delay_ms(100);

    // Send initial data
    SendUARTSequence(initSequence, sizeof(initSequence));
    SendCmd(0xA2, 2);  // start command
    SendCmd(0xA5, 20); // tune setting type bldc
    SendCmd(0xA6, 20); // tune setting speed

    // Main loop
    while (1)
    {
        // Handle speed encoder changes
        if (Flag_Enc_speed_change)
        {
            SPI0_Display_speed(EncData_speed);
            Flag_Enc_speed_change = 0;
            Flag_send_speed = 1;
        }

        // Display time if flag is set
        if (Flag_show_time)
        {
            SPI0_Display_time(EncData_time);
            Flag_show_time = 0;
        }

        // Handle button press logic
        if (Flag_button_ON == 1)
        {
            Flag_Enc_speed_change = 1;
            Flag_show_time = 1;

            // Send speed data and display real speed
            if (Flag_send_speed)
            {
                SendSpeed(EncData_speed);
                Flag_send_speed = 0;
            }

            SPI1_Display_realSpeed(GetSpeed());
        }

        // Reset speed and display when button is not pressed
        if (Flag_button_ON == 0)
        {
            SendSpeed(0);
            SPI0_Display_time(EncData_time);
            SPI1_Display_realSpeed(GetSpeed());
        }

        // Handle vacuum display based on PD2 state
        if (PIND & (1 << PD2))
        {
            if (PIND & (1 << PD2))
            {
                Flag_button_ON = 0;
                LED_PORT &= ~(1 << LED_PIN);
                SPI1_Display_vacuum(0);
            }
        }
        else
        {
            LED_PORT |= (1 << LED_PIN);
            SPI1_Display_vacuum(1);
        }
    }
}

void PortInit(void)
{
    // Initialize LED ports
    LED_DDR |= (1 << LED_PIN);
    LED_PORT &= ~(1 << LED_PIN);

    // Initialize button ports
    DDRD &= ~(1 << PIN_INT0) | ~(1 << PIN_INT1) | ~(1 << PIN_INT2) | ~(1 << PIN_INT3);
    PORTD |= (1 << PIN_INT0) | (1 << PIN_INT1) | (1 << PIN_INT2) | (1 << PIN_INT3);

    // Configure external interrupts
    EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT3);
    EICRA |= (1 << ISC01) | (0 << ISC00);
    EICRA |= (1 << ISC11) | (0 << ISC10);
}

void InitTimer(void)
{
    // Timer 0 initialization for speed control
    TCCR0 |= (0 << CS02) | (1 << CS01) | (1 << CS00);
    TCCR0 |= (1 << WGM01) | (0 << WGM00);
    TCNT0 = 0;
    OCR0 = 50;

    // Timer 1 initialization for time control
    TCCR1A |= (0 << WGM11) | (0 << WGM10);
    TCCR1B |= (0 << WGM13) | (1 << WGM12);
    TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
    TCNT1 = 0;
    OCR1A = 62500;

    // Enable timer interrupts
    TIMSK |= (1 << OCIE0) | (1 << OCIE1A);
}
void SendUARTSequence(const unsigned char *sequence, unsigned int length)
{
    for (unsigned int i = 0; i < length; i++)
    {
        UART0TX(sequence[i]);
    }
}

// Function to send speed via UART
void SendSpeed(int speed)
{
    unsigned char seed = 0;
    unsigned char BLSD_speed = speed / 60;

    // Initialization sequence to UART
    const unsigned char initSequence[] = {0xE6, 0x01, 0x51, 0x00, 0x18};
    SendUARTSequence(initSequence, sizeof(initSequence));

    // Calculate CRC and send speed data
    unsigned char CRC1 = Calc_CRC8(0x01, seed);
    unsigned char CRC2 = Calc_CRC8(0xA3, CRC1);
    unsigned char CRC3 = Calc_CRC8(BLSD_speed, CRC2);

    const unsigned char speedSequence[] = {0xE6, 0x01, 0xA3, BLSD_speed, CRC3};
    SendUARTSequence(speedSequence, sizeof(speedSequence));
}

// Function to get speed from UART
int GetSpeed(void)
{
    cli(); // Disable interrupts

    // Request speed data from UART
    const unsigned char requestSequence[] = {0xE6, 0x01, 0x50, 0x1F};
    SendUARTSequence(requestSequence, sizeof(requestSequence));

    // Receive speed data from UART
    unsigned char BLSD_speed = UART0RX(); // Speed

    sei(); // Enable interrupts
    return (BLSD_speed * 60);
}

// Function to send a command via UART
void SendCmd(unsigned char cmd, unsigned char data)
{
    unsigned char seed = 0;

    // Calculate CRC for the command
    unsigned char CRC1 = Calc_CRC8(0x01, seed);
    unsigned char CRC2 = Calc_CRC8(cmd, CRC1);
    unsigned char CRC3 = Calc_CRC8(data, CRC2);

    // Command sequence
    const unsigned char cmdSequence[] = {0xE6, 0x01, cmd, data, CRC3};
    SendUARTSequence(cmdSequence, sizeof(cmdSequence));
}

ISR(TIMER0_COMP_vect)
{
    // Clear the Timer0 compare flag and reset the timer count
    TIFR |= (0 << OCF0); // Clear compare flag
    TCNT0 = 0;           // Reset timer count

    // Read new encoder states
    unsigned char new_s = PIN_Enc & ENC_MASK;          // New speed encoder state
    unsigned char new_t = (PIN_Enc1 & ENC_MASK1) >> 2; // New time encoder state

    // Update speed based on encoder state change
    if (EncState_speed != new_s)
    {
        Flag_Enc_speed_change = 1;
        switch (EncState_speed)
        {
        case 2:
            if (new_s == 3 && EncData_speed != SPEED_LIMIT_UP)
                EncData_speed += SPEED_STEP;
            if (new_s == 0 && EncData_speed != SPEED_LIMIT_DOWN)
                EncData_speed -= SPEED_STEP;
            break;
        case 0:
            if (new_s == 2 && EncData_speed != SPEED_LIMIT_UP)
                EncData_speed += SPEED_STEP;
            if (new_s == 1 && EncData_speed != SPEED_LIMIT_DOWN)
                EncData_speed -= SPEED_STEP;
            break;
        case 1:
            if (new_s == 0 && EncData_speed != SPEED_LIMIT_UP)
                EncData_speed += SPEED_STEP;
            if (new_s == 3 && EncData_speed != SPEED_LIMIT_DOWN)
                EncData_speed -= SPEED_STEP;
            break;
        case 3:
            if (new_s == 1 && EncData_speed != SPEED_LIMIT_UP)
                EncData_speed += SPEED_STEP;
            if (new_s == 2 && EncData_speed != SPEED_LIMIT_DOWN)
                EncData_speed -= SPEED_STEP;
            break;
        }
        EncState_speed = new_s; // Update the current speed encoder state
    }

    // Update time based on encoder state change
    if (EncState_time != new_t)
    {
        Flag_show_time = 1;
        switch (EncState_time)
        {
        case 2:
            if (new_t == 3)
                upState++;
            if (new_t == 0)
                downState++;
            break;
        case 0:
            if (new_t == 2)
                upState++;
            if (new_t == 1)
                downState++;
            break;
        case 1:
            if (new_t == 0)
                upState++;
            if (new_t == 3)
                downState++;
            break;
        case 3:
            if (new_t == 1)
                upState++;
            if (new_t == 2)
                downState++;
            break;
        }
        EncState_time = new_t; // Update the current time encoder state
    }

    // Adjust time based on encoder input
    if (upState >= 4)
    {
        EncData_time++;
        upState = 0;
    }
    if (downState >= 4)
    {
        EncData_time--;
        downState = 0;
    }
}

ISR(INT0_vect)
{
    // Debounce delay for button press
    _delay_ms(25);

    // Check if both PD0 and PD2 are pressed
    if (!(PIND & (1 << PD0)) && !(PIND & (1 << PD2)))
    {
        Flag_button_ON = 1;
        Time_Save = EncData_time;
        TCNT1 = 0;
        TIMSK |= (1 << OCIE1A); // Enable Timer1 Compare A interrupt
    }
}

ISR(INT1_vect)
{
    // Debounce delay for button release
    _delay_ms(25);

    // Check if PD1 is released
    if (!(PIND & (1 << PD1)))
    {
        Flag_button_ON = 0;
        EncData_time = Time_Save;
    }
}

ISR(TIMER1_COMPA_vect)
{
    // Disable interrupts to ensure atomic operation
    cli();

    // Reset Timer1 count
    TCNT1 = 0;
    Flag_show_time = 1;

    // Decrement time if button is ON and update display flags
    if (Flag_button_ON == 1)
    {
        EncData_time--;
        Flag_show_speed = 1;
    }

    // Reset flags and timer interrupt if time reaches zero
    if (EncData_time == 0)
    {
        Flag_button_ON = 0;
        EncData_time = Time_Save;
        TIMSK &= ~(1 << OCIE1A); // Disable Timer1 Compare A interrupt
    }

    // Re-enable interrupts
    sei();
}
