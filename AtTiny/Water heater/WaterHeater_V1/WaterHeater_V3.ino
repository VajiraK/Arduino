/*
WaterHeater_V3
Sketch uses 722 bytes (70%) of program storage space. Maximum is 1024 bytes.
Global variables use 15 bytes (23%) of dynamic memory, 
leaving 49 bytes for local variables. Maximum is 64 bytes.
26 Aug 2021

Measured 
0.4L - 1.20 - 80s
0.5L - 1.43 - 103s
1.0L - 3.20 - 200s
1.5L - 4.45 - 285s

Predictions - https://mycurvefit.com/
y = 37569940 + (-21.4057 - 37569940)/(1 + (x/3618522)^0.7972852)

0.375  - 80s
0.75  - 155s
1.125   - 222s
1.5   - 285s
*/

#define anim_speed 200
#define dataPin PB0
#define latchPin PB1
#define clockPin PB2
#define pin_interrupt PB3
//#define pin_dot PB3
#define pin_relay PB4
//#define clockPin PB5//Don't use reset pin
#define MODE_START 0
#define MODE_STOP 5
#define BUTTON_PIN_BITMASK B00001000

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
  DDRB |= (1 << pin_relay);//pinMode(pin_relay, OUTPUT);
  //DDRB |= (1 << pin_dot);//pinMode(pin_dot, OUTPUT);
  
  //Setup button interrupt
  SetPinChangeInterrupt();
  
  //Show zero
  printNum(0);
}
//---------------------------------------
void SetPinChangeInterrupt()
{
  //cli();// Disable interrupts during setup
  pinMode(pin_interrupt, INPUT_PULLUP);
  GIMSK |= (1 << PCIE);//turns on pin change interrupts
  PCMSK |= (1 << PCINT3);//Interup pin is PB3
  //sei();//last line of setup - enable interrupts after setup
}
//---------------------------------------
void loop() 
{
  //Wait till user input
  while(mode == 0)
    delay(1);

  //Turn on relay
  PORTB |= (1 << pin_relay);
  
  WaitForHeatUp();
  
  //Turn off relay
  PORTB &= ~(1 << pin_relay);
  mode = MODE_STOP;
  
  RoundAndRound();
}
//---------------------------------------
void WaitForHeatUp()
{
  for(int i=0;i<active_duration;i++)
  {
    //PORTB |= (1 << pin_dot);
    delay(pause_relay);
    //PORTB &= ~(1 << pin_dot);
    delay(pause_relay);
  }
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
    
    //Re-heating
    if(mode == 1)
    {
      InitializeMode();
      return;
    }
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
//---------------------------------------
ISR (PCINT0_vect)
{//Raise for both H/L
  //Get button pin state
  byte bState = (PINB >> pin_interrupt & BUTTON_PIN_BITMASK >> pin_interrupt);
  //int button = digitalRead(pin_interrupt);
  if(bState == LOW)
    ButtonPressed();
}
