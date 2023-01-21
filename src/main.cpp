#include <Arduino.h>
#include "ntc.h"
#include <avr/io.h>
#include <util/delay.h>

// this works well
// https://blog.podkalicki.com/attiny13-hardware-pwm/

#define N_1    (_BV(CS00))
#define N_8    (_BV(CS01))
#define N_64   (_BV(CS01)|_BV(CS00))
#define N_256  (_BV(CS02))
#define N_1024 (_BV(CS02)|_BV(CS00))

void pwm_init(void)
{
    DDRB |= _BV(PB0); // set PWM pin as OUTPUT
    TCCR0A |= _BV(WGM01)|_BV(WGM00); // set timer mode to FAST PWM
    TCCR0A |= _BV(COM0A1); // connect PWM signal to pin (AC0A => PB0)
}

/* When timer is set to Fast PWM Mode, the freqency can be
calculated using equation: F = F_CPU / (N * 256) 
Posible frequencies (@1.2MHz):
 -> F(N_1) = 4.687kHz
 -> F(N_8) = 585Hz
 -> F(N_64) = 73Hz
 -> F(N_256) = 18Hz
 -> F(N_1024) = 4Hz */
void pwm_set_frequency(uint32_t N)
{

    TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | N; // set prescaler
}

void pwm_set_duty(uint8_t duty)
{

    OCR0A = duty; // set the OCRnx
}

void pwm_stop(void)
{
     TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00)); // stop the timer
}
//analog read did not work
// https://github.com/arc12/AVR-Experiments-and-Examples/blob/master/ADC%20Tiny85/ADC%20Tiny85/ADC%20Tiny85.cpp
void adc_init(void)
{
  //setup ADC -	ADC3/PB3 pin2
	ADMUX = (1<<MUX1) | (1<<MUX0); // MUX set to ADC3. VCC as ref. Right justified
	ADCSRA = (1<<ADEN); // turn ADC on and initialise. no auto-trigger, no interrupt. prescaler to div2
	DIDR0 = (1<<ADC3D);//turn digital input circuitry off
}

uint16_t adc_read(void)
{
  ADCSRA |= (1<<ADPS2) | (1<<ADPS1);
	//start a conversion
	ADCSRA |= (1<<ADSC);
	//wait for end of conversion
	while (ADCSRA & (1<<ADSC))
	{
	  //do nothing
	}
	uint8_t result[2];
	result[1] = ADCL;// datasheet says read low first
	result[0] = ADCH;
  uint16_t ret = ((uint16_t)result[0] << 8) | result[1];
  return ret;
}



int main(void)
{
    // init stuff 
    pwm_init();
    pwm_set_frequency(N_1);
    adc_init();
    // loop forever
    while(true){
      // read adc value between 0 and 1023
      uint16_t reading = adc_read();
      // convert adc value to ° Celsius
      // 20.0 degrees are returned as 200, 25.1 degress as 251 ...
      int16_t temp = NTC_ADC2Temperature(reading);
      // map temperature between 20 °C or 70 °C to PWM Duty Cycle of 0-255
      int16_t mapped = map(temp,200,700,0,255);
      // respect the boundaries
      if(mapped > 255){
        mapped = 255;
      }
      else if(mapped < 0){
        mapped = 0;
      }
      // set PWM
      pwm_set_duty(mapped);
    }
}