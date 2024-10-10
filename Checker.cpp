#include <assert.h>
#include <iostream>
#include "parameters.h"
using namespace std;

enum class Status {
    NO_BREACH,
    LOW_BREACH,
    HIGH_BREACH,
    LOW_WARNING,
    HIGH_WARNING
};

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

bool isTemperatureInRange(float temp, Status& TemperatureStatus)
{
  parameters Thresholds;
  if (Thresholds.warningForTemperature)
  {
    float MaxWarning = Thresholds.temperatureMax - Thresholds.warningTolerance*Thresholds.temperatureMax;
    float MinWarning = Thresholds.temperatureMin + Thresholds.warningTolerance*Thresholds.temperatureMax;
    UpdateHighWarningLimit(temp, MaxWarning, TemperatureStatus);
    UpdateLowWarningLimit(temp, MinWarning, TemperatureStatus);
  }
  UpdateHighBreachLimit(temp, Thresholds.temperatureMax, TemperatureStatus);
  UpdateLowBreachLimit(temp, Thresholds.temperatureMin, TemperatureStatus);
  return isInRange(TemperatureStatus);
}

bool isStateOfChargeInRange(float soc, Status& SOCStatus)
{
  parameters Thresholds;
  if (Thresholds.warningForStateOfCharge)
  {
    float MaxWarning = Thresholds.StateOfChargeMax - Thresholds.warningTolerance*Thresholds.StateOfChargeMax;
    float MinWarning = Thresholds.StateOfChargeMin + Thresholds.warningTolerance*Thresholds.StateOfChargeMax;
    UpdateHighWarningLimit(soc, MaxWarning, SOCStatus);
    UpdateLowWarningLimit(soc, MinWarning, SOCStatus);
  }
  UpdateHighBreachLimit(soc, Thresholds.StateOfChargeMax, SOCStatus);
  UpdateLowBreachLimit(soc, Thresholds.StateOfChargeMin, SOCStatus);
  return isInRange(SOCStatus);
}

bool isChargeRateInRange(float chargeRate, Status& ChargeRateStatus)
{
  parameters Thresholds;
  if (Thresholds.warningForChargeRate)
  {
    float MaxWarning = Thresholds.ChargeRateMax - Thresholds.warningTolerance*Thresholds.ChargeRateMax;
    UpdateHighWarningLimit(chargeRate, MaxWarning, ChargeRateStatus);
  }
  UpdateHighBreachLimit(chargeRate, Thresholds.ChargeRateMax, ChargeRateStatus); 
  return isInRange(ChargeRateStatus);
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
  
  bool isTemperatureOk{isTemperatureInRange(temperature, TemperatureStatus)};
  bool isStateOfChargeOk{isStateOfChargeInRange(soc, SOCStatus)};
  bool isChargeRateOk{isChargeRateInRange(chargeRate, ChargeRateStatus)};

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
