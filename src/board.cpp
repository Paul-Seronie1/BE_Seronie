#include "core_simulation.h"

#include "mydevices.h"

#include <windows.h>

using namespace std;

double frand_a_b(double a, double b){

    return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

int main(){

  // creation d'une board

  Board esp8266;

  // achat des senseurs et actionneurs



  DigitalActuatorLED led1(DELAY);

  I2CActuatorScreen screen;

  TensionSensor voltmetre(DELAY);
  Ventilator ventilo(0, DELAY);
  Composant CPU1(4, 1000);
  voltmetre.setProcesseur(&CPU1);
  AnalogSensorTemperature temperature(DELAY,TEMP);

  Ensemble systeme1(&CPU1, &ventilo, &temperature, &voltmetre);
  systeme1.initialisation();


  // branchement des capteurs actionneurs

  esp8266.pin(2, voltmetre);
  esp8266.pin(1,temperature);
  esp8266.pin(0,led1);
  esp8266.i2c(1,screen);
  esp8266.pin(3, ventilo);
  esp8266.pin(4, CPU1);



  // allumage de la carte

  esp8266.run();
  return 0;

}
