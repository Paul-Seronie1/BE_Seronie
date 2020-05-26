#include "mydevices.h"
#include "core_simulation.h"
#include <windows.h>

#include <cmath> // pour les calculs avec exponentionnel

#define sleep(x) Sleep(1000 * (x))


using namespace std;


//classe AnalogSensorTemperature

AnalogSensorTemperature::AnalogSensorTemperature(int d, int t):Device(),val(t),temps(d){

  alea=1;
}

AnalogSensorTemperature::~AnalogSensorTemperature()
{
}

void AnalogSensorTemperature::setTempRand(int tempAmbiente){
    int a = tempAmbiente+2;
    int b = tempAmbiente-2;
    srand(time(NULL)*577);
    val = (int)(rand()/(double)RAND_MAX * (b-a) + a);
}

/*void AnalogSensorTemperature::run(){
  while(1){
    this->setTempRand(25);
    if(ptrmem!=NULL)
      *ptrmem=val;
    sleep(temps);
  }
}*/


//class TensionSensor
TensionSensor::TensionSensor(int t):Device(),val(0), temps(t)
{
    m_processeur = new Composant(2.5, 1);
}

double TensionSensor::getTension(){

    return val;
}

void TensionSensor::setProcesseur(Composant *processeur){
    m_processeur = processeur;
}

void TensionSensor::setTension(double freq){
    val = freq*0.344*38102; //en mV

}
/*void TensionSensor::run()
{
    while(1)
    {
        setTension(m_processeur->getFreq());
        if (ptrmem!=NULL)
        {
            *ptrmem=val;
        }
        sleep(temps);
    }
}*/

//class Ventilator
Ventilator::Ventilator(bool m, int t):Device(),mode(m), temps(t)
{
    speed = 0;
}

int Ventilator::getSpeed()
{
    return speed;
}

void Ventilator::setSpeedManuel(int s)
{
        speed = s;
        if (speed > MAXVRPM)
        {
            speed = MAXVRPM;
        }
}

void Ventilator::setSpeedAuto(int temp, int tension){
    if (temp < 50 && tension < 0.7)
    {
        speed = 0;
    }
    else
    {
        if (110*(temp-50) > (int)5392*(tension-0.7))
        {
            speed = 110*(temp-50);
        }
        else
        {
            speed = (int)5392*(tension-0.7);
        }
        if (speed > MAXVRPM)
        {
            speed = MAXVRPM;
        }
    }
}

/*void Ventilator::run()
{
    while(1)
    {
        if (ptrmem!=NULL)
        {
            *ptrmem = speed;
        }
        if (mode)
        {
           cout << "Mode manuel, speed = " << speed << endl;
        }
        else
        {
            cout << "Mode auto, speed = " << speed << endl;
        }
        sleep(temps);
    }
}*/


//classe Composant

Composant::Composant() :Device(), m_freq(2.6), m_cycleVie(1000),m_tmax(100),m_percentage(50)     //Constructeur par défaut de la classe composant
{
    m_nbComposants++;
}

Composant::Composant(double f, int v) :Device(), m_freq(f), m_cycleVie(v), m_tmax(100), m_percentage(50)  //Constructeur surchargé de la classe composant, pas besoin de destructeur si on travaille sans pointeur
{
    m_nbComposants++;
}

Composant::~Composant(){m_nbComposants--;}

double Composant::getFreq() const
{
    return m_freq/(double)13107;
}

int Composant::getTmax() const
{
    return m_tmax;
}


int Composant::percentageUse()
{
    m_percentage=m_freq*20;
    return m_percentage;
}

void Composant::setFreq(double freq){
    m_freq = freq;
}

void Composant::setFreqRand(){
    int a = 0;
    int b = 5;
    srand(time(NULL)*577);
    m_freq = 13107*((rand()/(double)RAND_MAX)*(b-a) + a); //En MHz
}

