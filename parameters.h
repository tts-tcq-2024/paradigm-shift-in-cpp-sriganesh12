#ifndef PARAMETERS_H
#define PARAMETERS_H

struct parameters
{
float temperatureMax = 45;
float temperatureMin = 0;
float StateOfChargeMax = 80;
float StateOfChargeMin = 20;
float ChargeRateMax = 0.8;
float ChargeRateMin = 0;
float warningTolerance = 0.05;
bool warningForTemperature = true;
bool warningForStateOfCharge = true;
bool warningForChargeRate = true;
};

#endif
