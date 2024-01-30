#include "SPI_Indikator.h"

void SPI_Init(void)
{
  SPI0_DDR |= SPI0_CS | SPI0_MOSI | SPI0_SCK;
  SPI1_DDR |= SPI1_CS | SPI1_MOSI | SPI1_SCK;

  SPI0_PORT |= SPI0_CS;
  SPI1_PORT |= SPI1_CS;
}

void SPI0_Write_Byte(unsigned short num)
{
  unsigned short t, mask, flag;
  SPI0_PORT &= ~SPI0_SCK;
  mask = 128;
  for (t = 0; t < 8; t++)
  {
    flag = num & mask;
    if (flag == 0)
      SPI0_PORT &= ~SPI0_MOSI; // SPI0_MOSI = 0;
    else
      SPI0_PORT |= SPI0_MOSI; // SPI0_MOSI = 1;
    SPI0_PORT |= SPI0_SCK;
    SPI0_PORT &= ~SPI0_SCK;
    mask = mask >> 1;
  }
}

void SPI0_Display_INIT()
{
  // Disable Shutdown mode
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(0x0C); // Select Shutdown register
  SPI0_Write_Byte(0x01); // Set D0 bit to return to normal operation
  SPI0_PORT |= SPI0_CS;  // CS pin is pulled HIGH

  // Set BCD decode mode for digits DIG0-DIG3
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(0x09); // Select Decode Mode register
  SPI0_Write_Byte(0xFF); // SPI0_Write_Byte(0x0F);    // Select BCD mode for digits DIG0-DIG3
  SPI0_PORT |= SPI0_CS;  // CS pin is pulled HIGH

  // Set display brighness
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(0x0A); // Select Intensity register
  SPI0_Write_Byte(0x08); //(0x0a);    // Set maximum brightness
  SPI0_PORT |= SPI0_CS;  // CS pin is pulled HIGH

  // Set display refresh
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(0x0B); // Select Scan-Limit register
  SPI0_Write_Byte(0x07); // SPI0_Write_Byte(0x03);    // Select digits DIG0-DIG3
  SPI0_PORT |= SPI0_CS;  // CS pin is pulled HIGH

  // Enable Display-Test
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(0x0F); // Select Display-Test register
  SPI0_Write_Byte(0x01); // Enable Display-Test
  SPI0_PORT |= SPI0_CS;  // CS pin is pulled HIGH

  _delay_ms(250);
  // Disable Display-Test
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(0x0F); // Select Display-Test register
  SPI0_Write_Byte(0x00); // Disable Display-Test
  SPI0_PORT |= SPI0_CS;  // CS pin is pulled HIGH
}

