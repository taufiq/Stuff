#include <SPI.h>
#include <Andee.h>
#include "Wire.h"

#define DS1307_ADDRESS 0x68

bool automated = false;

const int lightHelperSensor = 0;
const int tempSensor = 1;

int counter = 1;
const int musicPin1 = 10;
const int fanPin1 = 9;
const int fanPin2 = 8;
int fanToggle_State = 0;
int musicToggle_State = 0;

int lightHelperReading;
int tempReading;
int timeReading[4];

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
  checkToggleButton();
  checkFan_MusicButton();
      setDate();
  updateRTCautomation();
}


byte bcdToDec(byte val) {
  return ((val/16*10) + (val%16));
}

void setDate() {
  Serial.println("setDate()");
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
//  Serial.print(month);
//  Serial.print("/");
//  Serial.print(monthDay);
//  Serial.print("/");
//  Serial.print(year);
//  Serial.print(" ");
//  Serial.print(hour);
//  Serial.print(":");
//  Serial.print(minute);
//  Serial.print(":");
//  Serial.println(second);
  
  timeReading[0] = second;
  timeReading[1] = minute;
  timeReading[2] = hour;
  timeReading[3] = weekDay;
}

void initialize() {
  Serial.println("initialize()");
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
  Serial.println("welcome()");
  if (welcomed) {
    welcomeHelper.update();
    delay(2000);
    welcomeHelper.remove();
    setManual();
    welcomed = false;
  }
}

void updateRTCautomation() {
  Serial.println("updateRTCautomation()");
  if (timeReading[3] != 6 || timeReading[3] != 7) {
    if ((timeReading[2] >= 17 && timeReading[2] <= 18) || (timeReading[2] >= 23 && timeReading[2] <= 3)) {
      setFanPin1(HIGH);
      setFanPin2(HIGH);
    } else {
      setFanPin1(LOW);
      setFanPin2(LOW);
    }
  } else {
    if ((timeReading[2] >= 11 && timeReading[2] <= 13) || (timeReading[2] >= 16 && timeReading[2] <= 18) || (timeReading[2] >= 23 && timeReading[2] <= 3)) {
      setFanPin1(HIGH);
      setFanPin2(HIGH);
    } else {
      setFanPin1(LOW);
      setFanPin2(LOW);
    }
  }
  Serial.println(timeReading[2]);
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

void checkFan_MusicButton() {
  digitalWrite(13,HIGH);
 if(fanToggleHelper.isPressed()){
    fanToggleHelper.ack();
    if(fanToggle_State == 0){
    fanToggle_State = 1;
    fanToggleHelper.setColor(GREEN);
    fanToggleHelper.setTitle("Fan On");
    fanToggleHelper.update();
    } else {
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
    } else {
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

