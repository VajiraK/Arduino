int latchPin = 1;//BP2
int clockPin = 2;//BP0
int dataPin = 0;//BP1


void setup()
{
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

void loop()
{
  printNum(B00000011);//0
  printNum(B10011111);//1
  printNum(B00100101);//2
  printNum(B00001101);//3
  printNum(B10011001);//4
  printNum(B01001001);//5
  printNum(B01000001);//6
  printNum(B00011111);//7
  printNum(B00000001);//8
  printNum(B00001001);//9
  
}

void printNum(byte n)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, n);
  digitalWrite(latchPin, HIGH);
  delay(1000);
}
