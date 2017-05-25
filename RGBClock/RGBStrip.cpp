#include "RGBStrip.h"


RGBStrip::RGBStrip(int pin) : pixels(LENGTH, 14, NEO_GRB + NEO_KHZ800){
    this->pin = pin;
    pixels.begin();
    smooth_speed=10;//desired brightness will be reached after number of refreshments specified here;
    for(int h=0;h<LENGTH;h++)
    {
      smooth_progress[h][0]=smooth_speed;
      smooth_progress[h][1]=smooth_speed;
      smooth_progress[h][2]=smooth_speed;
    }
    maximum = 1;
}
void RGBStrip::setPixel(int index, float rgb[]) {
    strip[index][0] = rgb[0];
    strip[index][1] = rgb[1];
    strip[index][2] = rgb[2];
}
void RGBStrip::setSmoothPixel(int index, float rgb[]){
    difference[index][0] = rgb[0] - strip[index][0];
    difference[index][1] = rgb[1] - strip[index][1];
    difference[index][2] = rgb[2] - strip[index][2];
    target[index][0]=rgb[0];
    target[index][1]=rgb[1];
    target[index][2]=rgb[2];
    smooth_progress[index][0]=0;
    smooth_progress[index][1]=0;
    smooth_progress[index][2]=0;
}

void RGBStrip::setSmoothPixel(int index, int r, int g, int b){
    difference[index][0] = r - strip[index][0];
    difference[index][1] = g - strip[index][1];
    difference[index][2] = b - strip[index][2];
    target[index][0]=r;
    target[index][1]=g;
    target[index][2]=b;
    smooth_progress[index][0]=0;
    smooth_progress[index][1]=0;
    smooth_progress[index][2]=0;
}

void RGBStrip::setStrip(float data[][3]){
    for (int i=0; i < LENGTH; i++){
        setPixel(i, data[i]);
    }
}
void RGBStrip::setSmoothStrip(float data[][3]){
    for (int i=0; i < LENGTH; i++){
        setSmoothPixel(i, data[i]);
    }
}
void RGBStrip::upd(){
    for (int i=0; i<30; i++){
        maximum = 1;
        maximum=max(max(1,difference[i][0]),max(difference[i][1],difference[i][2]));
        if(smooth_progress[i][0]<smooth_speed)
        {
          smooth_progress[i][0]++;
          strip[i][0] = (target[i][0]-difference[i][0])+smooth_progress[i][0]*difference[i][0]/smooth_speed;
        }
        
        if(smooth_progress[i][1]<smooth_speed)
        {
          smooth_progress[i][1]++;
          strip[i][1] = (target[i][1]-difference[i][1])+smooth_progress[i][1]*difference[i][1]/smooth_speed;
        }
        
        if(smooth_progress[i][2]<smooth_speed)
        {
          smooth_progress[i][2]++;
          strip[i][2] = (target[i][2]-difference[i][2])+smooth_progress[i][2]*difference[i][2]/smooth_speed;
        }
        
        
        
        
        
        pixels.setPixelColor(i, pixels.Color((int)strip[i][0], (int)strip[i][1], (int)strip[i][2]));
    }
    Serial.println(strip[2][2]);
    pixels.show();
}
