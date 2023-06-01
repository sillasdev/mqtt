#include <ESP8266WiFi.h>
#include <ESP8266LLMNR.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
 
const char* ssid = "Nino"; //VARIÁVEL QUE ARMAZENA O NOME DA REDE SEM FIO EM QUE VAI CONECTAR
const char* password = "25108500"; //VARIÁVEL QUE ARMAZENA A SENHA DA REDE SEM FIO EM QUE VAI CONECTAR

ESP8266WebServer server(80); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.105:8082)

void setup() {
 Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); //DEFINE QUE O PINO É UMA SAÍDA

 WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE SEM FIO

  while (WiFi.status() != WL_CONNECTED) { //ENQUANTO STATUS FOR DIFERENTE DE CONECTADO, FAZ
    delay(500); //INTERVALO DE 500 MILISEGUNDOS
  }
  
  LLMNR.begin("my");
  
  server.on("/",[](){server.send(200,"text/plain","Hello Sillas!");});
  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"
  //MDNS.addService("http","tcp",8082);
   
}
 
void loop() {
 server.handleClient();
}