/*void Composant::run(){
    sleep(1);
    while (1){
        this->setFreqRand();
        if (ptrmem!=NULL)
        {
            *ptrmem = m_freq;
        }
    sleep(2);
    }
}*/

//Classe Ensemble

Ensemble::Ensemble(Composant *processeur, Ventilator *ventilo, AnalogSensorTemperature *capteurTemp, TensionSensor *capteurTension):Device(),
m_processeur(processeur),m_ventilo(ventilo),
m_capteurTemp(capteurTemp), m_capteurTension(capteurTension)
{

}

Ensemble::~Ensemble(){}

Composant *Ensemble::getProcesseur(){
    return m_processeur;
}
Ventilator *Ensemble::getVentilo(){
    return m_ventilo;
}
AnalogSensorTemperature *Ensemble::getCapteurTemp(){
    return m_capteurTemp;
}
double Ensemble::calculCoeffA(){
    double resultat;
     if (m_processeur->percentageUse()>=10)
    {
        resultat=-1+(m_processeur->percentageUse()-10)*0.0111111111111;
    }                                                       // avec a pourcentage d'utilisation du proc entre 1(10%) et 0 (100%)
    else{
        resultat=10-m_processeur->percentageUse();
    }
    return resultat;
}


double Ensemble::calculCoeffB(){
     double resultat;
     if (m_processeur->percentageUse()>=10)
    {
        resultat=(double)(m_ventilo->getSpeed()/(double)5500)/10;
    }  //   b pourcentage utilisaition des ventilateur entre 0(0%) et 0,1 (100%), Passer b de 0 à 0,2 si Tmax=115 pour descendre sous 100°C
    else
    {
        resultat=0;
    }
    return resultat;
}

int Ensemble::getTemp(){
    return TEMP+(m_processeur->getTmax()-TEMP)*m_processeur->percentageUse();
}
int Ensemble::getColdTemp(){
    return TEMP + (m_processeur->getTmax()-TEMP)*exp(-(this->calculCoeffA()+this->calculCoeffB()));
}
void Ensemble::initialisation(){
    m_processeur->setFreqRand();
    m_capteurTension->setTension(m_processeur->getFreq());
    m_ventilo->setSpeedAuto(this->getTemp(), m_capteurTension->getTension());
}

void Ensemble::run(){
    while (1){

        m_processeur->setFreqRand();
        m_capteurTension->setTension(m_processeur->getFreq());
        m_ventilo->setSpeedAuto(this->getTemp(), m_capteurTension->getTension());
        m_capteurTemp->setTempRand(25);
        if (m_processeur->ptrmem!=NULL){
            m_processeur->*ptrmem = m_processeur->getFreq();
        }
        if (m_capteurTension->ptrmem!=NULL){
            m_capteurTension->*ptrmem = m_capteurTension->getTension();
        }
        if (m_ventilo->ptrmem!=NULL){
            m_ventilo->*ptrmem = m_ventilo->getSpeed();
        }
        if (m_capteurTemp->ptrmem!=NULL){
            m_capteurTemp->*ptrmem = m_capteurTemp->getTemp();
        }

        sleep(2);
    }
}

//Actuateur






//classe DigitalActuatorLED

DigitalActuatorLED::DigitalActuatorLED(int t):Device(),state(LOW),temps(t){

}



void DigitalActuatorLED::run(){

  while(1){

    if(ptrmem!=NULL)

      state=*ptrmem;

    if (state==LOW)

      cout << "((((eteint))))\n";

    else

    cout << "((((allume))))\n";

    sleep(temps);

    }

}



// classe I2CActuatorScreen

I2CActuatorScreen::I2CActuatorScreen ():Device(){

  }



void I2CActuatorScreen::run(){

  while(1){

    if ( (i2cbus!=NULL)&&!(i2cbus->isEmptyRegister(i2caddr))){

      Device::i2cbus->requestFrom(i2caddr, buf, I2C_BUFFER_SIZE);

      cout << "---screen :"<< buf << endl;


    }

    sleep(1);

    }

}

