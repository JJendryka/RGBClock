#include "RGBStrip.h"

int test;

  RGBStrip strip = RGBStrip(14);
  long unsigned prev = 0;
  
void setup() {
  Serial.begin(115200);
  Serial.println("dada");
  float data[3] = {100, 255, 170};
  strip.setSmoothPixel(2, data);
  strip.setSmoothPixel(3, 100,150,0);
  strip.setSmoothPixel(1, 200,75,150);
}

void loop() {
  test++;
  if(test%10==0)
  {
    strip.setSmoothPixel(random(59), random(255), random(255), random(255));
  }
  delay(20);
  strip.upd();
  Serial.println("a");
}
