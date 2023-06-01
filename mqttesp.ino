#include <ESP8266WiFi.h>
#include <MQTT.h>

WiFiClient net;
MQTTClient cliente;
const int GPIO_0 = 0;

void conectar(); // Predefinição de conectar() que é usado em setup()
void setup() {
  pinMode(GPIO_0, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(GPIO_0, HIGH); //RELÉ INICIA DESLIGADO
  
  Serial.begin(9600);
  WiFi.begin("Nino", "25108500");
  cliente.begin("autohome.cloud.shiftr.io", net);
  conectar();
}
void conectar() {
  //Serial.print("Verificando wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(1000);
  }
  //Serial.print("\nConectando...");
  while (!cliente.connect("esp8266", "autohome", "jsf94glWDwKLxumA")) {
    //Serial.print(".");
    delay(1000);
  }
  //Serial.println("\Conectado!");
  //cliente.subscribe("/UmidadeMaxima");
  //cliente.subscribe("/rele");
  cliente.onMessage(messageReceived);
  //cliente.subscribe("/UmidadeMinima");
  // cliente.subscribe("/NivelAgua");
}
void loop() {
  //Serial.println("\lendo!");
  cliente.loop();
  delay(10); // Evita instabilidade do WiFi
  if (!cliente.connected()) {
    conectar();
  }

  int temp = random(18, 30);
  cliente.publish("rele", String("rele"));
  delay(5000);
}


void messageReceived(String &topic, String &payload) {
  //Serial.print("Mensagem recebida: ");
  //Serial.print(topic);
  //Serial.print(" - ");
  //Serial.print(payload);
  //Serial.println();
  if (topic == "/rele") {
    if (payload == "liga"){
       Serial.println("@1|2#");
        Serial.flush();
    }else if (payload == "desliga"){
       Serial.println("@2|2#");
        Serial.flush();
    }
  }

}
