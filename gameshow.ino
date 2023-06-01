int botao1 = 10;
int botao2 = 12;


int led1 = 7;
int led2 = 6;

bool estadoLed = 0;
 
void setup()
{
  pinMode(botao1, INPUT_PULLUP); // define o pino do botao como entrada "INPUT"
  pinMode(botao2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, HIGH); 
  digitalWrite(led2, HIGH); 
}
 
void loop()
{
  if(!digitalRead(botao1) && digitalRead(botao2)) // Se o botão for pressionado
  {
 
    digitalWrite(led1, LOW); 
    delay(5000);
    digitalWrite(led1, HIGH); 
  }   

    if(!digitalRead(botao2) && digitalRead(botao1)) // Se o botão for pressionado
  {
 
    digitalWrite(led2, LOW); 
    delay(5000);
    digitalWrite(led2, HIGH); 
  }   

}
