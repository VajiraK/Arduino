/*
WaterHeater_V1
17 Aug 2021
*/

int const pin_relay = 4;
int const pin_led = 3;
int const count = 84;
int const pause_relay = 1000;
int const pause_led = 100;
bool boiled = false;

void setup() {
  pinMode(pin_relay, OUTPUT);
  pinMode(pin_led, OUTPUT);
}

void loop() {
  if(boiled == false)
    digitalWrite(pin_relay, HIGH);

  WaitForHeatup();

  if(boiled == false)
    digitalWrite(pin_relay, LOW); 
    
  boiled = true;
}

void WaitForHeatup()
{
  int p;
  
  if(boiled == false)
    p = pause_relay;
   else
    p = pause_led;
    
  for(int i=0;i<count;i++)
  {
    digitalWrite(pin_led, HIGH); 
    delay(p);
    digitalWrite(pin_led, LOW);
    delay(p);
  }
}
