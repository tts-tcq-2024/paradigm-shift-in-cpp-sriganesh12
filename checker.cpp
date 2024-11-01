#include <assert.h>
#include <iostream>
#include "parameters.h"
#include "statusEnum.h"
using namespace std;

bool isInRange(Status status)
{
    return (status==Status::NO_BREACH || status==Status::HIGH_WARNING || status==Status::LOW_WARNING);
}

void UpdateHighWarningLimit(float value, float max, Status& status)
{
  if(value > max)
  {
    status = Status::HIGH_WARNING;
  }
}

void UpdateLowWarningLimit(float value, float min, Status& status)
{
  if(value < min)
  {
    status = Status::LOW_WARNING;
  }
}

void UpdateHighBreachLimit(float value, float max, Status& status)
{
  if(value > max)
  {
    status = Status::HIGH_BREACH;
  }
}

void UpdateLowBreachLimit(float value, float min, Status& status)
{
  if(value < min)
  {
    status = Status::LOW_BREACH;
  }
}

bool isParameterInRange(float Parametervalue, Status& ParameterStatus, Parameters ParameterThreshold)
{
  if (ParameterThreshold.warningEnabled)
  {
    float MaxWarning = ParameterThreshold.maxLimit - ParameterThreshold.warningTolerance*ParameterThreshold.maxLimit;
    float MinWarning = ParameterThreshold.minLimit + ParameterThreshold.warningTolerance*ParameterThreshold.maxLimit;
    UpdateHighWarningLimit(Parametervalue, MaxWarning, ParameterStatus);
    UpdateLowWarningLimit(Parametervalue, MinWarning, ParameterStatus);
  }
  UpdateHighBreachLimit(Parametervalue, ParameterThreshold.maxLimit, ParameterStatus);
  UpdateLowBreachLimit(Parametervalue, ParameterThreshold.minLimit, ParameterStatus);
  return isInRange(ParameterStatus);
}

string getWarningMessage(const string& parameter, Status status) {
    switch (status) 
    {
        case Status::LOW_WARNING:
            return "Warning: " + parameter + " is approaching lower threshold.";
        case Status::HIGH_WARNING:
            return "Warning: " + parameter + " is approaching peak threshold.";
        default:
            return "";
    }
}

string getBreachMessage(const string& parameter, Status status) {
    switch (status) 
    {
        case Status::LOW_BREACH:
            return "Breached: " + parameter + " is exceeding lower threshold.";
        case Status::HIGH_BREACH:
            return "Warning: " + parameter + " is exceeding peak threshold.";
        default:
            return "";
    }
}

string statusToMessageTranslation(const string& parameter, Status status) {
    string message = getWarningMessage(parameter, status);
    if (message.empty())
    {
        message = getBreachMessage(parameter, status);
    }
    if (message.empty())
    {
        message = parameter + " is normal.";
    }
    return message;
}

bool batteryIsOk(float temperature, float soc, float chargeRate)
{
  Status TemperatureStatus = Status::NO_BREACH; 
  Status SOCStatus = Status::NO_BREACH;
  Status ChargeRateStatus = Status::NO_BREACH;

  Parameters TemperatureThreshold;
  TemperatureThreshold.maxLimit = 45;
  TemperatureThreshold.minLimit = 0;
  Parameters SOCThreshold;
  SOCThreshold.maxLimit = 80;
  SOCThreshold.minLimit = 20;
  Parameters ChargeRateThreshold;
  ChargeRateThreshold.maxLimit = 0.8;
  ChargeRateThreshold.minLimit = 0;
  
  bool isTemperatureOk{isParameterInRange(temperature, TemperatureStatus, TemperatureThreshold)};
  bool isStateOfChargeOk{isParameterInRange(soc, SOCStatus, SOCThreshold)};
  bool isChargeRateOk{isParameterInRange(chargeRate, ChargeRateStatus, ChargeRateThreshold)};

  std::cout << statusToMessageTranslation("Temperature",TemperatureStatus) << std::endl;
  std::cout << statusToMessageTranslation("State of Charge", SOCStatus) << std::endl;
  std::cout << statusToMessageTranslation("Charge Rate", ChargeRateStatus) << std::endl;

  return isTemperatureOk && isStateOfChargeOk && isChargeRateOk;
}

int main()
{
    assert(batteryIsOk(28, 74, 0.6) == true); // All Parameters in Range
    assert(batteryIsOk(48, 83, 0.4) == false); // Temperature Breach
    assert(batteryIsOk(-5, 52, 0.6) == false); // Temperature Breach
    assert(batteryIsOk(30, 10, 0.5) == false); // Soc Breach
    assert(batteryIsOk(25, 60, 0.85) == false); // Charge rate Breach
    assert(batteryIsOk(43, 76, 0.6) == true); // All Params in Range(Temperature and Soc in warning stage)
    assert(batteryIsOk(2, 22, 0.78) == true); // All Params in Range(Temperature, Soc and ChargeRate in Warning Stage)
}
