int latchPin = 1;//BP2
int clockPin = 2;//BP0
int dataPin = 0;//BP1

/*
WaterHeater_V3
26 Aug 2021
*/
int const anim_speed = 200;
//#define dataPin PB1
//#define latchPin PB2
#define pin_interrupt PB2
#define pin_dot PB3
#define pin_relay PB4
//#define clockPin PB0
#define MODE_START 0
#define MODE_STOP 5

byte mode = MODE_START;
int active_duration = 0;
int const heatup_durations[4] = {10, 20, 30, 40};
int const pause_relay = 1000;
int const pause_dot = 1000;

//---------------------------------------
void setup()
{
  DDRB |= (1 << latchPin);//pinMode(latchPin, OUTPUT);
  DDRB |= (1 << dataPin);//pinMode(dataPin, OUTPUT);
  DDRB |= (1 << clockPin);//pinMode(clockPin, OUTPUT);
  
}
//---------------------------------------
void loop() 
{
  
  
 
  
  RoundAndRound();
}
//---------------------------------------
void WaitForHeatUp()
{
  for(int i=0;i<active_duration;i++)
  {
    PORTB |= (1 << pin_dot);
    delay(pause_relay);
    PORTB &= ~(1 << pin_dot);
    delay(pause_relay);
  }
}
//---------------------------------------
void RoundAndRound()
{
 
  
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
  }

  PORTB &= ~(1 << latchPin);
  shiftOut(dataPin, clockPin, MSBFIRST, n);
  PORTB |= (1 << latchPin);
}
//---------------------------------------
void ButtonPressed() {
  if(mode == MODE_START)
  {//First press - Default mode 2
    mode = 2;
  }
  else if(mode == MODE_STOP)
  {//Re-heat
    mode = 1;
    return;//Escape from RoundAndRound()
  }
  else
  {
    mode++;
    //Wraparound
    if(mode > 4)
      mode = 1;
  }

  InitializeMode();
}
//---------------------------------------
void InitializeMode()
{
  active_duration = heatup_durations[mode - 1];
  printNum(mode);
}
