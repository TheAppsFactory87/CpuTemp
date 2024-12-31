#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Measurement.h"

class Temperature : public Measurement<double> {
public:
    Temperature() : Measurement() {};
    double readValue();
};

#endif 