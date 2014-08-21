#include <SPI.h>
#include <Andee.h>
#include "Wire.h"

#define DS1307_ADDRESS 0x68

const int lightHelperSensor = 0;
const int tempSensor = 1;

int counter = 1;
int sa = 0;
const int musicPin1 = 10;
const int fanPin1 = 13;
const int fanPin2 = 8;
int fanToggle_State = 0;
int musicToggle_State = 0;

int lightHelperReading;
int tempReading;
int timeReading[3];

//Welcoming
AndeeHelper welcomeHelper;
bool welcomed = true;

//Toggle
AndeeHelper manualHelper;
AndeeHelper autoHelper;
AndeeHelper fanToggleHelper;
AndeeHelper musicToggleHelper;

//Manual(PUT HERE)
AndeeHelper lightHelper;

//Automatic(Here :P)
AndeeHelper temperatureHelper;
AndeeHelper musicHelper;

void setup() {
  Serial.begin(9600);
  Andee.begin();
  Andee.clear();
  Wire.begin();
  pinMode(fanPin1, OUTPUT);
  pinMode(lightHelperSensor, INPUT);
  pinMode(tempSensor, INPUT);
  initialize();
}

void loop() {
  welcome();
  setDate();
  checkToggleButton();
  checkFan_MusicButton();
  Serial.println(sa);
//  Serial.print(timeReading[0]);
//  Serial.print(" : ");
//  Serial.print(timeReading[1]);
//  Serial.print(" : ");
//  Serial.println(timeReading[2]);
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

void initialize() {
//  lightHelper.setId(1);
//  lightHelper.setType(SLIDER_IN);
//  lightHelper.setLocation(1,0,FULL);
//  lightHelper.setTitle("Light Brightness");
//  lightHelper.setSliderReportMode(ON_VALUE_CHANGE);
//  lightHelper.setSliderNumIntervals(100);
//  lightHelper.setColor("4Dda6e17");
  
  welcomeHelper.setId(2);
  welcomeHelper.setType(DATA_OUT);
  welcomeHelper.setLocation(0,0,FULL);
  welcomeHelper.setData("Welcome to Smart Living Bedroom, Made by Group KYNRT");
  welcomeHelper.setColor("FF357385");
  
  temperatureHelper.setId(3);
  temperatureHelper.setType(DATA_OUT);
  temperatureHelper.setLocation(1,0,FULL);
  temperatureHelper.setTitle("Current Temperature");
  
  musicHelper.setId(5);
  musicHelper.setType(DATA_OUT);
  musicHelper.setLocation(2,0,FULL);
  musicHelper.setTitle("Music State");
  
  manualHelper.setId(6);
  manualHelper.setType(BUTTON_IN);
  manualHelper.setLocation(0,0,HALF);
  manualHelper.setTitle("Manual");
 
  autoHelper.setId(7);
  autoHelper.setType(BUTTON_IN);
  autoHelper.setLocation(0,0,HALF);
  autoHelper.setTitle("Automatic"); 
  
  fanToggleHelper.setId(8);
  fanToggleHelper.setType(BUTTON_IN);
  fanToggleHelper.setLocation(1,0,FULL);
  fanToggleHelper.setTitle("Fan Off");
  fanToggleHelper.setColor(RED);
  
  musicToggleHelper.setId(9);
  musicToggleHelper.setType(BUTTON_IN);
  musicToggleHelper.setLocation(2,0,FULL);
  musicToggleHelper.setTitle("Music Off");
  musicToggleHelper.setColor(RED);
}

void welcome() {
  if (welcomed) {
    welcomeHelper.update();
    delay(2000);
    welcomeHelper.remove();
    setManual();
    welcomed = false;
  }
}

void setManual() {
   manualHelper.setColor(RED);
   autoHelper.setColor(BLUE);
   manualHelper.update();
   autoHelper.update();
   fanToggleHelper.update();
   musicToggleHelper.update();
   temperatureHelper.remove();
   musicHelper.remove();
}

void setAuto() {
   autoHelper.setColor(RED);
   manualHelper.setColor(BLUE);
   manualHelper.update();
   autoHelper.update();
   fanToggleHelper.remove();
   musicToggleHelper.remove();
   temperatureHelper.update();
   musicHelper.update();
}

void checkToggleButton() {
  if (autoHelper.isPressed()) {
    autoHelper.ack();
    setAuto();
  } else if (manualHelper.isPressed()) {
    manualHelper.ack();
    setManual();
  }
  
}

void changeDisplay(AndeeHelper helper, int newReading) {
  helper.setData(newReading);
}
void checkFan_MusicButton(){
  digitalWrite(13,HIGH);
 if(fanToggleHelper.isPressed()){
    fanToggleHelper.ack();
    if(fanToggle_State == 0){
    fanToggle_State = 1;
    fanToggleHelper.setColor(GREEN);
    fanToggleHelper.setTitle("Fan On");
    fanToggleHelper.update();
    }else{
     fanToggle_State = 0;
     fanToggleHelper.setColor(RED);
     fanToggleHelper.setTitle("Fan Off");
     fanToggleHelper.update();
     Serial.println("OFF");
     }
  }
  if(musicToggleHelper.isPressed()){
    musicToggleHelper.ack();
    if(musicToggle_State == 0){
    musicToggle_State = 1;
    musicToggleHelper.setColor(GREEN);
    musicToggleHelper.setTitle("Music On");
    musicToggleHelper.update();
    setMusicPin(HIGH);
    }else{
    musicToggle_State = 0;  
    musicToggleHelper.setColor(RED);
    musicToggleHelper.setTitle("Music Off");
    musicToggleHelper.update(); 
    setMusicPin(LOW);
    }
  } 
}

void setFanPin1(int value) {
  digitalWrite(fanPin1, value);
}
void setFanPin2(int value) {
  digitalWrite(fanPin2, value);
}
void setMusicPin(int value) {
  digitalWrite(musicPin1, value);
}

