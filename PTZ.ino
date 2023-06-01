#include <SoftwareSerial.h>

char readBluetooth;//Variável que irá receber o valor enviado do celular para o Bluetooth


const byte rxPin =4;
const byte txPin = 3;

SoftwareSerial SerialRS = SoftwareSerial(rxPin, txPin);


const byte C_STOP = 0x00;
const byte C_UP = 0x08;
const byte C_DOWN = 0x10;
const byte C_LEFT = 0x04;
const byte C_RIGHT = 0x02;
const byte C_PRESET = 0x07;
const byte C_SETPRESET = 0x03;
const byte C_FOCUSFAR = 0x80;
const byte C_FOCUSNEAR = 0x00;

const byte C_ZOOMIN = 0x20;
const byte C_ZOOMOUT = 0x40;

const byte C_SET_PAN_POSITION = 0x4B;  // 1/100ths of degree
const byte C_SET_TILT_POSITION = 0x4D; // 1/100ths of degree

const byte address = 0x01; //camera address
 
byte speed = 0x00;

byte broadcast[4] = {0x88, 0x30, 0x01, 0xFF};
byte call1[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X01, 0xFF};
byte call2[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X02, 0xFF};
byte call3[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X03, 0xFF};
byte call4[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X04, 0xFF};
byte call5[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X05, 0xFF};
byte call6[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X06, 0xFF};
byte call7[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X07, 0xFF};
byte call8[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X08, 0xFF};
byte call9[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X09, 0xFF};
byte call10[7] = {0x81, address, 0x04, 0x3F, 0X02, 0X0A, 0xFF};

byte set1[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X01, 0xFF};
byte set2[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X02, 0xFF};
byte set3[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X03, 0xFF};
byte set4[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X04, 0xFF};
byte set5[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X05, 0xFF};
byte set6[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X06, 0xFF};
byte set7[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X07, 0xFF};
byte set8[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X08, 0xFF};
byte set9[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X09, 0xFF};
byte set10[7] = {0x81, address, 0x04, 0x3F, 0X01, 0X0A, 0xFF};


byte down[9] = {0x81, address, 0x06, 0x01, 0X01, 0X01, 0X03, 0X02, 0xFF};
byte up[9] = {0x81, address, 0x06, 0x01, 0X01, 0X01, 0X03, 0X01, 0xFF};
byte left[9] = {0x81, address, 0x06, 0x01, 0X01, 0X01, 0X01, 0X03, 0xFF};
byte right[9] = {0x81, address, 0x06, 0x01, 0X18, 0X17, 0X02, 0X03, 0xFF};
byte stop[9] = {0x81, address, 0x06, 0x01, 0X00, 0X00, 0X03, 0X03, 0xFF};

byte zoomin[6] = {0x81, address, 0x04, 0x07, 0X27, 0xFF};
byte zoomout[6] = {0x81, address, 0x04, 0x07, 0X03, 0xFF};
byte zoomstop[6] = {0x81, address, 0x04, 0x07, 0X00, 0xFF};

byte focusFar[6] = {0x81, address, 0x04, 0x08, 0X03, 0xFF};
byte focusNear[6] = {0x81, address, 0x04, 0x08, 0X02, 0xFF};
byte focusStop[6] =  { 0x81, address, 0x04, 0x08, 0x00, 0xFF };
byte focusManualOn[6] =  { 0x81, address,0x04,0x38, 0x03,0xFF };

byte irisUp[6] =  { 0x81, address,0x04,0x0B, 0x02,0xFF };
byte irisDown[6] =  { 0x81, address,0x04,0x0B, 0x03,0xFF };
byte irisManualOn[6] =  { 0x81, address,0x04,0x39, 0x00,0xFF };

byte teste[6] =  { 0x81, address,0x06,0x31, 0x01,0xFF };
byte teste2[6] =  { 0x81, address,0x06,0x31, 0x02,0xFF };

long debounceDelay = 50;

const int buttonSaida1 = 8;
const int buttonSaida2 = 9;


int Saida1State1 = LOW;
int buttonState1;
int lastButtonState1 = LOW;
long lastDebounceTime1 = 0;

int Saida1State2 = LOW;
int buttonState2;
int lastButtonState2 = LOW;
long lastDebounceTime2 = 0;


void setup(){
   Serial.begin(9600);//Inicia comunicação serial
  SerialRS.begin(9600);
 pinMode(buttonSaida1, INPUT);
pinMode(13, OUTPUT); 
digitalWrite(13, LOW);  
}

void sendPelcoDFrame(byte command, byte data1, byte data2)
{
  byte bytes[7] = {0xFF, address, 0x00, command, data1, data2, 0x00};
  byte crc = (bytes[1] + bytes[2] + bytes[3] + bytes[4] + bytes[5]) % 0x100;
  bytes[6] = crc;

  for (int i = 0; i < 7; i++)
  {
    SerialRS.write(bytes[i]);
    //Serial.print(bytes[i], HEX); //debug
  }
  // Call Preset  0xFF  Address 0x00  0x07  0x00  Preset ID SUM
}

