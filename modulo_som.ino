/*****************************************************************************************
   AUTOHOME - AUTOMAÇÃO RESIDENCIAL
 *                                                                                        *
   Módulo Escravo de Automação Externa
   Endereço Escravo - 5
   Obs: Aclopado ao Módulo de rele com 8 canais
   Cliente - Saturnino José dos Santos
   Cidade - Ubatã
 ******************************************************************************************/

//1|3*4|C|-|S1*S3*S2*S4*S6|S13*S7*S12*S8|S2*S11*S14
//1|4|S13|W|IS
#include <Thread.h>
#include <ThreadController.h>

String msg;
bool ax = true;
boolean stringComplete = false;  // whether the string is complete
String readString = "";
const int CONTROL = 4;
const String ADDRESS = "6";

const int TAM_SAIDAS = 8;
const int SAIDAS[] = {
  9, 10, 11, 12, A0, A1, A2, A3
};

unsigned long previousMillis = 0;
const long interval = 5000;

const int CORTINA_ABRIR = 5;
const int CORTINA_FECHAR = 6;

const int ALARME = 7;
const int SENSOR = 8;

Thread myThread = Thread();

void setup() {
  Serial.begin(9600);

  //Cortina
  pinMode(CORTINA_ABRIR, OUTPUT);
  pinMode(CORTINA_FECHAR, OUTPUT);
  digitalWrite(CORTINA_ABRIR, LOW);
  digitalWrite(CORTINA_FECHAR, LOW);


  //Saídas Rele
  for (int i = 0; i < TAM_SAIDAS; i++) {
    pinMode(SAIDAS[i], OUTPUT);
  }

  //Inicializa Desligado
  for (int i = 0; i < TAM_SAIDAS; i++) {
    digitalWrite(SAIDAS[i], HIGH);
  }

  //CONTROL RS485
  pinMode(CONTROL, OUTPUT);
  digitalWrite(CONTROL, LOW);

  //Alarme
  pinMode(ALARME,INPUT);

  //Sensor
  pinMode(SENSOR,INPUT);
  
  // Configure myThread
  myThread.onRun(startCortina);
  myThread.setInterval(1000);
  
}

String cmdCortina;
int countCortina = 0;
bool runThread = false;

void startCortina() {
  if (runThread) {
    if (countCortina == 0) {
      enviaMsg("@1|4|S4|W|1#");
    } else {
      if (countCortina <= 3) {
        if (cmdCortina == "1") {
          digitalWrite(CORTINA_ABRIR, HIGH);
          delay(500);
          digitalWrite(CORTINA_ABRIR, LOW);
        } else {
          digitalWrite(CORTINA_FECHAR, HIGH);
          delay(500);
          digitalWrite(CORTINA_FECHAR, LOW);
        }
      } else {
        if (countCortina == 45) {
          enviaMsg("@1|4|S4|W|0#");
          countCortina = -1;
          runThread = false;
        }
      }
    }
    countCortina++;
  }
}




void loop() {
  if(myThread.shouldRun())
    myThread.run();
    
  if (!ax) {
    if (millis() - previousMillis  >= interval) {
      ax = true;
      readString = "";
      stringComplete = false;
    }
  }
  if (stringComplete) {
    //1|"END"|TV|RAW|"RAW(TAM)"|"KHZ"|"SIGNAL"
    if (readString.length() > 0) {
      String aux = getValue(readString, '|', 0);
      if (aux == "1") { //É uma String válida?
        aux = getValue(readString, '|', 1);
        int pos = hasMyAddress(aux, '*');
        if (pos != -1) { //É o meu endereço escravo?
          aux = getValue(readString, '|', 2);
          if (aux == "C") { //Cenas?
            int count;
            count = pos + 1;
            count = count + pos;
            aux = getValue(readString, '|', count);//Liga dispositivos
            if (aux != "-") {
              changeOutput(aux, '*', "1");
            }
            count++;
            aux = getValue(readString, '|', count);//Desliga dispositivos
            if (aux != "-") {
              changeOutput(aux, '*', "0");
            }
          }
          else {
            if (aux == "T") {
              aux = getValue(readString, '|', 3);
              if (aux == "A") {
                digitalWrite(CORTINA_ABRIR, HIGH);
                delay(500);
                digitalWrite(CORTINA_ABRIR, LOW);
              }
              else {
                if (aux == "F") {
                  digitalWrite(CORTINA_FECHAR, HIGH);
                  delay(500);
                  digitalWrite(CORTINA_FECHAR, LOW);
                }
              }
            }
            else {
              aux = getValue(readString, '|', 3);   //Escrita ou leitura?
              String dispositivo = getValue(readString, '|', 2);   //Qual Dispositivo?
              if (aux == "W") {
                aux = getValue(readString, '|', 4);   //Liga ou Desliga?
                changeOutput(dispositivo, '*', aux);
              }
              else {
                if (aux == "R") {
                  readOutputs(dispositivo, '*');
                }

              }
            }

          }
        }
      }
      readString = "";
      ax = true;
      stringComplete = false;
    }
  }
  debounceAlarme();
  debounceSensor();
}

