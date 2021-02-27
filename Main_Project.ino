#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "Yash Moto"
#define WIFI_PASS "onepiece2luffy"

//#define WIFI_SSID "Akshit"
//#define WIFI_PASS "akshit2001"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "akagarwa1"
#define MQTT_PASS "aio_UjRk3692OhO5Y80U0r7KNuShfomz"

int R = D8;
int G = D7;
int B = D6;

//-------------Class for defining colors --------
struct color
{
    int redValue;
    int greenValue;
    int blueValue;
  
    color(int x, int y, int z)
    {
      redValue   = x;
      greenValue = y;
      blueValue  = z;
    }
  
    void lightItUp()
    {
        digitalWrite(R,redValue);
        digitalWrite(G,greenValue);
        digitalWrite(B,blueValue);
    }
};
//-----------------------------------------------


//----------------Define Color Profiles----------
color Red(255,0,0);
color Blue(0,0,255);
color Green(0,255,0);
color Yellow(255,200,0);
color Pink(255,128,255);
color Purple(85,0,128);
color OFF(0,0,0);
//-----------------------------------------------


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe happyLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/happyLight");
Adafruit_MQTT_Publish happyStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/happyStatus");
Adafruit_MQTT_Subscribe sadLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/sadLight");
Adafruit_MQTT_Publish sadStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/sadStatus");
Adafruit_MQTT_Subscribe tiredLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/tiredLight");
Adafruit_MQTT_Publish tiredStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/tiredStatus");
Adafruit_MQTT_Subscribe stressedLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/stressedLight");
Adafruit_MQTT_Publish stressedStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/stressedStatus");
Adafruit_MQTT_Subscribe TurnOFF = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/TurnOFF");
Adafruit_MQTT_Publish TurnOFFStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/TurnOFFStatus");

//-------------------MQTT connection-------
void MQTT_connect()
{

  //  // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    mqtt.disconnect();
    return;
  }

  int8_t ret;
  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}
//-------------------------------------------------------


void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

//-------Connect to WiFi-------------------------------
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  digitalWrite(LED_BUILTIN, LOW);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(500);
  }
//-----------------------------------------------------  

  Serial.println("OK!");

//-------Subscribe to all the feeds on io.adafruit-----
  mqtt.subscribe(&happyLight);
  mqtt.subscribe(&sadLight);
  mqtt.subscribe(&tiredLight);
  mqtt.subscribe(&stressedLight);
  mqtt.subscribe(&TurnOFF);
//-----------------------------------------------------

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);

  MQTT_connect();
}


void loop()
{
  Adafruit_MQTT_Subscribe * subscription;
  
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &happyLight)
    {
        Serial.print("happyLight: ");
        Serial.println((char*) happyLight.lastread);
        Red.lightItUp();
        happyStatus.publish("ON");
        sadStatus.publish("OFF");
        tiredStatus.publish("OFF");
        stressedStatus.publish("OFF");
        TurnOFFStatus.publish("NOT DONE");
    }
    
    else if(subscription == &sadLight)
    {
      Serial.print("sadLight: ");
      Serial.println((char*) sadLight.lastread);
      Yellow.lightItUp();
      happyStatus.publish("OFF");
      sadStatus.publish("ON");
      tiredStatus.publish("OFF");
      stressedStatus.publish("OFF");
      TurnOFFStatus.publish("NOT DONE");
    }
    
    else if (subscription == &tiredLight)
    {
      Serial.print("tiredLight: ");
      Serial.println((char*) tiredLight.lastread);
      Blue.lightItUp();
      happyStatus.publish("OFF");
      sadStatus.publish("OFF");
      tiredStatus.publish("ON");
      stressedStatus.publish("OFF");
      TurnOFFStatus.publish("NOT DONE");
    }

    else if (subscription == &stressedLight)
    {
      Serial.print("stressedLight: ");
      Serial.println((char*) stressedLight.lastread);
      Green.lightItUp();
      happyStatus.publish("OFF");
      sadStatus.publish("OFF");
      tiredStatus.publish("OFF");
      stressedStatus.publish("ON");
      TurnOFFStatus.publish("NOT DONE");
    }

    else if (subscription == &TurnOFF)
    {
        Serial.print("All Lights: ");
        Serial.println((char*) happyLight.lastread);
        OFF.lightItUp();
        happyStatus.publish("OFF");
        sadStatus.publish("OFF");
        tiredStatus.publish("OFF");
        stressedStatus.publish("OFF");
        TurnOFFStatus.publish("DONE");
    }
  }
}
