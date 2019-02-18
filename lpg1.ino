#include<Wire.h>
#include<Adafruit_SSD1306.h>
#include<Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D0, D7);  //(Rx,Tx)
#include "ThingSpeak.h"
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(-1);
#define lpg_sensor A0
#define buzzer D3


const char*  ssid= "";
const char* password="";
unsigned long myChannelNumber=  ;
const char* mywriteapkey="";
WiFiClient WiFiclient;

void setup() 
{  Serial.begin(115200);
  pinMode(lpg_sensor, INPUT);
  pinMode(D6,OUTPUT);
   mySerial.begin(9600); 
  pinMode(buzzer, OUTPUT);
  analogWrite(D3,0);
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();
    display.setTextSize(2);
  display.setTextColor(WHITE);
 
  display.println("LPG GAS DETECTOR");
  
   display.display();
  delay(2000);
 
   WiFi.begin(ssid, password);
   ThingSpeak.begin(WiFiclient);
   digitalWrite(D6,LOW);
}

void loop() 
{
  int val=analogRead(lpg_sensor);
  display.setCursor(0, 0);
  display.println("  LPG GAS  DETECTOR");
  ThingSpeak.writeFields(myChannelNumber,mywriteapkey);
   ThingSpeak.setField(1,val);
  
   Serial.println(val);

  if(val>60)
  {
    
    display.setCursor(0,32);
  display.println(" !!ALERT!!");
  
       display.display();
    
    analogWrite(buzzer, 255);
    display.display();

        mySerial.println("AT+CMGF=1"); 
    mySerial.println("AT+CMGS=\"+9999999999999\"\r");
    mySerial.println("HIGH LEAKAGE DETECTED!!!");
    Serial.print("message sent");
    mySerial.println((char)26);
    delay(1000);
  mySerial.println("ATD+919999999;");
  digitalWrite(D6,HIGH);
  } 
  else if(val>=50)
  {
    //display.clearDisplay();
    display.setCursor(0,32);
    display.display();
    display.print(" LPG GAS  LEAKAGE...");
  
    analogWrite(buzzer,150);
    display.display();
    
    mySerial.println("AT+CMGF=1"); 
    mySerial.println("AT+CMGS=\"+919999999999\"\r");
    mySerial.println("LPG GAS LEAKAGE DETECTED!!!");
    mySerial.println((char)26);
    Serial.print("message sent");
    digitalWrite(D6,HIGH);
  }
  
  else 
  {
    //display.clearDisplay();
    display.setCursor(0,32);
    digitalWrite(buzzer, LOW);
    display.println("NO LPG GAS DETECTED ");
   display.display();
   digitalWrite(D6,LOW);
    
  }
  display.clearDisplay();
}
