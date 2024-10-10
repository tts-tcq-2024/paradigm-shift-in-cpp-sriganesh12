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

void UpdateWarningLimit(float value, float max, float min, Status& status)
{
  if(value > max)
  {
    status = Status::HIGH_WARNING;
  }
  else if(value < min)
  {
    status = Status::LOW_WARNING;
  }
  else
  {
    //DO NOTHING
  }
}

void UpdateBreachLimit(float value, float max, float min, Status& status)
{
  if(value > max)
  {
    status = Status::HIGH_BREACH;
  }
  else if(value < min)
  {
    status = Status::LOW_BREACH;
  }
  else
  {
    //DO NOTHING
  }
}

bool isTemperatureInRange(float temp, Status& TemperatureStatus)
{
  parameters Thresholds;
  if (Thresholds.warningForTemperature)
  {
    float MaxWarning = Thresholds.temperatureMax - Thresholds.warningTolerance*Thresholds.temperatureMax;
    float MinWarning = Thresholds.temperatureMin + Thresholds.warningTolerance*Thresholds.temperatureMax;
    UpdateWarningLimit(temp, MaxWarning, MinWarning, TemperatureStatus);
  }
  UpdateBreachLimit(temp, Thresholds.temperatureMax, Thresholds.temperatureMin, TemperatureStatus); 
  return (TemperatureStatus==Status::NO_BREACH || TemperatureStatus==Status::HIGH_WARNING || TemperatureStatus==Status::LOW_WARNING);
}

bool isStateOfChargeInRange(float soc, Status& SOCStatus)
{
  parameters Thresholds;
  if (Thresholds.warningForStateOfCharge)
  {
    float MaxWarning = Thresholds.StateOfChargeMax - Thresholds.warningTolerance*Thresholds.StateOfChargeMax;
    float MinWarning = Thresholds.StateOfChargeMin + Thresholds.warningTolerance*Thresholds.StateOfChargeMax;
    UpdateWarningLimit(soc, MaxWarning, MinWarning, SOCStatus);
  }
  UpdateBreachLimit(soc, Thresholds.StateOfChargeMax, Thresholds.StateOfChargeMin, SOCStatus); 
  return (SOCStatus==Status::NO_BREACH || SOCStatus==Status::HIGH_WARNING || SOCStatus==Status::LOW_WARNING);
}

bool isChargeRateInRange(float chargeRate, Status& SOCStatus)
{
  parameters Thresholds;
  if (Thresholds.warningForChargeRate)
  {
    float MaxWarning = Thresholds.ChargeRateMax - Thresholds.warningTolerance*Thresholds.ChargeRateMax;
    float MinWarning = Thresholds.ChargeRateMin + Thresholds.warningTolerance*Thresholds.ChargeRateMax;
    UpdateWarningLimit(chargeRate, MaxWarning, MinWarning, SOCStatus);
  }
  UpdateBreachLimit(chargeRate, Thresholds.ChargeRateMax, Thresholds.ChargeRateMin, SOCStatus); 
  return (SOCStatus==Status::NO_BREACH || SOCStatus==Status::HIGH_WARNING || SOCStatus==Status::LOW_WARNING);
}

bool batteryIsOk(float temperature, float soc, float chargeRate)
{
  Status TemperatureStatus = Status::NO_BREACH; 
  Status SOCStatus = Status::NO_BREACH;
  Status ChargeRateStatus = Status::NO_BREACH;
  
  bool isTemperatureOk{isTemperatureInRange(temperature, TemperatureStatus)};
  bool isStateOfChargeOk{isStateOfChargeInRange(soc, SOCStatus)};
  bool isChargeRateOk{isChargeRateInRange(chargeRate, ChargeRateStatus)};

  return isTemperatureOk && isStateOfChargeOk && isChargeRateOk;
}

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true);
  assert(batteryIsOk(50, 65, 0) == false);
  assert(batteryIsOk(25, 85, 0.5) == false);
  assert(batteryIsOk(25, 70, 1) == false);
}
