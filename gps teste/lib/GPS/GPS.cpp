#include "GPS.h"

GPS::GPS() {}

GPS::~GPS() {}

void GPS::setup() {
    Serial2.begin(9600);
}

bool GPS::gpsIsValid() {

    while (Serial2.available() > 0) {
        if (gps.encode(Serial2.read())) {
            if (gps.location.isUpdated()) {
                return true;
            }
        }
    }
    return false;

}

void GPS::displayInfo() {

    if (gps.encode(Serial2.read())){
        Serial.print(F("Location: "));
        Serial.print("Lat: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print("Lng: ");
        Serial.print(gps.location.lng(), 6);
        Serial.print(F(","));
        Serial.print("sat: ");
        Serial.print( gps.satellites.value());
        Serial.println();
    }

    


}

void GPS::displayRawData() {

  while (Serial2.available() > 0) {
    char c = Serial2.read();
    Serial.print(c);
  }

}

dados GPS::readData() {
    dados dadosGPS;

    if(gpsIsValid()) {
        dadosGPS.latitude = gps.location.lat();
        dadosGPS.longitude = gps.location.lng();
    } else {
        dadosGPS.latitude = 0;
        dadosGPS.longitude = 0;
    }

    return dadosGPS;
}