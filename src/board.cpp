#include "core_simulation.h"

#include "mydevices.h"

#include <windows.h>



int main(){

  // creation d'une board

  Board esp8266;

  // achat des senseurs et actionneurs

  AnalogSensorTemperature temperature(DELAY,TEMP);

  DigitalActuatorLED led1(DELAY);

  I2CActuatorScreen screen;
  TensionSensor voltmetre(3, DELAY);
  Ventilator ventilo(0, DELAY);

  ventilo.setSpeed(0, 110, 1.5);




  // branchement des capteurs actionneurs

  esp8266.pin(2, voltmetre);
  esp8266.pin(1,temperature);
  esp8266.pin(0,led1);
  esp8266.i2c(1,screen);
  esp8266.pin(3, ventilo);



  // allumage de la carte

  esp8266.run();

  return 0;
}
