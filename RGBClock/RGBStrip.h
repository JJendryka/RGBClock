#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define LENGTH 60

class RGBStrip {
private:
    int strip[LENGTH][3];
    int difference[LENGTH][3];
    int target[LENGTH][3];
    int smooth_progress[LENGTH][3];
    int smooth_speed;
    int pin;
    Adafruit_NeoPixel pixels;
    int maximum;
public:
    RGBStrip();
    void setPixel(int index, float rgb[3]);
    void setSmoothPixel(int index, float rgb[3]);
    void setSmoothPixel(int index, int r, int g, int b);
    void setStrip(float data[LENGTH][3]);
    void setSmoothStrip(float data[LENGTH][3]);
    void upd();
};
