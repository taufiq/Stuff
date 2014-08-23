#include <SPI.h>
#include <Andee.h>

int brightness;
int receivedBrightness;
float multiplier = 1.0f;
const float readingCap = 500.0f;
const float brightnessCap = 5000.0f;
const float multiplierCap = brightnessCap / readingCap;
int led = 10;
AndeeHelper displayBoxA;

void setup() {
  Andee.begin();
  Andee.clear();
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(0, INPUT);
  
  displayBoxA.setId(0);
  displayBoxA.setType(DATA_OUT);
  displayBoxA.setLocation(0,0, FULL);
  displayBoxA.setTitle("Title");
  displayBoxA.setData("Data");
  displayBoxA.setUnit("Unit");
}

void loop() {
  int reading = analogRead(0);
  if (reading >= readingCap) {
    reading = readingCap - 1;
  }
  multiplier = multiplierCap - reading / (readingCap / multiplierCap);
  receivedBrightness = readingCap - reading;
  brightness = receivedBrightness * multiplier;
  if (brightness <= 0) {
    return;
  }
  Serial.println(reading);
  delayMicroseconds(10000 - brightness);
  digitalWrite(led, HIGH);
  delayMicroseconds(brightness);
  digitalWrite(led, LOW);
}
