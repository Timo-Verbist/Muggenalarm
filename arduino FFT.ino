//https://clydelettsome.com/blog/2019/12/18/my-weekend-project-audio-frequency-detector-using-an-arduino/
#include "arduinoFFT.h"
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "wifi_credentials.h"

//Parameters
const int micPin  = A0;

#define SAMPLES 128   //SAMPLES-pt FFT. Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 2048 //TS = Based on Nyquist, must be 2 times the highest expected frequency.

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "10.6.120.6";
//const char broker[] = "141.134.70.180";
//const char broker[] = "192.168.0.205";
int        port     = 1885;
const char topic[]  = "test";

const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void setup() 
{
    Serial.begin(115200); //Baud rate for the Serial Monitor
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}
    
void loop()
{
    /*Sample SAMPLES times*/
    for(int i=0; i<SAMPLES; i++) 
    {
        microSeconds = micros(); //Returns the number of microseconds since the Arduino board began running the current script.
        
        vReal[i] = analogRead(0); //Reads the value from analog pin 0 (A0), quantize it and save it as a real term.
        vImag[i]= 0; //Makes imaginary term 0 always

        /*remaining wait time between samples if necessary*/
        while (micros() < (microSeconds + samplingPeriod))
        {
            //do nothing
        }
    }
    
    /* Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD) ;
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES) ;
    
    /*Find peak frequency and print peak*/
    double peak = FFT.MajorPeak (vReal, SAMPLES, SAMPLING_FREQUENCY);
    Serial.println(peak); //out the most dominant frequency.

    if (peak<500) {
      if (peak > 150) {
        Serial.println("MUG!");
        mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    //save the last time a message was sent
    previousMillis = currentMillis;

    //record random value from A0, A1 and A2
//    char Rvalue = "MUG!";

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println("MUG!");


    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print("MUG!");
    mqttClient.endMessage();


    Serial.println();
  }
      }
    }

//    for(int i=0; i<SAMPLES; i++) {
//      vReal[i] = 0;
//    }
//    memset(vReal, 0, sizeof(vReal));
    peak = 0;
    
    /*Script stops here. Hardware reset required. */
//    while (1); //do one time
    delay(2000);

    
}
