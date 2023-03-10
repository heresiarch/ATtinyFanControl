# Temperature regulated PWM Fan Control with ATTiny 45/85
This is a simple circuit to drive an 4-Pin PWM Fan using an ATtiny 45. With an Arduino Uno the first code was running after 30 minutes, but I would like to use a very small circuit in my gaming PC, so I tried to adopt it to some left over ATTinys. It was not as easy as I thought, because fast PWM on ATtiny needs special setup of some registers. The best description I found was on this website, which sets the PWM to 32 kHz, with 8Mhz F_CPU and no CKLDIV, which is a little bit higher than 25 kHz needed for the PWM of the Fan, but it worked with full duty cycle.

https://blog.podkalicki.com/attiny13-hardware-pwm/

The second challenge was that Arduiono analogread() did not work on the ATtiny at all. I found an alternative way on the following page:

http://www.hilltop-cottage.info/blogs/adam/avr-adc-1-basic-examples-attiny85/

https://github.com/arc12/AVR-Experiments-and-Examples/blob/master/ADC%20Tiny85/ADC%20Tiny85/ADC%20Tiny85.cpp

The NTC conversion/correction to °C from 10bit ADC output was generated with this page:
https://www.sebulli.com/ntc/index.php

The circuit and code should be self explanatory.
![Alt text][def3]

Example screenshots of changing pulse width on NTC with different temperatures and the frequency of about 32kHz.
![PWM1][def1]

![PWM2][def2]

[def1]: doc/pwm01.jpg?raw=true "PWM short pulse width"
[def2]: doc/pwm02.jpg?raw=true "PWM longer pulse width"
[def3]: fritzing/fritzing_circuit.png?raw=true "PWM Control"