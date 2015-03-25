int inPin = 2; // insta-pulse input pin
int outPin = 13; // LED

int bpm = 0; // beats per minute
float ibi = 0.0; // inter-beat interval
unsigned long time = 0; // inter-beat interval counter

int sensorVal = LOW; // sensor value, HIGH or LOW

char buffer[64] = {0}; // output buffer for debugging

void setup() {
  //start serial connection, go fast
  Serial.begin(115200);
 
  pinMode(inPin, INPUT); // insta-pulse input pin
  pinMode(outPin, OUTPUT); // LED pin
  
  // BEGIN TIMER SETUP
  cli();//stop interrupts

  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;// initialize counter value to 0;
  // set timer count for 1khz increments
  OCR1A = 1999;// = (16*10^6) / (1000*8) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();// allow interrupts
  // END TIMER SETUP
 
}


ISR(TIMER1_COMPA_vect) { // Interrupt at freq of 1kHz to measure insta-pulse
  cli(); // stop interrupts
  
  time++; // increment by 1kz = .001 second
  
  if(time > 250){ // debounce: 250 = a heart rate of about 240 bpm

    sensorVal = digitalRead(inPin); // read the insta-pulse
   
    if (sensorVal) { // if pulse detected
      
      ibi = float(time)/1000.0; // calculate inter-beat interval in seconds
      bpm = int(60.0/ibi); // calculate beats per minute
      
      Serial.println(bpm); // print out bpm
      digitalWrite(outPin,HIGH);  // blink the LED
      
      time = 0; // reset the ibi counter
      
    } else {
      digitalWrite(outPin,LOW); // kill the LED
    } 
  }
 
  sei(); // allow interrupts
}

void loop() {
    // do other stuff here, control lights, sound, whatever...
}
