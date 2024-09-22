#include <assert.h>
#include <iostream>
using namespace std;

bool isTemperatureInRange(float temperature)
{
  return (temperature >= 0 && temperature <= 45);
}

bool isStateOfChargeInRange(float soc)
{
  return (soc >= 20 && soc <= 80);
}

bool isChargeRateInRange(float chargeRate)
{
  return (chargeRate <= 0.8);
}

bool batteryIsOk(float temperature, float soc, float chargeRate)
{
  bool isTemperatureOk{isTemperatureInRange(temperature)};
  bool isStateOfChargeOk{isStateOfChargeInRange(soc)};
  bool isChargeRateOk{isChargeRateInRange(chargeRate)};

  if(!isTemperatureOk)
  {
    cout << "Temperature out of range!\n";
  }
  if(!isStateOfChargeOk)
  {
    cout << "State of Charge out of range!\n";
  }
  if(!isChargeRateOk)
  {
    cout << "Charge Rate out of range!\n";
  }

  return isTemperatureOk && isStateOfChargeOk && isChargeRateOk;
}

int main() {
  assert(batteryIsOk(25, 70, 0.7) == true);
  assert(batteryIsOk(50, 65, 0) == false);
  assert(batteryIsOk(25, 85, 0.5) == false);
  assert(batteryIsOk(25, 70, 1) == false);
}
