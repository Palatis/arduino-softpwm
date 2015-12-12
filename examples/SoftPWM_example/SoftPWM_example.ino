/* Define this before including SoftPWM.h for printInterruptLoad()
   to work. */
#define __DEBUG_SOFTPWM__ 1

#include <SoftPWM.h>

/* pins_arduino.h defines the pin-port/bit mapping as PROGMEM so
   you have to read them with pgm_read_xxx(). That's generally okay
   for ordinary use, but really bad when you're writing super fast
   codes because the compiler doesn't treat them as constants and
   cannot optimize them away with sbi/cbi instructions.

   Therefore we have to tell the compiler the PORT and BIT here.
   Hope someday we can find a way to workaround this.

   Check the manual of your MCU for port/bit mapping. */
SOFTPWM_DEFINE_CHANNEL(0, DDRC, PORTC, PORTC3);
SOFTPWM_DEFINE_CHANNEL(1, DDRC, PORTC, PORTC2);
SOFTPWM_DEFINE_CHANNEL(2, DDRD, PORTD, PORTD4);
SOFTPWM_DEFINE_CHANNEL(3, DDRB, PORTB, PORTB6);
SOFTPWM_DEFINE_CHANNEL(4, DDRB, PORTB, PORTB7);
SOFTPWM_DEFINE_CHANNEL(5, DDRD, PORTD, PORTD5);
SOFTPWM_DEFINE_CHANNEL(6, DDRD, PORTD, PORTD6);
SOFTPWM_DEFINE_CHANNEL(7, DDRD, PORTD, PORTD7);
SOFTPWM_DEFINE_CHANNEL(8, DDRB, PORTB, PORTB0);
SOFTPWM_DEFINE_CHANNEL(9, DDRB, PORTB, PORTB1);
SOFTPWM_DEFINE_CHANNEL(10, DDRB, PORTB, PORTB2);
SOFTPWM_DEFINE_CHANNEL(11, DDRB, PORTB, PORTB3);
SOFTPWM_DEFINE_CHANNEL(12, DDRB, PORTB, PORTB4);
SOFTPWM_DEFINE_CHANNEL(13, DDRB, PORTB, PORTB5);
SOFTPWM_DEFINE_CHANNEL(14, DDRC, PORTC, PORTC0);
SOFTPWM_DEFINE_CHANNEL(15, DDRC, PORTC, PORTC1);

/* Or you may want inverted outputs: */
/*
  SOFTPWM_DEFINE_CHANNEL_INVERT(0, DDRC, PORTC, PORTC3);
  SOFTPWM_DEFINE_CHANNEL_INVERT(1, DDRC, PORTC, PORTC2);
  SOFTPWM_DEFINE_CHANNEL_INVERT(2, DDRD, PORTD, PORTD4);
  SOFTPWM_DEFINE_CHANNEL_INVERT(3, DDRB, PORTB, PORTB6);
  SOFTPWM_DEFINE_CHANNEL_INVERT(4, DDRB, PORTB, PORTB7);
  SOFTPWM_DEFINE_CHANNEL_INVERT(5, DDRD, PORTD, PORTD5);
  SOFTPWM_DEFINE_CHANNEL_INVERT(6, DDRD, PORTD, PORTD6);
  SOFTPWM_DEFINE_CHANNEL_INVERT(7, DDRD, PORTD, PORTD7);
  SOFTPWM_DEFINE_CHANNEL_INVERT(8, DDRB, PORTB, PORTB0);
  SOFTPWM_DEFINE_CHANNEL_INVERT(9, DDRB, PORTB, PORTB1);
  SOFTPWM_DEFINE_CHANNEL_INVERT(10, DDRB, PORTB, PORTB2);
  SOFTPWM_DEFINE_CHANNEL_INVERT(11, DDRB, PORTB, PORTB3);
  SOFTPWM_DEFINE_CHANNEL_INVERT(12, DDRB, PORTB, PORTB4);
  SOFTPWM_DEFINE_CHANNEL_INVERT(13, DDRB, PORTB, PORTB5);
  SOFTPWM_DEFINE_CHANNEL_INVERT(14, DDRC, PORTC, PORTC0);
  SOFTPWM_DEFINE_CHANNEL_INVERT(15, DDRC, PORTC, PORTC1);
*/

/* Here you make an instance of desired channel counts you want
   with the default 256 brightness levels (0 ~ 255). */
//SOFTPWM_DEFINE_OBJECT(16);

/* Or you can make one with only 100 brightness levels (0 ~ 99).
   By using less brightness levels, you may be able to use higher
   pwm frequencies. */
SOFTPWM_DEFINE_OBJECT_WITH_BRIGHTNESS_LEVELS(16, 100);

/* If you want to use the SoftPWM object outside where it's defined,
   add the following line to the file. */
//SOFTPWM_DEFINE_EXTERN_OBJECT(16);
SOFTPWM_DEFINE_EXTERN_OBJECT_WITH_BRIGHTNESS_LEVELS(16, 100);

void setup() {
  Serial.begin(19200);

  // begin with 60hz pwm frequency
  SoftPWM.begin(60);

  // print interrupt load for diagnostic purposes
  SoftPWM.printInterruptLoad();
}

static volatile uint8_t v = 0;
void loop() {
  long nextMillis = 0;

  for (uint8_t i = 0; i < SoftPWM.size(); ++i) {
    Serial.print(micros());
    Serial.print(" loop(): ");
    Serial.println(i);

    unsigned long const WAIT = 1000000 / SoftPWM.brightnessLevels() / 2;
    unsigned long nextMicros = 0;
    for (int v = 0; v <= SoftPWM.brightnessLevels() - 1; ++v) {
      while (micros() < nextMicros);
      nextMicros = micros() + WAIT;
      SoftPWM.set(i, v);
    }
    for (int v = SoftPWM.brightnessLevels() - 1; v >= 0; --v) {
      while (micros() < nextMicros);
      nextMicros = micros() + WAIT;
      SoftPWM.set(i, v);
    }
  }
}
