#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string.h>
#include "core_simulation.h"
#define MAXVRPM 5500

class Composant: public Device {

protected:
    double m_freq;
    int m_cycleVie;
    int m_tmax;
    int m_percentage;
    int m_nbComposants;

public:
    Composant();
    Composant(double f, int v);  // Faire virtual pure abstrait pour pas y toucher
    virtual ~Composant();
    //virtual void benchmark(double m_freq);  // Création de classe abstraite avec utilisation de méthode virtuelle pure afin d'interdire l'utilisation de la classe composant
    //virtual void randomUse(double m_freq);
    virtual double getFreq() const;
    virtual int getTmax() const;
    virtual int percentageUse(); // Convertit la fréquence de fonctionnement en un pourcentage d'utilisation
    virtual void setFreq(double freq);
    virtual void setFreqRand();
    virtual void run();
};

//Capteur de tension (ou plus communément Voltmètre)
class TensionSensor: public Device {

protected:
    //valeur de la tension mesurée
    unsigned short val;
    //temps entree deux prises de valeurs
    int temps;

    Composant *m_processeur;

public:
    //constructeur
    TensionSensor( int t);
    double getTension();
    void setTension(double freq);
    void setProcesseur(Composant *processeur);
    virtual void run();

};

class Ventilator: public Device {

protected:
    int speed;
    bool mode; //0 auto vs 1 manual
    int temps;

public:
    Composant *processeur;
    TensionSensor *capteur;
    Ventilator(bool m, int t);
    int getSpeed();
    void setProcesseur(Composant *proces);
    void setCapteur(TensionSensor *capt);
    void setSpeedManuel(int s);
    void setSpeedAuto(int temp, double tension);
    virtual void run();
};





// exemple de capteur analogique de temperature, ne pas oublier d'heriter de Device
class AnalogSensorTemperature: public Device {
protected:
  // fait osciller la valeur du cpateur de 1
  int alea;
  // valeur de temperature mesuree
  int val;
  // temps entre 2 prises de valeurs
  int temps;

public:
  //constructeur ne pas oublier d'initialiser la classe mere
  AnalogSensorTemperature(int d, int t);
  virtual ~AnalogSensorTemperature();
  void setTempRand(int tempAmbiente);
  int getTemperature();
  virtual void run();
  // thread representant le capteur et permettant de fonctionner independamment de la board
};



class Ensemble: public Device {

protected:
    Composant *m_processeur;
    Ventilator *m_ventilo;
    AnalogSensorTemperature *m_capteurTemp;
    TensionSensor *m_capteurTension;
public:
    Ensemble(Composant *processeur, Ventilator *ventilo, AnalogSensorTemperature *capteurTemp, TensionSensor *capteurTension);
    virtual ~Ensemble();
    Composant *getProcesseur();
    Ventilator *getVentilo();
    AnalogSensorTemperature *getCapteurTemp();
    double calculCoeffB();
    int getTemp();
    int getColdTemp();
    void initialisation();
    virtual void run();

};



// exceptions gerees
enum excep2{TEMPCRITIQUE,TEMPDESTRUCTION};
class ComposantException{
protected:
    // numero de l'exception
  int num;
public:
  ComposantException(int i):num(i){}
    // recuperation du numero d'exception
    int get();
    // chaine expliquant l'exception
    string text();
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
