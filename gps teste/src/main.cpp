#include <Arduino.h>
#include "GPS.h"

GPS gps;


void setup() {
  Serial.begin(115200);
  gps.setup();
  Serial.println("OK");
}

void loop() {

  //gps.displayRawData();
  gps.displayInfo();

}