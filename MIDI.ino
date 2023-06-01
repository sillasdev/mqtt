#include <MIDI.h>
#include "InputDebounce.h"
// Simple tutorial on how to receive and send MIDI messages.
// Here, when receiving any message on channel 4, the Arduino
// will blink a led and play back a note for 1 second.
#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

static const int pin1 = 8;
static const int pin2 = 9;
static const int pin3 = 10;

static InputDebounce button1; // not enabled yet, setup has to be called first, see setup() below
static InputDebounce button2;
static InputDebounce button3;

MIDI_CREATE_DEFAULT_INSTANCE();

void button1_pressedCallback(uint8_t pinIn, unsigned long duration)
{
  // handle released state


}

void button2_pressedCallback(uint8_t pinIn, unsigned long duration)
{

}

void button3_pressedCallback(uint8_t pinIn, unsigned long duration)
{
   
     MIDI.sendNoteOn(42, 127, 1);
}
const int pinoPOT = A0;
float valuestored = 0;
//Global Variables
    int sensorPin = 0;      //pin number to use the ADC
    float sensorValue = 0;    //initialization of sensor variable, equivalent to EMA Y
    float EMA_a = 0.6;      //initialization of EMA alpha
    int EMA_S = 0; 
    
void setup()
{
  MIDI.begin(MIDI_CHANNEL_OMNI);
  button1.registerCallbacks(button1_pressedCallback,NULL,NULL,NULL);
  button2.registerCallbacks(button2_pressedCallback,NULL,NULL,NULL);
  button3.registerCallbacks(button3_pressedCallback,NULL,NULL,NULL);
  button1.setup(pin1, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES,300);
  button2.setup(pin2, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES, 300); // single-shot pressed-on time duration callback
  button3.setup(pin3, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES, 300); 
   // MIDI.begin(4);                      // Launch MIDI and listen to channel 4
pinMode(pinoPOT, INPUT); //DEFINE O PINO COMO SAÍDA
 //EMA_S = analogRead(pinoPOT); 
 //Serial.begin(9600);
}

    
void loop()
{
    unsigned long now = millis();
   sensorValue = analogRead(pinoPOT);                //read the sensor value using ADC

  if(sensorValue != valuestored){
    float res = sensorValue - valuestored;
    res=abs(res);
    if(res>5){
       float luminosidadeLED = map(sensorValue, 0, 1023, 0, 127);
       MIDI.sendControlChange(43, luminosidadeLED, 1);
       valuestored=sensorValue;
    }
  }
  
  
  // poll button state
  button1.process(now); // callbacks called in context of this function
  button2.process(now);
  button3.process(now);
  delay(1); // [ms]

 
}
