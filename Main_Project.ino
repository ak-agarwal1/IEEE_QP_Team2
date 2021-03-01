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
#define MQTT_PASS ""

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
        analogWrite(R,redValue);
        analogWrite(G,greenValue);
        analogWrite(B,blueValue);
    }
};
//-----------------------------------------------


//----------------Define Color Profiles----------
color Yellow(255,255,0);          
color LightRed(255,10,10);        
color Lavender(181,66,246);       
color LightGreen(103,223,147);
color Coral(249,119,72);    
color Turquoise(72,209,204);           
color OFF(0,0,0);                 //Turn of the lights by changing RGB to  0 0 0
//-----------------------------------------------


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe sadLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/sadLight");
Adafruit_MQTT_Subscribe tiredLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/tiredLight");
Adafruit_MQTT_Subscribe stressedLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/stressedLight");
Adafruit_MQTT_Subscribe headacheLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/headacheLight");
Adafruit_MQTT_Subscribe worriedLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/worriedLight");
Adafruit_MQTT_Subscribe boredLight = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/boredLight");
Adafruit_MQTT_Subscribe TurnOFF = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/TurnOFF");

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
  mqtt.subscribe(&sadLight);
  mqtt.subscribe(&tiredLight);
  mqtt.subscribe(&stressedLight);
  mqtt.subscribe(&headacheLight);
  mqtt.subscribe(&worriedLight);
  mqtt.subscribe(&boredLight);
  mqtt.subscribe(&TurnOFF);
//-----------------------------------------------------

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  MQTT_connect();
  OFF.lightItUp();
}

void loop()
{
  Adafruit_MQTT_Subscribe * subscription;
  
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if(subscription == &sadLight)
    {
      Serial.print("sadLight: ");
      Serial.println((char*) sadLight.lastread);
      Yellow.lightItUp();
    }
    
    else if (subscription == &tiredLight)
    {
      Serial.print("tiredLight: ");
      Serial.println((char*) tiredLight.lastread);
      LightRed.lightItUp();
    }

    else if (subscription == &stressedLight)
    {
      Serial.print("stressedLight: ");
      Serial.println((char*) stressedLight.lastread);
      Lavender.lightItUp();
    }

    else if(subscription == &headacheLight)
    {
      Serial.print("headacheLight: ");
      Serial.println((char*) headacheLight.lastread);
      LightGreen.lightItUp();
    }

    else if(subscription == &worriedLight)
    {
      Serial.print("worriedLight: ");
      Serial.println((char*) worriedLight.lastread);
      Turquoise.lightItUp();
    }

    else if(subscription == &boredLight)
    {
      Serial.print("boredLight: ");
      Serial.println((char*) boredLight.lastread);
      Coral.lightItUp();
    }
    
    else if (subscription == &TurnOFF)
    {
        Serial.print("Turn off Lights: ");
        Serial.println((char*) TurnOFF.lastread);
        OFF.lightItUp();
    }
  }
}
