/*
WaterHeater_V2
24 Aug 2021
*/

bool boiled = false;
byte dis_update_count = 0;
byte progress = 0;
int const heatup_duration = 30;
int const dis_update_point = 3;
int const pause_relay = 1000;
int const pause_dot = 1000;
int const anim_speed = 300;
int const dataPin = 0;
int const latchPin = 1;
int const clockPin = 2;
int const pin_dot = 3;
int const pin_relay = 4;

//---------------------------------------
void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(pin_relay, OUTPUT);
  pinMode(pin_dot, OUTPUT);
}
//---------------------------------------
void loop() {
  printNum(0);
  digitalWrite(pin_relay, HIGH);
  WaitForHeatUp();
  digitalWrite(pin_relay, LOW);
  RoundAndRound();
}
//---------------------------------------
void WaitForHeatUp()
{
  for(int i=0;i<heatup_duration;i++)
  {
    UpdateDisplay();
    digitalWrite(pin_dot, HIGH); 
    delay(pause_relay);
    digitalWrite(pin_dot, LOW);
    delay(pause_relay);
  }
}
//---------------------------------------
void AfterHeatUp()
{
  while(true)
  {
    printNum(0);
    digitalWrite(pin_dot, HIGH); 
    delay(pause_dot);
    digitalWrite(pin_dot, LOW);
    delay(pause_dot);
  }
}
//---------------------------------------
void RoundAndRound()
{
  digitalWrite(pin_dot, HIGH);
  
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
  
  	if(dis_update_count == dis_update_point)
    {
      printNum(progress++);
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

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, n);
  digitalWrite(latchPin, HIGH);
}
