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
//1|END|EPPROM|ADRESS|SAVE
//1|4|S13|W|IS

#include <IRremote.h>
#include "Dimmer.h"
#include <EEPROM.h>

Dimmer dimmer(5, DIMMER_RAMP, 1.5);
IRsend irsend;

String msg;
bool ax = true;
boolean stringComplete = false;  // whether the string is complete
String readString = "";
const int CONTROL = 4;
const String ADDRESS = "5";

const int TAM_SAIDAS = 8;
const int SAIDAS[] = {
  9, 10, 11, 12, A0, A1, A2, A3
};

unsigned long previousMillis = 0;
const long interval = 5000;

int addr = 0;

void setup() {
  Serial.begin(9600);

  dimmer.begin();

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

}

void loop() {
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
            if (aux == "TV") {

              int count;
              count = countData(readString, '|');
              count = count - 3;
              int tam = count / 2;
              int pos = 3;
              for (int i = 0; i < tam; i++) {
                int khz = getValue(readString, '|', pos).toInt();
                pos++;
                String ir_signal = getValue(readString, '|', pos);
                sendIr(ir_signal, '*', khz);
                pos++;
                if (tam != 0)
                  delay(1000);
              }
            }
            else {
              if (aux == "D") {
                aux = getValue(readString, '|', 3);
                int newValueDimmer = aux.toInt();
                dimmer.set(newValueDimmer);
                if (newValueDimmer == 0) {
                  enviaMsg("Y|3|2|0");
                } else {
                  enviaMsg("Y|3|2|1");
                }
              } else {
                if (aux == "EPPROM") {
                  aux = getValue(readString, '|', 3);
                  int address = aux.toInt();
                  aux = getValue(readString, '|', 4);
                  int newValue = aux.toInt();
                  EEPROM.write(address, newValue);
                } else {
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
        }
      }
      readString = "";
      ax = true;
      stringComplete = false;
    }
  }
}

/***
   Quebra a String a partir da posição do caractere separador desejado(pos)
   Separa o protocolo AUTOHOME dos RAWs
   obs: Utilizado em conjunto com a biblioteca IRremote
   raw_tam = Tamanho da quantidade de bits(IRREMOTE)
   khz= Frequencia(IRREMOTE)
   Exemplo: data="1|3|TV|RAW|62|36|2700|850|450"
   Com pos = 6, irá processar a partir do 2700
*/
void sendIr(String data, char separator, int khz)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  int index_array = 0;
  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
    }
  }
  unsigned int irSignal[found];
  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
      String a = data.substring(strIndex[0], strIndex[1]);
      irSignal[index_array] = a.toInt();
      index_array++;
    }
  }

  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
}

int countData(String data, char separator)
{

  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
    }
  }
  return found;
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
        if (cmd == "1") {
          digitalWrite(SAIDAS[6], LOW);
          delay(1000);
          digitalWrite(SAIDAS[6], HIGH);
          enviaMsg("Y|3|3|1");
        }
        else {
          if (cmd == "0") {
            digitalWrite(SAIDAS[7], LOW);
            delay(1000);
            digitalWrite(SAIDAS[7], HIGH);
            enviaMsg("Y|3|3|0");
          }
          else {
            if (cmd == "P") {
              digitalWrite(SAIDAS[7], LOW);
              digitalWrite(SAIDAS[6], LOW);
              delay(1000);
              digitalWrite(SAIDAS[7], HIGH);
              digitalWrite(SAIDAS[6], HIGH);
            }
          }
        }
      } else {
        if (dispositivo == "SC2") {
          if (cmd == "1") {
            digitalWrite(SAIDAS[6], LOW);
            delay(1000);
            digitalWrite(SAIDAS[6], HIGH);
            int value = EEPROM.read(addr);
            value = value * 100;
            delay(value);
            digitalWrite(SAIDAS[7], LOW);
            digitalWrite(SAIDAS[6], LOW);
            delay(1000);
            digitalWrite(SAIDAS[7], HIGH);
            digitalWrite(SAIDAS[6], HIGH);
          }
          else {
            if (cmd == "0") {
              digitalWrite(SAIDAS[7], LOW);
              delay(1000);
              digitalWrite(SAIDAS[7], HIGH);
            }
          }
        } else {
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
          if (dispositivo == "S1") {
            msg = "Y|3|1|";
            msg = msg + !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
            enviaMsg(msg);
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
  delay(250);
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
