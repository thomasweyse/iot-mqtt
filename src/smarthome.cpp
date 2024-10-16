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
WiFiClientSecure espClient;

int progress = 0;
char getProgress(int progress);
void printTempLCD(int progress);
void printStateLCD(const String message);
void printFullStateLCD(String firstLine, String secondLine);
LiquidCrystal_I2C mylcd(0x27,16,2);

xht11 xht(17);
unsigned char dht[4] = {0, 0, 0, 0};


void setup() {
    // Initialize LCD
    mylcd.init();
    mylcd.backlight();

    // TODO think about certs / TKW / 18.10.2024
    // espClient.setCACert(ca_cert);

    // Establish connection to IoT Broker
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        printStateLCD("Init WiFi/MQTT");
    }
}

void loop()
{
    if (xht.receive(dht))
    {
        printTempLCD(progress);
        progress++;
        
        if(progress == 2)
        {
            progress = 0;
        }
    }
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