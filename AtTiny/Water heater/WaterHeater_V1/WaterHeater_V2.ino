/*
WaterHeater_V2
24 Aug 2021
*/

bool boiled = false;
byte dis_update_count = 0;
byte progress = 0;
int const heatup_duration = 5;
int const dis_update_point = 1;
int const pause_relay = 1000;
int const pause_dot = 1000;
int const anim_speed = 300;
int const dataPin = PB0;//pin 0
int const latchPin = PB1;//pin 1
int const clockPin = PB2;//pin 2
int const pin_dot = PB3;//pin 3
int const pin_relay = PB4;//pin 4

//---------------------------------------
void setup()
{
  DDRB |= (1 << latchPin);//pinMode(latchPin, OUTPUT);
  DDRB |= (1 << dataPin);//pinMode(dataPin, OUTPUT);
  DDRB |= (1 << clockPin);//pinMode(clockPin, OUTPUT);
  DDRB |= (1 << pin_relay);//pinMode(pin_relay, OUTPUT);
  DDRB |= (1 << pin_dot);//pinMode(pin_dot, OUTPUT);
}
//---------------------------------------
void loop() {
  printNum(progress);
  PORTB |= (1 << pin_relay);//digitalWrite(pin_relay, HIGH);
  WaitForHeatUp();
  PORTB &= ~(1 << pin_relay);//digitalWrite(pin_relay, LOW);
  RoundAndRound();
}
//---------------------------------------
void WaitForHeatUp()
{
  for(int i=0;i<heatup_duration;i++)
  {
    UpdateDisplay();
    PORTB |= (1 << pin_dot);//digitalWrite(pin_dot, HIGH); 
    delay(pause_relay);
    PORTB &= ~(1 << pin_dot);//digitalWrite(pin_dot, LOW);
    delay(pause_relay);
  }
}
//---------------------------------------
void RoundAndRound()
{
  PORTB |= (1 << pin_dot);//digitalWrite(pin_dot, HIGH);
  
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
void UpdateDisplay()
{
  	if(boiled)
  		return;
  
	dis_update_count++;
  
  	if(dis_update_count == dis_update_point && progress < 9)
    {
      printNum(++progress);
      dis_update_count = 0;
    }
}
//---------------------------------------
void printNum(byte n)
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

  PORTB &= ~(1 << latchPin);//digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, n);
  PORTB |= (1 << latchPin);//digitalWrite(latchPin, HIGH);
}