void SPI0_Display_speed(unsigned int speed)
{
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(4);    // Send thousands digit
  SPI0_Write_Byte((speed / 1000) % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH

  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(3);    // Send hundreds digit
  SPI0_Write_Byte((speed / 100) % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH

  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(2);    // Send tens digit
  SPI0_Write_Byte((speed / 10) % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH

  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(1);    // Send ones digit
  SPI0_Write_Byte(speed % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH
}

void SPI0_Display_time(unsigned int time)
{
  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(8);    // Send thousands digit
  SPI0_Write_Byte((time / 1000) % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH

  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(7);    // Send hundreds digit
  SPI0_Write_Byte((time / 100) % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH

  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(6);    // Send tens digit
  SPI0_Write_Byte((time / 10) % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH

  SPI0_PORT &= ~SPI0_CS; // CS pin is pulled LOW
  SPI0_Write_Byte(5);    // Send ones digit
  SPI0_Write_Byte(time % 10);
  SPI0_PORT |= SPI0_CS; // CS pin is pulled HIGH
}

void SPI1_Write_Byte(unsigned short num)
{
  unsigned short t, mask, flag;
  SPI1_PORT &= ~SPI1_SCK;
  mask = 128;
  for (t = 0; t < 8; t++)
  {
    flag = num & mask;
    if (flag == 0)
      SPI1_PORT &= ~SPI1_MOSI; // SPI1_MOSI = 0;
    else
      SPI1_PORT |= SPI1_MOSI; // SPI1_MOSI = 1;
    SPI1_PORT |= SPI1_SCK;
    SPI1_PORT &= ~SPI1_SCK;
    mask = mask >> 1;
  }
}

void SPI1_Display_INIT()
{
  // Disable Shutdown mode
  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(0x0C); // Select Shutdown register
  SPI1_Write_Byte(0x01); // Set D0 bit to return to normal operation
  SPI1_PORT |= SPI1_CS;  // CS pin is pulled HIGH

  // Set BCD decode mode for digits DIG0-DIG3
  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(0x09); // Select Decode Mode register
  SPI1_Write_Byte(0xFF); // Select BCD mode for digits DIG0-DIG3
  SPI1_PORT |= SPI1_CS;  // CS pin is pulled HIGH

  // Set display brighness
  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(0x0A); // Select Intensity register
  SPI1_Write_Byte(0x08); // Set maximum brightness
  SPI1_PORT |= SPI1_CS;  // CS pin is pulled HIGH

  // Set display refresh
  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(0x0B); // Select Scan-Limit register
  SPI1_Write_Byte(0x07); // Select digits DIG0-DIG3
  SPI1_PORT |= SPI1_CS;  // CS pin is pulled HIGH

  // Enable Display-Test
  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(0x0F); // Select Display-Test register
  SPI1_Write_Byte(0x01); // Enable Display-Test
  SPI1_PORT |= SPI1_CS;  // CS pin is pulled HIGH

  _delay_ms(250);
  // Disable Display-Test
  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(0x0F); // Select Display-Test register
  SPI1_Write_Byte(0x00); // Disable Display-Test
  SPI1_PORT |= SPI1_CS;  // CS pin is pulled HIGH
}

void SPI1_Display_realSpeed(unsigned int realSpeed)
{
  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(8);    // Send thousands digit
  SPI1_Write_Byte((realSpeed / 1000) % 10);
  SPI1_PORT |= SPI1_CS; // CS pin is pulled HIGH

  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(7);    // Send hundreds digit
  SPI1_Write_Byte((realSpeed / 100) % 10);
  SPI1_PORT |= SPI1_CS; // CS pin is pulled HIGH

  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(6);    // Send tens digit
  SPI1_Write_Byte((realSpeed / 10) % 10);
  SPI1_PORT |= SPI1_CS; // CS pin is pulled HIGH

  SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
  SPI1_Write_Byte(5);    // Send ones digit
  SPI1_Write_Byte(realSpeed % 10);
  SPI1_PORT |= SPI1_CS; // CS pin is pulled HIGH

  //   int i=1;
  //   for (i=1;i<5;i++)
  //   {
  // 	  SPI1_PORT&=~SPI1_CS;                   // CS pin is pulled LOW
  // 	  SPI1_Write_Byte(i);             // Send ones digit
  // 	  SPI1_Write_Byte(0x0F);
  // 	  SPI1_PORT|=SPI1_CS;                   // CS pin is pulled HIGH
  //   }
}

void SPI1_Display_vacuum(unsigned char vac)
{
  // CS pin is pulled HIGH

  int i = 1;
  for (i = 1; i < 5; i++)
  {
    SPI1_PORT &= ~SPI1_CS; // CS pin is pulled LOW
    SPI1_Write_Byte(i);    // Send ones digit
    if (vac)
    {
      SPI1_Write_Byte(0b00001010);
    }
    else
      SPI1_Write_Byte(0x0F);

    SPI1_PORT |= SPI1_CS; // CS pin is pulled HIGH
  }
}
/*void SPI1_Display(unsigned int time){
  SPI1_PORT&=~SPI1_CS;                   // CS pin is pulled LOW
  SPI1_Write_Byte(4);             // Send thousands digit
  SPI1_Write_Byte((time/1000)%10);
  SPI1_PORT|=SPI1_CS;                   // CS pin is pulled HIGH

  SPI1_PORT&=~SPI1_CS;                   // CS pin is pulled LOW
  SPI1_Write_Byte(3);             // Send hundreds digit
  SPI1_Write_Byte((time/100)%10);
  SPI1_PORT|=SPI1_CS;                   // CS pin is pulled HIGH

  SPI1_PORT&=~SPI1_CS;                   // CS pin is pulled LOW
  SPI1_Write_Byte(2);             // Send tens digit
  SPI1_Write_Byte((time/10)%10);
  SPI1_PORT|=SPI1_CS;                   // CS pin is pulled HIGH

  SPI1_PORT&=~SPI1_CS;                   // CS pin is pulled LOW
  SPI1_Write_Byte(1);             // Send ones digit
  SPI1_Write_Byte(time%10);
  SPI1_PORT|=SPI1_CS;                   // CS pin is pulled HIGH
 }*/
