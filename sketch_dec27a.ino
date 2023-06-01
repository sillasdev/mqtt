
int latchPin = 2; //pin 12
int dataPin = 3; //pin 3
int clockPin = 4; //pin 13
int d1=50;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  
  //0
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 63);
  digitalWrite(latchPin, HIGH);
  delay(1000);

  //1
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 6);
  digitalWrite(latchPin, HIGH);
  delay(1000);
  
  //2
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 91);
  digitalWrite(latchPin, HIGH);
  delay(1000);

  //3
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 79);
  digitalWrite(latchPin, HIGH);
  delay(1000);
  
  //4
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 102);
  digitalWrite(latchPin, HIGH);
  delay(1000);  
  
  //5
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 109);
  digitalWrite(latchPin, HIGH);
  delay(1000);

  //6
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 125);
  digitalWrite(latchPin, HIGH);
  delay(1000);

  //7
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 7);
  digitalWrite(latchPin, HIGH);
  delay(1000);

  //8
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 127);
  digitalWrite(latchPin, HIGH);
  delay(1000);

  //9
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 111);
  digitalWrite(latchPin, HIGH);
  delay(1000);
 

}
