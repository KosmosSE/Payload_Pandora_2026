#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>

struct dados {
    float temperature;
    float pressure;
    float altitude;
};


class GPS {

    public:

    private:
        TinyGPSPlus gps;

};
#endif