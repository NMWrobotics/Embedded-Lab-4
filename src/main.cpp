#include <Arduino.h>

unsigned long milliSeconds;
unsigned long us;

void setup() {
  Serial.begin(9600);
  DDRB = 0;
  DDRB |=  (1<<5);
  //PORTB |= (1<<5);

  cli();
  //setup timer 1 for toggling led 13 every 1 second
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCNT1 = 0;

  //set Timer 1 for count miliseconds
  //calculating OCR1A
  //1ms 1KHz
  //OCR1A = (16,000,000/(pre scalar*1000)) - 1 
  //OCR1A = (16,000,000/(1*1000)) - 1 = 15999
  //OCR1A = (16,000,000/(8*1000)) - 1 = 1999
  //OCR1A = (16,000,000/(64*1000)) - 1 = 249
  //OCR1A = (16,000,000/(256*1000)) - 1 = 61.5
  //OCR1A = (16,000,000/(1024*1000)) - 1 = 14.625
  //lets choose 64 as the pre scalar then OCR1A = 249
  OCR1A = 249;
  TCCR1B |= (1 << WGM12); //ctc mode
  TCCR1B |= (1 << CS10) | (1 << CS11); // set prescalar
  TIMSK1 |= ( 1 << OCIE1A); //match A interrupt enable
  sei(); //enable glpbal interruptd

  
  
  //DDRB |= (1 << DDB5);  //set pin 13 a s output

}

ISR(TIMER1_COMPA_vect){
  //PORTB ^= (1 << PORTB5);
  milliSeconds++;
}
void delay_ms(unsigned long ms){
  unsigned long current_ms = milliSeconds;
  while(true){
    if(milliSeconds - current_ms >= ms){
      break;
    }
    Serial.println("yes");
  }
}

void loop() {
  //delay(500);
  //PORTB &= ~(1<<5);
  //delay(500);
  PORTB |= (1<<5);
  delay_ms(3000);
  //delay(3000);
  PORTB &= ~(1<<5);
  delay_ms(1000);
  //delay(1000);
   
}