int buttonState;             
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled



void debounceAlarme(){

  int reading = digitalRead(ALARME);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > 5000) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        digitalWrite(SAIDAS[getDispositivo("S1")], !digitalRead(SAIDAS[getDispositivo("S1")]));
         enviaMsg("ALARME|6");
      }
    }
  }
  lastButtonState = reading;
}


int buttonState2;             
int lastButtonState2 = LOW;
unsigned long lastDebounceTime2 = 0; 

void debounceSensor(){

  int reading = digitalRead(SENSOR);

  if (reading != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > 1000) {
    if (reading != buttonState2) {
      buttonState2 = reading;
      if (buttonState2 == HIGH) {
         enviaMsg("SENSOR|6");
      }
    }
  }
  lastButtonState2 = reading;
}

int getDispositivo(String dispositivo) {

  if (dispositivo == "S1") {
    return 0;
  }
  else if (dispositivo == "S2") {
    return 1;
  }
  else if (dispositivo == "S3") {
    return 2;
  }
  else if (dispositivo == "S4") {
    return 3;
  }
  else if (dispositivo == "S5") {
    return 4;
  }
  else if (dispositivo == "S6") {
    return 5;
  }
  else if (dispositivo == "S7") {
    return 6;
  }
  else if (dispositivo == "S8") {
    return 7;
  }

  return 0;

}


void readOutputs(String data, char separator)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  String readDisp = "";
  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
      String dispositivo = data.substring(strIndex[0], strIndex[1]);
      int r = !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
      if (readDisp == "") {
        readDisp = readDisp + dispositivo;
        readDisp = readDisp + ":";
        readDisp = readDisp + r;
      } else {
        readDisp = readDisp + "*";
        readDisp = readDisp + dispositivo;
        readDisp = readDisp + ":";
        readDisp = readDisp + r;
      }
    }
  }
  msg = "READ|";
  msg = msg + ADDRESS;
  msg = msg + "|" + readDisp;
  enviaMsg(msg);
}



void enviaMsg(String msg) {
  digitalWrite(CONTROL, HIGH);
  delay(100);
  Serial.println(msg);
  delay(100);
  digitalWrite(CONTROL, LOW);
  Serial.flush();
  delay(300);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


int hasMyAddress(String data, char separator)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
      String addressed = data.substring(strIndex[0], strIndex[1]);
      if (addressed == ADDRESS) {
        return found;
      }
    }
  }
  return -1;
}


void changeOutput(String data, char separator, String cmd)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  String readDisp = "";

  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
      String dispositivo = data.substring(strIndex[0], strIndex[1]);
      if (dispositivo == "SC") {
        cmdCortina=cmd;
        countCortina=0;
        runThread = true;
      } else {
        if (dispositivo == "S7") {
          if (cmd == "0") {
            digitalWrite(SAIDAS[getDispositivo("S1")], HIGH);
            digitalWrite(SAIDAS[getDispositivo("S2")], HIGH);
            digitalWrite(SAIDAS[getDispositivo("S3")], HIGH);
            digitalWrite(SAIDAS[getDispositivo("S4")], HIGH);
            digitalWrite(SAIDAS[getDispositivo("S5")], HIGH);
          }
        }
        if (cmd == "1") {
          if (digitalRead(SAIDAS[getDispositivo(dispositivo)]) != LOW) {
            digitalWrite(SAIDAS[getDispositivo(dispositivo)], LOW);
          }
        }
        else {
          if (cmd == "0") {
            if (digitalRead(SAIDAS[getDispositivo(dispositivo)]) != HIGH) {
              digitalWrite(SAIDAS[getDispositivo(dispositivo)], HIGH);
            }
          }
          else {
            if (cmd == "IS") {
              digitalWrite(SAIDAS[getDispositivo(dispositivo)], !digitalRead(SAIDAS[getDispositivo(dispositivo)]));
            }
          }
        }

      }

      int r = !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
      if (readDisp == "") {
        readDisp = readDisp + dispositivo;
        readDisp = readDisp + ":";
        readDisp = readDisp + r;
      } else {
        readDisp = readDisp + "*";
        readDisp = readDisp + dispositivo;
        readDisp = readDisp + ":";
        readDisp = readDisp + r;
      }
    }
  }
  msg = "READ|";
  msg = msg + ADDRESS;
  msg = msg + "|" + readDisp;
  enviaMsg(msg);
}


void serialEvent() {
  while (Serial.available()) {
    //digitalWrite(SAIDAS[0], !digitalRead(SAIDAS[0]));
    char inChar = (char)Serial.read();

    if (inChar == '@' && ax) {
      previousMillis = millis();
      ax = false;
    }
    if (!ax) {
      if (inChar == '#') {
        stringComplete = true;
      }
      else {
        if (inChar != '@') {
          readString += inChar;
        }
      }
    }
  }
}
