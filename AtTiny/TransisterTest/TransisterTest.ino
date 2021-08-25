int basePin = 6;

void setup() {
  pinMode(basePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  analogWrite(basePin, 0);
  Serial.println(0.0);
  delay(3000);
  
  for(int i=0;i<256;i+=10)
  {
    analogWrite(basePin, i);
    Serial.print(5.0*(i/256.0));
    delay(500);                   
  }

  analogWrite(basePin, 255);
  Serial.println(5.0);
  delay(3000);
}
