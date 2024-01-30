#include "encoder.h"

void ENC_SPEED_InitEncoder(void)
{  
  DDR_Enc|=(0<<Pin1_Enc)|(0<<Pin2_Enc);
  PORT_Enc|=(1<<Pin1_Enc)|(1<<Pin2_Enc);
}

void ENC_TIME_InitEncoder(void)
{  
  DDR_Enc1|=(0<<Pin1_Enc1)|(0<<Pin2_Enc1);
  PORT_Enc1|=(1<<Pin1_Enc1)|(1<<Pin2_Enc1);
}




