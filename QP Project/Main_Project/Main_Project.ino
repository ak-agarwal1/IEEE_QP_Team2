#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "Yash Moto"
#define WIFI_PASS "onepiece2luffy"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "akagarwa1"
#define MQTT_PASS "aio_UjRk3692OhO5Y80U0r7KNuShfomz"

int R = D8;
int B = D7;
int G = D6;

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
color Yellow(255,255,0);
color Pink(255,128,255);
color Purple(85,0,128);
color OFF(0,0,0);
//-----------------------------------------------


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe button = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/happyLight");
Adafruit_MQTT_Publish status1 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/status1");


//-------------------MQTT connection-------
void MQTT_connect()
{

  //  // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    //    mqtt.disconnect();
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
    delay(5000);  // wait 5 seconds
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


  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  digitalWrite(LED_BUILTIN, LOW);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(50);
  }

  Serial.println("OK!");

  mqtt.subscribe(&button);

  pinMode(R, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(R, LOW);

}


void loop()
{
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &button)
    {
      //Print the new value to the serial monitor
      Serial.print("button: ");
      Serial.println((char*) button.lastread);

      if (!strcmp((char*) button.lastread, "ON"))
      {
        digitalWrite(R, HIGH);
        status1.publish("ON");
      }
      else if (!strcmp((char*) button.lastread, "OFF"))
      {
        digitalWrite(R, LOW);
        status1.publish("OFF");
      }
      else
      {
        status1.publish("ERROR");
      }
    }
    else
    {
      //status1.publish("ERROR");
    }
  }
}
