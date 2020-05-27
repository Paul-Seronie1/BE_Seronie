#ifndef MYDEVICES_H
#define MYDEVICES_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string.h>
#include "core_simulation.h"
#define MAXVRPM 5500

//Repr�sente la classe m�re des processeurs (CPU/GPU)
class Composant: public Device {

protected:
    double m_freq;
    int m_cycleVie; //usure du composant
    int m_tmax;
    int m_percentage;
    int m_nbComposants;

public:
    Composant();
    Composant(double f, int v);  // Faire virtual pure abstrait pour pas y toucher
    virtual ~Composant();
    virtual double getFreq() const;
    virtual int getTmax() const;
    virtual int percentageUse(); // Convertit la fr�quence de fonctionnement en un pourcentage d'utilisation
    virtual void setFreq(double freq);
    virtual void setFreqRand();
    virtual void run();
};

//Capteur de tension (ou plus commun�ment Voltm�tre)
class TensionSensor: public Device {

protected:
    //valeur de la tension mesur�e
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
//Classe ventilateur
class Ventilator: public Device {

protected:
    int speed;
    bool mode; //0 auto vs 1 manual
    int temps;

public:
    //Autres Device li�s au ventilateur pour le r�guler
    Composant *processeur;
    TensionSensor *capteur;
    Ventilator(bool m, int t);
    int getSpeed();
    void setProcesseur(Composant *proces);
    void setCapteur(TensionSensor *capt);
    //dif�rents modes d'utilisation du ventiateur (manuel/auto)
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


// Classe permettant de mettre en lien plusieurs device et de simuler le somportement du processeur r�gul�
class Ensemble: public Device {

protected:
    //Diff�rents Device le composant
    Composant *m_processeur;
    Ventilator *m_ventilo;
    AnalogSensorTemperature *m_capteurTemp;
    TensionSensor *m_capteurTension;
public:
    Ensemble(Composant *processeur, Ventilator *ventilo, AnalogSensorTemperature *capteurTemp, TensionSensor *capteurTension);
    virtual ~Ensemble();
    //Diff�rents assesseurs
    Composant *getProcesseur();
    Ventilator *getVentilo();
    AnalogSensorTemperature *getCapteurTemp();
    //Partue mod�lisation math�matiques
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
    int m_memory;  //M�moire de la carte graphique
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
