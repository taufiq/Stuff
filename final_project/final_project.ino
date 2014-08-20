#include <SPI.h>
#include <Andee.h>
#include "Wire.h"

#define DS1307_ADDRESS 0x68

const int lightSensor = 0;
const int tempSensor = 1;

int counter = 1;


const int musicPin1 = 10;
const int fanPin1 = 9;
const int fanPin2 = 8;
const int lightPin1 = 7;
const int lightPin2 = 6;
const int lightPin3 = 5;
bool once = true;
bool once_for_off = false;

int lightReading;
int tempReading;
int timeReading[3];

//Welcoming
AndeeHelper welcomeButton;

//Toggle
AndeeHelper togglebutton;
int state = 0;

//Manual(PUT HERE)
AndeeHelper light;

//Automatic(Here :P)
AndeeHelper temperature;
AndeeHelper lighting;
AndeeHelper music;

void setup() {
  Serial.begin(9600);
  Andee.begin();
  Andee.clear();
  Wire.begin();
  state = 0;
  pinMode(lightSensor, INPUT);
  pinMode(tempSensor, INPUT);
  
  set();
}

void loop() {
  welcome();
  setDate();
  Serial.print(timeReading[0]);
  Serial.print(" : ");
  Serial.print(timeReading[1]);
  Serial.print(" : ");
  Serial.println(timeReading[2]);

  
  if(togglebutton.isPressed()) {
    togglebutton.ack(); 
    if(state == 0) {
      togglebutton.setTitle("Manual");
      state = 1;
      togglebutton.setColor(RED);
      light.update();
      temperature.remove();
      lighting.remove();
      music.remove();
    } else {
      togglebutton.setTitle("Automation");
      state = 0;
      togglebutton.setColor(GREEN);
      light.remove();
      temperature.update();
      lighting.update();
      music.update();
    }  
  }
  togglebutton.update();
  

}

byte bcdToDec(byte val) {
  return ((val/16*10) + (val%16));
}

void setDate() {
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111);
  int weekDay = bcdToDec(Wire.read());
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());
   
  timeReading[0] = second;
  timeReading[1] = minute;
  timeReading[2] = hour; 
}
void set(){
  togglebutton.setId(0);
  togglebutton.setType(BUTTON_IN);
  togglebutton.setLocation(0,0,FULL);
  togglebutton.setTitle("Neutral, tap for Manual");
  togglebutton.setColor(BLUE);
  
  light.setId(1);
  light.setType(SLIDER_IN);
  light.setLocation(1,0,FULL);
  light.setTitle("Light Brightness");
  light.setSliderReportMode(ON_VALUE_CHANGE);
  light.setSliderNumIntervals(100);
  light.setColor("4Dda6e17");
  
  welcomeButton.setId(2);
  welcomeButton.setType(DATA_OUT);
  welcomeButton.setLocation(0,0,FULL);
  welcomeButton.setData("Welcome to Smart Living Bedroom, Made by Group KYNRT");
  welcomeButton.setColor("FF357385");
  
  temperature.setId(3);
  temperature.setType(DATA_OUT);
  temperature.setLocation(1,0,FULL);
  temperature.setTitle("Current Temperature");
  
  lighting.setId(4);
  lighting.setType(DATA_OUT);
  lighting.setLocation(2,0,FULL);
  lighting.setTitle("Current Lighting");  
  
  music.setId(5);
  music.setType(DATA_OUT);
  music.setLocation(3,0,FULL);
  music.setTitle("Music State");
}
void welcome(){
  welcomeButton.update();
  delay(2000);
  welcomeButton.remove();
}


