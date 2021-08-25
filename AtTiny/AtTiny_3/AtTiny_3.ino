int Speaker = 1;

void setup()
{
  pinMode(Speaker, OUTPUT);
}

void loop() {

  for (int i=0; i < 1000; i++) {
    digitalWrite(Speaker, HIGH);
    delayMicroseconds(400);
    digitalWrite(Speaker, LOW);
    delayMicroseconds(400);
  }

  delay(1000);
}