void sendViscaFrame(byte *cmd, int byteSize)
{
  for (int i = 0; i < 4; i++)
  {
    SerialRS.write(broadcast[i]);
  }
  //delay(10);
  for (int i = 0; i < byteSize; i++)
  {
    SerialRS.write(cmd[i]);
  }
}

void sendPelcoDFrame2(byte command, byte data1, byte data2)
{
  byte bytes[7] = {0xFF, address, 0x01, command, data1, data2, 0x00};
  byte crc = (bytes[1] + bytes[2] + bytes[3] + bytes[4] + bytes[5]) % 0x100;
  bytes[6] = crc;

  for (int i = 0; i < 7; i++)
  {
    SerialRS.write(bytes[i]);
    //Serial.print(bytes[i], HEX); //debug
  }
  // Call Preset  0xFF  Address 0x00  0x07  0x00  Preset ID SUM
}

void MIDImessage(byte command, byte data1, byte data2) //pass values out through standard Midi Command
{
   Serial.write(command);
   Serial.write(data1);
   Serial.write(data2);
}
void loop(){
   int reading1 = digitalRead(buttonSaida1);
    if (reading1 != lastButtonState1) {
      lastDebounceTime1 = millis();
  
    }
  
    if ((millis() - lastDebounceTime1) > debounceDelay) {
      if (reading1 != buttonState1) {
        buttonState1 = reading1;
        if (buttonState1 == LOW) {
          //SOLTOU
           sendViscaFrame(teste,sizeof(teste));
        }else{
          //PRESSIONOU
           //sendViscaFrame(focusManualOn,sizeof(focusManualOn));
          // delay(10);
          //sendViscaFrame(focusNear,sizeof(focusNear));
        }
      }
    }

        //DEBOUNCE Saida2
  int reading2 = digitalRead(buttonSaida2);
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == LOW) {
         //SOLTOU
           sendViscaFrame(teste2,sizeof(teste2));
        }else{
          //PRESSIONOU
          //sendViscaFrame(focusManualOn,sizeof(focusManualOn));
          //delay(10);
          //sendViscaFrame(focusFar,sizeof(focusFar));
        }
    }
  }
    lastButtonState1 = reading1;

 lastButtonState2 = reading2;

   
    if(Serial.available()){
      readBluetooth = Serial.read();
      Serial.println(readBluetooth);
      switch (readBluetooth) {
        case 'A':
          digitalWrite(13, HIGH);  
          sendViscaFrame(stop,sizeof(stop));
          break;
        case 'B':
           sendViscaFrame(zoomin,sizeof(zoomin));
          break;
        case 'C':
          sendViscaFrame(zoomout,sizeof(zoomout));
          break;
        case 'D':
          sendViscaFrame(down,sizeof(down));
          break;
        case 'E':
          sendViscaFrame(up,sizeof(up));
          break;
        case 'F':
          sendViscaFrame(left,sizeof(left));
          break;
        case 'G':
          sendViscaFrame(right,sizeof(right));
          break;
        case 'H':
          sendViscaFrame(call1,sizeof(call1));
          break;
        case 'I':
          sendViscaFrame(set1,sizeof(set1));
          break;
        case 'J':
          sendViscaFrame(call2,sizeof(call2));
          break;
        case 'L':
          sendViscaFrame(set2,sizeof(set2));
          break;
        case 'M':
           sendViscaFrame(call3,sizeof(call3));
          break;
        case 'N':
          sendViscaFrame(set3,sizeof(set3));
          break;
        case 'O':
          sendViscaFrame(call4,sizeof(call4));
          break;
        case 'P':
          sendViscaFrame(set4,sizeof(set4));
          break;
        case 'Q':
          sendViscaFrame(call5,sizeof(call5));
          break;
        case 'R':
          sendViscaFrame(set5,sizeof(set5));
          break;
        case 'S':
           sendViscaFrame(call6,sizeof(call6));
          break;                    
        case 'T':
          sendViscaFrame(set6,sizeof(set6));
          break;
        case 'Y':
          sendViscaFrame(call7,sizeof(call7));
          break; 
        case 'W':
          sendViscaFrame(set7,sizeof(set7));
          break;
        case 'a':
           sendViscaFrame(call8,sizeof(call8));
          break;                    
        case 'b':
          sendViscaFrame(set8,sizeof(set8));
          break;
        case 'c':
           sendViscaFrame(call9,sizeof(call9));
          break;                    
        case 'd':
          sendViscaFrame(set9,sizeof(set9));
          break;  
        case 'e':
           sendViscaFrame(call10,sizeof(call10));
          break;                    
        case 'f':
          sendViscaFrame(set10,sizeof(set10));
          break;                       
        case 'U':
          sendViscaFrame(zoomstop,sizeof(zoomstop));
          break;                    
        case 'V':
           sendViscaFrame(focusFar,sizeof(focusFar));
          break;
        case 'X':
          sendViscaFrame(focusNear,sizeof(focusNear));
          break; 
        case 'Z':
          sendViscaFrame(focusStop,sizeof(focusStop));
          break;    
              
      }
        
   }
}
