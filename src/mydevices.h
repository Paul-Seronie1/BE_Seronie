#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string.h>
#include "core_simulation.h"
#define MAXVRPM 5500

class Ventilator: public Device {

private:
    int speed;
    bool mode; //0 auto vs 1 manual
    int temps;

public:
    Ventilator(bool m, int t);
    int getSpeed();
    void setSpeed(int s, int temp, float tension);
    virtual void run();
};


class Composant: public Device {

private:
    double m_freq;
    int m_cycleVie;
    int tmax;
    int m_Percentage;


public:
    Composant();
    Composant(double f, int v);  // Faire virtual pure abstrait pour pas y toucher
    virtual void benchmark(double m_freq);  // Création de classe abstraite avec utilisation de méthode virtuelle pure afin d'interdire l'utilisation de la classe composant
    virtual void randomUse(double m_freq);
    virtual double getFreq() const;
    virtual double getTmax() const;
    virtual int PercentageUse(double m_freq); // Convertit la fréquence de fonctionnement en un pourcentage d'utilisation
};



// exemple de capteur analogique de temperature, ne pas oublier d'heriter de Device
class AnalogSensorTemperature: public Device {
private:
    Composant *m_freq2; // On créer un pointeur vers la classe Composant
  Ventilator *m_Vent2; // On créer un pointeur vers la classe Ventilator

  // fait osciller la valeur du cpateur de 1
  int alea;
  // valeur de temperature mesuree
  int val;
  // valeur de temperature intermédiaire
  int m_tempInt;
  int m_tempCold;
  int m_freqSensor;
  int m_SpeedVentilo;
  double m_CoeffA;
  double m_CoeffB;
  // temps entre 2 prises de valeurs
  int temps;


public:
  //constructeur ne pas oublier d'initialiser la classe mere
  AnalogSensorTemperature(int d,int  t,double frequence, int vie);
  ~AnalogSensorTemperature();
  AnalogSensorTemperature(AnalogSensorTemperature const& VentiloACopier);
  // thread representant le capteur et permettant de fonctionner independamment de la board
  virtual void run(AnalogSensorTemperature &b);
  virtual void TempCold(AnalogSensorTemperature &b);
};

//Capteur de tension (ou plus communément Voltmètre)
class TensionSensor: public Device {

private:
    //valeur de la tension mesurée
    int val;
    //temps entree deux prises de valeurs
    int temps;

public:
    //constructeur
    TensionSensor(int v, int t);
    virtual void run();
};




/*class CPU: public Composant {

private:
    int m_nbrCoeur;  //Nombre de coeur du processeur
    int m_nbrThread;  //Nombre de thread du processeur


public:
    Composant();
    Composant(int m_freq);

};*/




/*class GPU: public Composant {

private:
    int m_memory;  //Mémoire de la carte graphique


public:
    Composant();
    Composant(int m_freq);
    void benchmark(int m_freq);
    void randomUse(int m_freq);
};

*/




// exemple d'actionneur digital : une led, ne pas oublier d'heriter de Device
class DigitalActuatorLED: public Device {
private:
  // etat de la LED
  int state;
  // temps entre 2 affichage de l etat de la led
  int temps;

public:
    // initialisation du temps de rafraichiisement
  DigitalActuatorLED(int t);
  // thread representant l'actionneur et permettant de fonctionner independamment de la board
  virtual void run();
};

// exemple d'actionneur sur le bus I2C permettant d'echanger des tableaux de caracteres : un ecran, ne pas oublier d'heriter de Device
class I2CActuatorScreen : public Device{
protected:
    // memorise l'affichage de l'ecran
  char buf[I2C_BUFFER_SIZE];

public:
  // constructeur
  I2CActuatorScreen ();
  // thread representant le capteur et permettant de fonctionner independamment de la board
  virtual void run();
};

#endif
