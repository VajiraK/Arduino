/*
WaterHeater_V4
20 Sep 2021
*/

const int 	sensorPin	=  A2;
int const dataPin = PB0;//pin 0
int const latchPin = PB1;//pin 1
int const clockPin = PB2;//pin 2
//---------------------------------------
void setup() {
  DDRB |= (1 << latchPin);//pinMode(latchPin, OUTPUT);
  DDRB |= (1 << dataPin);//pinMode(dataPin, OUTPUT);
  DDRB |= (1 << clockPin);//pinMode(clockPin, OUTPUT);
}
//---------------------------------------
void loop() {
  /*ADC Multiplexer Selection Register
  Bit 6 – REFS0: Reference Selection Bit
  Bit 5 – ADLAR: ADC Left Adjust Result
  Bits 1:0 – MUX[1:0]: Analog Channel Selection Bits*/
  ADMUX = B00000010;
  
  /*ADC Control and Status Register A
  Bit 7 – ADEN: ADC Enable
  Bit 6 – ADSC: ADC Start Conversion
  Bit 5 – ADATE: ADC Auto Trigger Enable
  Bit 4 – ADIF: ADC Interrupt Flag
  Bit 3 – ADIE: ADC Interrupt Enable
  Bits 2:0 – ADPS[2:0]: ADC Prescaler Select Bits*/
  ADCSRA = B11000110;

  //Wait till Bit 4 – ADIF is set(complete ADC)
  byte flag = 0;
  do{
  	flag = ADCSRA & B00010000;
  }while(flag == 0);
  
  //ADCL must be read first
  byte _ADCL = ADCL;
  byte _ADCH = ADCH; 

  //First half
  int rawData = 0;
  rawData |= _ADCH;
  rawData = rawData << 8;
  //Second half
  rawData |= ADCL;
  
  int temp = toCelsius(rawData);
  printTwoDigits(temp);
  
  delay(1000);
}
//---------------------------------------
//reads data, converts to degree celsius
float toCelsius(int raw) {
  float volt = raw * 5.0;
  volt /= 1024.0;
  float temp = (volt - 0.5) * 100;
  return temp;
}
//---------------------------------------
byte printTwoDigits(byte digits)
{
  PORTB &= ~(1 << latchPin);//digitalWrite(latchPin, LOW);
  
  byte n = getBits(digits%10);
  shiftOut(dataPin, clockPin, MSBFIRST, n);
  
  n = getBits(digits/10);
  shiftOut(dataPin, clockPin, MSBFIRST, n);
  
  PORTB |= (1 << latchPin);//digitalWrite(latchPin, HIGH);
}
//---------------------------------------
byte getBits(byte n)
{
  switch(n)
  {
    case 0:
    	n = B00000011;
    break;
    case 1:
    	n = B10011111;
    break;
    case 2:
    	n = B00100101;
    break;
    case 3:
    	n = B00001101;
    break;
    case 4:
    	n = B10011001;
    break;
    case 5:
    	n = B01001001;
    break;
    case 6:
    	n = B01000001;
    break;
    case 7:
    	n = B00011111;
    break;
    case 8:
    	n = B00000001;
    break;
    case 9:
    	n = B00001001;
    break;
  }
  
  return n;
}
//---------------------------------------
