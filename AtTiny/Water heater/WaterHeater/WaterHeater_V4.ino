/*
WaterHeater_V4
20 Sep 2021
*/

#define MAX_MASTER_COUNT 25
#define MAX_FINAL_COUNT 3
#define STATUS_READY 0
#define STATUS_STARTED 1
#define STATUS_REACHED 2
#define STATUS_STOPED 3

#define anim_speed 200
#define sensorPin A2
#define dataPin PB0
#define latchPin PB1
#define clockPin PB2
#define pin_relay PB3

int masterCount = 0;
byte finalCount = 0;
int status = STATUS_READY;
//---------------------------------------
void setup() {
  DDRB |= (1 << latchPin);//pinMode(latchPin, OUTPUT);
  DDRB |= (1 << dataPin);//pinMode(dataPin, OUTPUT);
  DDRB |= (1 << clockPin);//pinMode(clockPin, OUTPUT);
  DDRB |= (1 << pin_relay);//pinMode(pin_relay, OUTPUT);
  
  /*ADC Multiplexer Selection Register
  Bit 6 – REFS0: Reference Selection Bit
  Bit 5 – ADLAR: ADC Left Adjust Result
  Bits 1:0 – MUX[1:0]: Analog Channel Selection Bits*/
  ADMUX = B00100010;

  /*ADC Control and Status Register A
  Bit 7 – ADEN: ADC Enable
  Bit 6 – ADSC: ADC Start Conversion
  Bit 5 – ADATE: ADC Auto Trigger Enable
  Bit 4 – ADIF: ADC Interrupt Flag
  Bit 3 – ADIE: ADC Interrupt Enable
  Bits 2:0 – ADPS[2:0]: ADC Prescaler Select Bits*/
  ADCSRA = B11000010;
  
  ReadTemperature();
  
  //Give some time to stabilize ADC
  delay(1000);
}
//---------------------------------------
void loop() {
  
  masterCount++;
  
  if(status == STATUS_READY)
  {//Turn on relay
	PORTB |= (1 << pin_relay);
	status = STATUS_STARTED;
  }
  
  if(status != STATUS_STOPED)
  {
	  if(status != STATUS_REACHED)
	  {//Bellow 97 Celsius
		  byte temp = ReadTemperature();
		  SetStatus(temp);
		  //Get 10th position
		  temp = temp/10;
		  //We can only display one digit
		  if(temp > 9) temp = 9;
		  printNum(temp);
	  }
	  else
	  {//Above 97 Celsius
		if(finalCount++ == MAX_FINAL_COUNT)
		{
			status = STATUS_STOPED;
			//Turn off relay
			PORTB &= ~(1 << pin_relay);
		}
	  }
  }
  else
  {
	  RoundAndRound();
  }
    
  delay(1000);
}
//---------------------------------------
void SetStatus(byte temp)
{
  if(masterCount == MAX_MASTER_COUNT)
	status = STATUS_STOPED;//Something wrong - let's turn off.

  if(temp > 97)
	  status = STATUS_REACHED;
}
//---------------------------------------
byte ReadTemperature()
{
	  //ADC Start Conversion
	  ADCSRA |= (1 << ADSC); 
	  WaitForAdcComplete();
	  return getLeftAdjustResult();
}
//---------------------------------------
void WaitForAdcComplete()
{
  //Wait till Bit 4 – ADIF is set(complete ADC)
  byte flag = 0;
  do{
  	flag = ADCSRA & B00010000;
  }while(flag == 0);
}
//---------------------------------------
byte getLeftAdjustResult(){

  //ADCL must be read first
  byte _ADCL = ADCL;
  byte _ADCH = ADCH; 

  //First half
  int rawData = 0;
  rawData |= _ADCH;
  rawData = rawData << 2;
  //Second half
  _ADCL = _ADCL >> 6;
  rawData |= _ADCL;
  
  return toCelsius(rawData);
}
//---------------------------------------
float toCelsius(int raw) {
  //reads data, converts to degree celsius
  float volt = raw * 5.0;
  volt /= 1024.0;
  float temp = (volt - 0.5) * 100;
  return temp;
}
//---------------------------------------
void RoundAndRound()
{
  //PORTB |= (1 << pin_dot);
  
  while(true)
  {
    printNum(B11101111);
    delay(anim_speed);
    printNum(B11110111);
    delay(anim_speed);
    printNum(B11111011);
    delay(anim_speed);
    printNum(B01111111);
    delay(anim_speed);
    printNum(B10111111);
    delay(anim_speed);
    printNum(B11011111);
    delay(anim_speed);
  }
}
//---------------------------------------
byte printNum(byte digits)
{
  PORTB &= ~(1 << latchPin);//digitalWrite(latchPin, LOW);

  digits = getBits(digits);
  shiftOut(dataPin, clockPin, MSBFIRST, digits);
  
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
