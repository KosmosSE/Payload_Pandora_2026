#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>

struct dados {
    float latitude;
    float longitude;
    float altitude;
};

class GPS {
    public:
        GPS();
        ~GPS();
        void setup();
        bool gpsIsValid();
        void displayInfo();
        void displayRawData();
        dados readData();

    private:
        TinyGPSPlus gps;
};


#endif