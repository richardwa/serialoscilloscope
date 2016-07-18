// Arrays to save our results in
#define BUFFERSIZE 1024
#define ADCPIN 2

// Defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

uint8_t values[BUFFERSIZE];
//settings
uint8_t trigger;
boolean start;

void setup() {
  Serial.begin(115200);

  //set reference voltage to internal 5v
  cbi(ADMUX,REFS1); 
  sbi(ADMUX,REFS0); 

  //left align adc output
  sbi(ADMUX,ADLAR); 

  //set ADC pin
  ADMUX |= ( ADCPIN & 0x07 );

  // set up the ADC prescaler
  setADCPrescaler(4);

  //start a read
  readADC();
  
  Serial.println("Oscilloscope ready");
}

uint8_t readADC()
{
    sbi(ADCSRA,ADSC);
    while( !(ADCSRA & _BV(ADIF)) ) ;
    return ADCH;
}

void collect(){
  Serial.println("getting data");
  uint32_t i;
  long start_time = micros();
  for( i=0;i<BUFFERSIZE;i++) {
    values[i] = readADC();
  }
  long stop_time = micros();
  Serial.println("+++ Results (in hexidecimal) +++"); 
  for(i=0;i<BUFFERSIZE;i++) {
    Serial.print(values[i],HEX);
  }
  Serial.println("\n--- Results ---");
  
  Serial.print("\nbuffer[");
  Serial.print(BUFFERSIZE);
  Serial.print("] completed in ");
  Serial.print(stop_time - start_time);
  Serial.println(" micros");
}


void loop() {
  uint8_t val = readADC();
  if (start && val >= trigger){
    collect();
    start = false;
  }
}

void commandRecieved(String cmd){
  char chars[6];
  switch (cmd.charAt(0)) {
      case 'b': //begin
        start = true;
        Serial.print("listening for trigger at ");
        Serial.println(trigger);
        break;
      case 'e': //stop
        start = false;
        Serial.println(" stop listening ");
        break;
      case 't':
        cmd.substring(1).toCharArray(chars, sizeof(chars));
        trigger = (uint8_t) strtol(chars,NULL,HEX);
        Serial.print("trigger set to ");
        Serial.println(trigger);
        break;
      default:
        // Display error message
        Serial.print("ERROR: bad command : ");
        Serial.println(cmd);
  }
}

void serialEventRun(void) {
  String inputString = Serial.readString();  
  if (inputString != ""){
    commandRecieved(inputString);
  }
}


void setADCPrescaler( uint8_t Prescaler )
{
  switch (Prescaler)
  {
  case 2:
    cbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);
    sbi(ADCSRA,ADPS0);
    break;
  case 4:
    cbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);
    cbi(ADCSRA,ADPS0);
    break;
  case 8:
    cbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);
    sbi(ADCSRA,ADPS0);
    break;
  case 16:
    sbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);
    cbi(ADCSRA,ADPS0);
    break;
  case 32:
    sbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);
    sbi(ADCSRA,ADPS0);
    break;
  case 64:
    sbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);
    cbi(ADCSRA,ADPS0);
    break;
  case 128:
    sbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);
    sbi(ADCSRA,ADPS0);
    break;
  default:  // Set 128
    sbi(ADCSRA,ADPS2);
    sbi(ADCSRA,ADPS1);
    sbi(ADCSRA,ADPS0);
  }
}

