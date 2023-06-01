/*****************************************************************************************
                          AUTOHOME - AUTOMAÇÃO RESIDENCIAL
 *                                                                                        *
                         Placa Escrava de Automação Externa
                              Endereço Escravo - 3
                         Caracteristicas: Possui 6 saidas
                        Cliente - Saturnino José dos Santos
                                 Cidade - Ubatã
 ******************************************************************************************/
//#include <IRremote.h>

boolean stringComplete = false;  // whether the string is complete

const int CONTROL = 2;
const String ADDRESS = "3";

const int buttonSaida1 = 5;
const int buttonSaida2 = 6;
const int buttonSaida3 = 7;


const int SAIDAS[] = {
  A2, A1, A0, 12, 11, 10
};
const int TAM_SAIDAS = 6;
long debounceDelay = 50;


int Saida1State1 = LOW;
int buttonState1;
int lastButtonState1 = LOW;
long lastDebounceTime1 = 0;

int Saida1State2 = LOW;
int buttonState2;
int lastButtonState2 = LOW;
long lastDebounceTime2 = 0;

int Saida1State3 = LOW;
int buttonState3;
int lastButtonState3 = LOW;
long lastDebounceTime3 = 0;

bool serialEv = true;
String msg;
boolean readingSOS = 0;
String readString = "";
bool ax = true;
//IRsend irsend;

void setup() {
  Serial.begin(9600);
  //Botões
  pinMode(buttonSaida1, INPUT);
  pinMode(buttonSaida2, INPUT);
  pinMode(buttonSaida3, INPUT);

  //Saídas Rele
  for (int i = 0; i < TAM_SAIDAS; i++) {
    pinMode(SAIDAS[i], OUTPUT);
  }

  //Inicializa Desligado
  for (int i = 0; i < TAM_SAIDAS; i++) {
    digitalWrite(SAIDAS[i], LOW);
  }

  //CONTROL RS485
  pinMode(CONTROL, OUTPUT);
  digitalWrite(CONTROL, LOW);
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
          if (aux == "TV") {
            aux = getValue(readString, '|', 3);
            if (aux == "RAW") {
              int raw_tam = getValue(readString, '|', 4).toInt();
              int khz = getValue(readString, '|', 5).toInt();
              //sendIr(readString, 6, '|', raw_tam, khz);
            }
            else {
              //unsigned int irSignal[] = {
              //  2650, 850, 500, 400, 450, 450, 450, 850, 450, 850, 1350, 850, 450, 450, 400, 450, 450, 450, 400, 450, 450, 450, 400, 450, 450, 450, 850, 450, 450, 400, 450, 850, 450, 450, 900, 850, 900, 850, 900, 850, 450, 450, 850, 850, 450, 450, 450, 450, 400, 450, 450, 450, 850, 850, 450, 450, 450, 450, 400, 450, 450
              //};
              //irsend.sendRaw(irSignal, 62, 32);
            }
          }
          else {
            if (aux == "C") {
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
      }
      readString = "";
      ax = true;
      stringComplete = false;
    }
  }

  //DEBOUNCE Saida1
  int reading1 = digitalRead(buttonSaida1);
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == HIGH) {
        //Saida 3 Plafon2
        //Saida 1 Pendentes 0
        //Saida 2 Spots 1
        if (!digitalRead(SAIDAS[0]) && !digitalRead(SAIDAS[1])) {
          digitalWrite(SAIDAS[0], HIGH);
        }
        else {
          if (digitalRead(SAIDAS[0]) && !digitalRead(SAIDAS[1])) {
            digitalWrite(SAIDAS[0], LOW);
            digitalWrite(SAIDAS[1], HIGH);
          }
          else {
            if (!digitalRead(SAIDAS[0]) && digitalRead(SAIDAS[1])) {
              digitalWrite(SAIDAS[0], HIGH);
              digitalWrite(SAIDAS[1], HIGH);
            }
            else {
              if (digitalRead(SAIDAS[0]) && digitalRead(SAIDAS[1])) {
                digitalWrite(SAIDAS[0], LOW);
                digitalWrite(SAIDAS[1], LOW);
              }
            }
          }
        }
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
      if (buttonState2 == HIGH) {
        digitalWrite(SAIDAS[2], !digitalRead(SAIDAS[2]));
      }
    }
  }

  //DEBOUNCE Saida3
  int reading3 = digitalRead(buttonSaida3);
  if (reading3 != lastButtonState3) {
    lastDebounceTime3 = millis();
  }

  if ((millis() - lastDebounceTime3) > debounceDelay) {
    if (reading3 != buttonState3) {
      buttonState3 = reading3;
      if (buttonState3 == HIGH) {
        if (!digitalRead(SAIDAS[3]) && !digitalRead(SAIDAS[5])) {
          digitalWrite(SAIDAS[3], HIGH);
        }
        else {
          if (digitalRead(SAIDAS[3]) && !digitalRead(SAIDAS[5])) {
            digitalWrite(SAIDAS[3], LOW);
            digitalWrite(SAIDAS[5], HIGH);
            enviaMsg("@1|4|S11|W|1#");
          }
          else {
            if (!digitalRead(SAIDAS[3]) && digitalRead(SAIDAS[5])) {
              digitalWrite(SAIDAS[3], HIGH);
              digitalWrite(SAIDAS[5], HIGH);
              enviaMsg("@1|4|S11|W|1#");
            }
            else {
              if (digitalRead(SAIDAS[3]) && digitalRead(SAIDAS[5])) {
                digitalWrite(SAIDAS[3], LOW);
                digitalWrite(SAIDAS[5], LOW);
                enviaMsg("@1|4|S11|W|0#");
              }
            }
          }
        }

      }
    }
  }

  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
  lastButtonState3 = reading3;
}




