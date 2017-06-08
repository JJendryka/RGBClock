#include <ArduinoJson.h>

#pragma once


#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

#include "RGBStrip.h"

int SECONDS[] = {100, 0, 0};
int TIMEELAPSED[] = {0, 100, 50};
int TIMEREMAINING[] = {50, 100, 0};
int PAUSEELAPSED[] = {0, 50, 100};
int PAUSEREMAINING[] = {50, 0, 100};
int MINUTES[] = {0, 100, 0};
int PAUSEMINUTES[] = {0, 0, 100};
int HOURS[] = {70, 70, 70};


  RGBStrip strip = RGBStrip();
  int periods[100];
  int countOfPeriods = 0;
  int actualTime = 0;
  long long previousMillis = 0;
  
void setup() {
  pinMode(0, INPUT_PULLUP);
  
  Serial.begin(115200);
  rainbow();
  WiFiMulti.addAP("PWR-WiFi", "");
}

void loop() {
  if(actualTime % 120 == 0 || digitalRead(0) == 0) {
    if((WiFiMulti.run() == WL_CONNECTED)) {
  
      HTTPClient http;
  
      Serial.print("[HTTP] begin...\n");
      http.begin("http://jendryka.tk/timetable.json"); //HTTP
  
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
  
      if(httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            StaticJsonBuffer<1000> jsonBuffer;
            char json[payload.length()+1];
            payload.toCharArray(json, payload.length()+1);
            JsonObject& root = jsonBuffer.parseObject(json);
            actualTime = root["time"];
            countOfPeriods = root["periods"];
            JsonArray& classes = root["classes"];
            for (int i=0; i<countOfPeriods; i++){
              int classes0 = classes[i*2];
              int classes1 = classes[i*2+1];
              periods[i] = (classes0 * 60) + classes1;
            }
        }
      }
      else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
  
      http.end();
    }
  }
  while(true){
    if (millis() - previousMillis >= 1000) break;
  }
  previousMillis = millis();
  setTimeOnDisplay(actualTime++);
  delay(1000);
  for (int i=0; i<10; i++) {
    strip.upd();
  }
}

int getHours(int actualTime) {
  return (((actualTime - (actualTime % 3600)) / 3600)%24);
}

int getMinutes(int actualTime) {
  return (((actualTime - (actualTime % 60)) / 60)%60);
}

int getSeconds(int actualTime) {
  return (actualTime % 60);
}

bool isInRegion(int index, int startOfRegion, int endOfRegion) {
  if (startOfRegion < endOfRegion) {
    if (index >= startOfRegion && index < endOfRegion) return true;
  }
  else {
    if (index >= startOfRegion || index < endOfRegion) return true;
  }
  return false;
}

void setTimeOnDisplay(int actualTime)
{
  int found = -1;
  for(int i=0; i < countOfPeriods - 1; i++)
  {
    if ((actualTime - (actualTime % 60)) / 60 > periods[i] && (actualTime - (actualTime % 60)) / 60 < periods[i+1]) {
      found = i;
    }
  }
  if (found == -1) {
    for (int i=0; i<60; i++){
      if (i == getSeconds(actualTime)) strip.setSmoothPixel(i, SECONDS);
      else if (i == getMinutes(actualTime)) strip.setSmoothPixel(i, MINUTES);
      else if (i == getHours(actualTime)%12 * 5) strip.setSmoothPixel(i, HOURS);
      else strip.setSmoothPixel(i, 0, 0, 0);
    }
  }
  else {
    for (int i=0; i<60; i++){
      if (i == getSeconds(actualTime)) strip.setSmoothPixel(i, SECONDS);
      else if (i == getHours(actualTime)%12 * 5) strip.setSmoothPixel(i, HOURS);
      else if (i == getMinutes(actualTime)) {
        if (found%2) strip.setSmoothPixel(i, PAUSEMINUTES);
        else strip.setSmoothPixel(i, MINUTES);
      }
      else if (isInRegion(i, periods[found] % 60, getMinutes(actualTime))) {
        if (found%2) strip.setSmoothPixel(i, PAUSEELAPSED);
        else strip.setSmoothPixel(i, TIMEELAPSED);
      }
      else if (isInRegion(i, getMinutes(actualTime), periods[found+1] % 60)) {
        if (found%2) strip.setSmoothPixel(i, PAUSEREMAINING);
        else strip.setSmoothPixel(i, TIMEREMAINING);
      }
      else strip.setSmoothPixel(i, 0, 0, 0);
    }
  }
}

void rainbow()
{
 int strp [60][3];
 for (int test=0; test<2000; test++) {
  if(test%10==0)
  {
    //float data[3] = {(sin((float)3.14*2*(float)(test%256))+1)*100,0,0};
    
    for(int i = 59; i>=1;i--)
    {
      strp[i][0]=strp[i-1][0];
      strp[i][1]=strp[i-1][1];
      strp[i][2]=strp[i-1][2];
    }
    strp[0][0]=(sin((float)(test%60)*3.14*2/60)+1)*100;
    strp[0][1]=(sin((float)((test+85)%60)*3.14*2/60)+1)*100;;
    strp[0][2]=(sin((float)((test+171)%60)*3.14*2/60)+1)*100;
    for(int i = 59; i>=0;i--)
    {
      float data[3] = {strp[i][0], strp[i][1], strp[i][2]};
      strip.setPixel(i, data);
    }  
  }
  delay(1);
  strip.upd(); 
 }
}
