#include <cstdlib>
#include <string.h>
#include <time.h>
#include <WiFi.h>
#include "LiquidCrystal_I2C.h"
#include "xht11.h"
#include "PubSubClient.h"
#include "smarthome.h"
#include "mqttcert.h"
#include <WiFiClientSecure.h>

// init wifi secure client
WiFiClient espClient;
PubSubClient mqttClient(espClient);

int progress = 0;
char getProgress(int progress);
void printTempLCD(int progress);
void printStateLCD(const String message);
void printFullStateLCD(String firstLine, String secondLine);
LiquidCrystal_I2C mylcd(0x27,16,2);

xht11 xht(17);
unsigned char dht[4] = {0, 0, 0, 0};

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

char mqttDataStream[64];
void generateMQTTData(unsigned char dht[4]);

void setup() {
    Serial.begin(115200);

    // Initialize LCD
    mylcd.init();
    mylcd.backlight();

    // Establish connection to IoT Broker
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        printStateLCD("Init WiFi/MQTT");
    }
    printStateLCD("               ");

    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(callback);

    while (!mqttClient.connected()) 
    {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (mqttClient.connect(client_id.c_str(), MQTT_USER, MQTT_PWD)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(mqttClient.state());
            delay(2000);
        }
    }
    Serial.print("-> setup() completed!");
}

void loop()
{
    if (xht.receive(dht))
    {
        printTempLCD(progress);
        
        if(progress == 0)
        {
            //mqttClient.publish(MQTT_TOPIC, "Hallo erstmal!");
            generateMQTTData(dht);
            mqttClient.publish(MQTT_TOPIC, mqttDataStream);
            mqttClient.subscribe(MQTT_TOPIC);
        }

        progress++;

        if(progress == 2)
        {
            progress = 0;
        }
    }

    delay(3000);
}

char getProgress(int progress)
{
  char retVal = ' ';
  switch(progress)
  {
    case 0: retVal = '-';
            break;
    case 1: retVal = '|';
            break;
    default: retVal = '*';
            break;
  }
  return retVal;
}

void printTempLCD(int progress)
{
    mylcd.setCursor(0, 0);
    mylcd.print("T = ");
    mylcd.print(dht[2]);
    mylcd.setCursor(0, 1);
    mylcd.print("H = ");
    mylcd.print(dht[0]);
    mylcd.setCursor(15,1);
    mylcd.print(getProgress(progress));
}

void printFullStateLCD(String firstLine, String secondLine)
{
    mylcd.setCursor(0, 0);
    mylcd.print(firstLine);
    mylcd.setCursor(0, 1);
    mylcd.print(secondLine);
}

void printStateLCD(const String message){
    mylcd.setCursor(0, 0);
    mylcd.print("state:");
    mylcd.setCursor(0, 1);
    mylcd.print(message);
}

void clearStateLCD(){
    mylcd.clear();
}

void generateMQTTData(unsigned char dht[4]){
    int temp = dht[2];
    int humi = dht[0];
    snprintf( mqttDataStream, sizeof(mqttDataStream), "%d %d" , temp, humi);
}