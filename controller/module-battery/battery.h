
#ifndef CPRE288_BATTERY_H
#define CPRE288_BATTERY_H

void initializeBatteryReadings();
void updateBatteryReadings();
int getBatteryVoltage();
void transmitBatteryVoltage();

#endif // CPRE288_BATTERY_H
