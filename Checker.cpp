#include <assert.h>
#include <iostream>
using namespace std;

bool isTemperatureInRange(float temperature)
{
  if(temperature < 0 || temperature > 45)
  {
    cout << "Temperature out of range!\n";
    return false;
  }
  return true;
}

bool isStateOfChargeInRange(float soc)
{
  if(soc < 20 || soc > 80) 
  {
    cout << "State of Charge out of range!\n";
    return false;
  }
  return true;
}

bool isChargeRateInRange(float chargeRate)
{
  if(chargeRate > 0.8) 
  {
    cout << "Charge Rate out of range!\n";
    return false;
  }
  return true;
}

bool batteryIsOk(float temperature, float soc, float chargeRate)
{
  bool isTemperatureOk{isTemperatureInRange(temperature)};
  bool isStateOfChargeOk{isStateOfChargeInRange(soc)};
  bool isChargeRateOk{isChargeRateInRange(chargeRate)};

  return isTemperatureOk && isStateOfChargeOk && isChargeRateOk;
}

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true);
  assert(batteryIsOk(50, 65, 0) == false);
  assert(batteryIsOk(25, 85, 0.5) == false);
  assert(batteryIsOk(25, 70, 1) == false);
}
