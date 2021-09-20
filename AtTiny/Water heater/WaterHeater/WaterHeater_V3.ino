/*
WaterHeater_V3
Sketch uses 778 bytes (75%) of program storage space. Maximum is 1024 bytes.
Global variables use 11 bytes (17%) of dynamic memory,
leaving 53 bytes for local variables. Maximum is 64 bytes.

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
#define pin_relay PB4
//#define clockPin PB5//Don't use reset pin
#define MODE_START 0
#define MODE_STOP 5
#define BUTTON_PIN_BITMASK B00001000
#define PAUSE_RELAY 1000
//volatile prevents compiler optimization
volatile byte timer = 0; 
volatile byte mode = MODE_START;
volatile int active_duration = 0;
volatile int const heatup_durations[4] = {80, 175, 195, 276};

//---------------------------------------
void setup()
{
  DDRB |= (1 << latchPin);//pinMode(latchPin, OUTPUT);
  DDRB |= (1 << dataPin);//pinMode(dataPin, OUTPUT);
  DDRB |= (1 << clockPin);//pinMode(clockPin, OUTPUT);
  DDRB |= (1 << pin_relay);//pinMode(pin_relay, OUTPUT);
  
  //Setup button interrupt
  SetPinChangeInterrupt();
  
  //Show zero
  printNum(0);
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
    delay(PAUSE_RELAY);
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
    
    //Re-heating
    if(mode != MODE_STOP)
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
void SetPinChangeInterrupt()
{
  //cli();// Disable interrupts during setup
  pinMode(pin_interrupt, INPUT_PULLUP);
  GIMSK |= (1 << PCIE);//turns on pin change interrupts
  PCMSK |= (1 << PCINT3);//Interup pin is PB3
  //sei();//last line of setup - enable interrupts after setup
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
  //We need timer-delay for button debouncing
  if(bState == LOW)
    EnableTimer();
}
//---------------------------------------
ISR(TIM0_COMPA_vect)
{
  timer++;
  
  if(timer > 24)
  {
  	ButtonPressed();
    DisableTimer();
  }
}
//---------------------------------------
void EnableTimer()
{
  TCCR0A |= (1 << WGM01); // Set CTC mode on Timer 1
  TIMSK0 |= (1 << OCIE0A); // Enable the Timer/Counter0 Compare Match A interrupt
  TCCR0B = (1<<CS00) | (1<<CS02);//Set prescaler
  //OCR0A = 125;// Set the output compare reg
}
//---------------------------------------
void DisableTimer()
{
    TIMSK0 &= ~(1 << OCIE0A);
    timer = 0;
}


