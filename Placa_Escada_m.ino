/*****************************************************************************************
   AUTOHOME - AUTOMAÇÃO RESIDENCIAL
 *                                                                                        *
   Módulo Escravo de Automação Externa
   Endereço Escravo - 4
   Obs: Aclopado ao Módulo de rele com 16 canais
   Cliente - Saturnino José dos Santos
   Cidade - Ubatã
 ******************************************************************************************/


boolean stringComplete = false;  // whether the string is complete

const int CONTROL = 4;
const String ADDRESS = "4";

const int SAIDAS[] = {
  9, 8, 10, 7, 11, 6, 12, 5, 2, A0, 3, A1, A4, A2, A5
};
const int TAM_SAIDAS = 15;

int Saida1State = LOW;

int controllSaida1 = 0;

boolean readingSOS = 0;
String readString = "";

bool serialEv = true;
String msg;

bool ax = true;


void setup() {
  delay(2000);
  Serial.begin(9600);

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

  //Relembrar Estado das Saídas
  //Caso o arduino se reinicie
  //delay(1000);
  //enviaMsg("CHECK|3");
}

void loop() {

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
      readString = "";
      ax = true;
      stringComplete = false;
    }
  }
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
  else if (dispositivo == "S10") {
    return 8;
  }
  else if (dispositivo == "S11") {
    return 9;
  }
  else if (dispositivo == "S12") {
    return 10;
  }
  else if (dispositivo == "S13") {
    return 11;
  }
  else if (dispositivo == "S14") {
    return 12;
  }
  else if (dispositivo == "S15") {
    return 13;
  }
  else if (dispositivo == "S16") {
    return 14;
  }
  return 0;

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

      if (dispositivo == "S1") { //Sala Central
        msg = "Y|1|1|";
        msg = msg + !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
        enviaMsg(msg);
        delay(250);
      }
      else if (dispositivo == "S4") { // Spots Sala Visitas
        msg = "Y|1|3|";
        msg = msg + !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
        enviaMsg(msg);
        delay(250);
      }
      else if (dispositivo == "S3") { //Spots Mezanino
        msg = "Y|1|2|";
        msg = msg + !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
        enviaMsg(msg);
        delay(250);
      }
      else if (dispositivo == "S2") { //Plafon Torre
        msg = "Y|2|1|";
        msg = msg + !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
        enviaMsg(msg);
        delay(250);
      }
      else if (dispositivo == "S12") { //Rua
        msg = "Y|2|2|";
        msg = msg + !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
        enviaMsg(msg);
        delay(250);
      }
      else if (dispositivo == "S6") { //Garagem
        msg = "Y|2|3|";
        msg = msg + !digitalRead(SAIDAS[getDispositivo(dispositivo)]);
        enviaMsg(msg);
        delay(250);
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

void serialEvent() {
  while (Serial.available()) {
    //digitalWrite(SAIDAS[0], !digitalRead(SAIDAS[0]));
    char inChar = (char)Serial.read();
    if (inChar == '@' && ax) {
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
