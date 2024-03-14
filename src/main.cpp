#include <Arduino.h>

unsigned long milliSeconds;
unsigned long microSeconds;
int counter =25;


ISR(TIMER1_COMPA_vect){
  milliSeconds++; // Increment milliseconds counter
}
ISR(TIMER0_COMPA_vect) {
  microSeconds++; // Increment microseconds counter
}

void delay_ms(unsigned long ms){
  unsigned long current_ms = milliSeconds;
  while(true){
    if(milliSeconds - current_ms >= ms){
      break;
    }
    asm("nop");
  }
}

void delay_us(unsigned long us){
  unsigned long current_us = microSeconds;
  while(true){
    if(microSeconds - current_us >= us){
      break;
    }
    asm("nop");
  }
}


void print(char byte){
  // wait for empty transmit buffer
  while (!(UCSR0A & (1 << UDRE0)));
  // put data into buffer, sends the data
  UDR0 = byte;
}

void printInt(int x) {
  
  char buffer[20]; 
  itoa(x, buffer, 10); 

  // print each character
  for (int i = 0; buffer[i] != '\0'; i++) {
    print(buffer[i]);
  }
  print('\n'); //print new line
}

void setup_uart(){
  // set baud rate to 9600
  UBRR0H = 0;
  UBRR0L = 103;
  // enable receiver and transmitter
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  // set frame format: 8data, 1stop bit
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


void setup() {
  DDRB = 0;
  DDRB |=  (1<<5);

  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;

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
  

  //set Timer 0 for count microseconds
  //calculating OCR0A
  //1us 1MHz
  //OCR0A = (16,000,000/(pre scalar*1,000,000)) - 1 
  //OCR0A = (16,000,000/(1*1,000,000)) - 1 = 15
  //OCR0A = (16,000,000/(8*1,000,000)) - 1 = 1
  //OCR0A = (16,000,000/(64*1,000,000)) - 1 = -0.75
  //OCR0A = (16,000,000/(256*1,000,000)) - 1 = -0.9375
  //OCR0A = (16,000,000/(1024*1,000,000)) - 1 = -0.984375
  //lets choose 8 as the pre scalar then OCR2A = 1
  OCR0A = 15;
  TCCR0A |= (1 << WGM01); // Configure Timer0 for CTC mode
  TCCR0B |= (1 << CS00); // Start Timer0 with no prescaler
  TIMSK0 |= (1 << OCIE0A); // Enable CTC interrupt

  sei(); //enable glpbal interrupts

  // configure UART
  setup_uart();

}

void loop() {

  PORTB |= (1<<5);
  printInt(counter);
  delay_us(1000000);
  
  PORTB &= ~(1<<5);
  
  delay_us(1000000);

   
}
