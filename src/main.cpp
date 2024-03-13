#include <Arduino.h>


void setup() {
  //DDRB = 0;
  //DDRB |=  (1<<5);
  //PORTB |= (1<<5);
  //setup timer 1 for toggling led 13 every 1 second
  TCCR1A =0;
  TCCR1B =0;
  TCNT1 =0;

  //chanel A compare value
  OCR1A = 15625;
  TCCR1B |= (1 << WGM12); //ctc mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // set presca;ar
  TIMSK1 |= ( 1 << OCIE1A); //match A interrupt enable
  sei(); //enable glpbal interruptd

  DDRB |= (1 << DDB5);  //set pin 13 a s output

}

ISR(TIMER1_COMPA_vect){
  PORTB ^= (1 << PORTB5);
}

void loop() {
  //delay(500);
  //PORTB &= ~(1<<5);
  //delay(500);
  //PORTB |= (1<<5);

   
}
