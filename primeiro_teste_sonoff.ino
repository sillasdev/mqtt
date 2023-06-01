#include <ESP8266WiFi.h> //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO
 
const char* ssid = "Nino"; //VARIÁVEL QUE ARMAZENA O NOME DA REDE SEM FIO EM QUE VAI CONECTAR
const char* password = "25108500"; //VARIÁVEL QUE ARMAZENA A SENHA DA REDE SEM FIO EM QUE VAI CONECTAR

//DEFINIÇÃO DE IP FIXO PARA O ESP8266
IPAddress ip(192,168,1,99); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110 **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
IPAddress gateway(192,168,1,1); //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
IPAddress subnet(255,255,255,0); //MASCARA DE REDE (ALTERE PARA A SUA MÁSCARA DE REDE)

int relePin = 12; //DECLARAÇÃO DE VARIÁVEL DO TIPO INTEIRA(VARIÁVEL CORRESPONDENTE AO GPIO 12 DO ESP8266)

WiFiServer server(8082); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.105:8082)

int status = 0; //DECLARAÇÃO DE VARIÁVEL DO TIPO INTEIRA(SERÁ RESPONSÁVEL POR VERIFICAR O STATUS ATUAL DA LÂMPADA)
 
void setup() {
 
  pinMode(LED_BUILTIN, OUTPUT); //DEFINE QUE O PINO É UMA SAÍDA
 
   
  WiFi.begin(ssid, password); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE SEM FIO
  WiFi.config(ip, gateway, subnet); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI SETAR O IP FIXO NO ESP8266
 
  while (WiFi.status() != WL_CONNECTED) { //ENQUANTO STATUS FOR DIFERENTE DE CONECTADO, FAZ
    delay(500); //INTERVALO DE 500 MILISEGUNDOS
  }
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"

   digitalWrite(LED_BUILTIN, LOW); 
}
 
void loop() {
  // Check if a client has connected
   if (WiFi.status() != WL_CONNECTED) {
       digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
       delay(1000); 
    
   }
  WiFiClient client = server.available(); //VERIFICA SE ALGUM CLIENTE ESTÁ CONECTADO NO SERVIDOR
  if (!client) { //SE NÃO EXISTIR CLIENTE CONECTADO, FAZ
    return; //REEXECUTA O PROCESSO ATÉ QUE ALGUM CLIENTE SE CONECTE AO SERVIDOR
  }
 
  while(!client.available()){ //ENQUANTO CLIENTE ESTIVER CONECTADO
    delay(1); //INTERVALO DE 1 MILISEGUNDO
  }
 
  String request = client.readStringUntil('\r'); //FAZ A LEITURA DA PRIMEIRA LINHA DA REQUISIÇÃO
  client.flush(); //AGUARDA ATÉ QUE TODOS OS DADOS DE SAÍDA SEJAM ENVIADOS AO CLIENTE
 
  if (request.indexOf("/?releParamOn") != -1)  { //SE ENCONTRAR O PARÂMETRO "/?releParamOn" NA URL, FAZ
    digitalWrite(LED_BUILTIN, HIGH); //LIGA O RELÉ
    status = 1; //VARIÁVEL RECEBE VALOR 1(SIGNIFICA QUE A LÂMPADA ESTÁ ACESA)
  }
  if (request.indexOf("/?releParamOff") != -1)  { //SE ENCONTRAR O PARÂMETRO "/?releParamOff" NA URL, FAZ
    digitalWrite(LED_BUILTIN, LOW); //DESLIGA O RELÉ
    status = 0; //VARIÁVEL RECEBE VALOR 0(SIGNIFICA QUE A LÂMPADA ESTÁ APAGADA)
  }

  
  client.flush();
  
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nLED is now ";
  s += (status)?"ON":"OFF";
  s += "</html>\n";

  
  client.print(s);
  delay(1);
}