int getDispositivo(String dispositivo) {

  if (dispositivo == "S1") {
    return 0;
  }
  if (dispositivo == "S2") {
    return 1;
  }
  if (dispositivo == "S3") {
    return 2;
  }
  if (dispositivo == "S4") {
    return 3;
  }
  if (dispositivo == "S5") {
    return 4;
  }
  if (dispositivo == "S6") {
    return 5;
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


/***
   Quebra a String a partir da posição do caractere separador desejado(pos)
   Separa o protocolo AUTOHOME dos RAWs
   obs: Utilizado em conjunto com a biblioteca IRremote
   raw_tam = Tamanho da quantidade de bits(IRREMOTE)
   khz= Frequencia(IRREMOTE)
   Exemplo: data="1|3|TV|RAW|62|36|2700|850|450"
   Com pos = 6, irá processar a partir do 2700
*/
void sendIr(String data, int pos, char separator, int raw_tam, int khz)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  int index_array = 0;
  unsigned int irSignal[raw_tam];
  for (int i = 0; i <= maxIndex; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;

      if (found > pos) {
        String a = data.substring(strIndex[0], strIndex[1]);
        irSignal[index_array] = a.toInt();
        index_array++;
      }

    }
  }
  //irsend.sendRaw(irSignal, raw_tam, khz);
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
        if (digitalRead(SAIDAS[getDispositivo(dispositivo)]) != HIGH) {
          digitalWrite(SAIDAS[getDispositivo(dispositivo)], HIGH);
        }
      }
      else {
        if (cmd == "0") {
          if (digitalRead(SAIDAS[getDispositivo(dispositivo)]) != LOW) {
            digitalWrite(SAIDAS[getDispositivo(dispositivo)], LOW);
          }
        } else {
          if (cmd == "IS") {
            digitalWrite(SAIDAS[getDispositivo(dispositivo)], !digitalRead(SAIDAS[getDispositivo(dispositivo)]));
          }
        }
      }
      int r = digitalRead(SAIDAS[getDispositivo(dispositivo)]);
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
      int r = digitalRead(SAIDAS[getDispositivo(dispositivo)]);
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
