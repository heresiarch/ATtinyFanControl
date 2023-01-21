#include "ntc.h"

int16_t NTC_table[33] = {
  1570, 1293, 1016, 866, 763, 685, 621, 567, 
  520, 477, 439, 403, 370, 338, 308, 278, 250, 
  222, 194, 167, 139, 111, 83, 53, 22, -11, 
  -47, -87, -132, -186, -256, -364, -472
};

int16_t NTC_ADC2Temperature(uint16_t adc_value){
  int16_t p1,p2;
  p1 = NTC_table[ (adc_value >> 5)  ];
  p2 = NTC_table[ (adc_value >> 5)+1];
 
  /* Zwischen beiden Punkten linear interpolieren. */
  return p1 - ( (p1-p2) * (adc_value & 0x001F) ) / 32;
};
