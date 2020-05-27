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
int AnalogSensorTemperature::getTemperature(){
return val;
}
void AnalogSensorTemperature::run(){
  while(1){
    this->setTempRand(25);
    if(ptrmem!=NULL)
      *ptrmem=val;
    sleep(temps);
  }
}


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
void TensionSensor::run()
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
}

//class Ventilator
Ventilator::Ventilator(bool m, int t):Device(),mode(m), temps(t)
{
    speed =0;
    processeur = new Composant(2.5, 1000);
    capteur = new TensionSensor(2);
}
void Ventilator::setProcesseur(Composant *proces){
    processeur = proces;
}
void Ventilator::setCapteur(TensionSensor *capt){
    capteur = capt;
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

void Ventilator::setSpeedAuto(int temp, double tension){
    if (temp < 50 && tension < 0.7)
    {
        speed = 0;
    }
    else
    {
        if (110*(temp-50) > (int)5392*(tension-0.7)) //On regarde qui de la tension ou de la fréquence est critique
        {
            speed = 110*(temp-50);
        }
        else
        {
            speed = (int)5392*(tension-0.7);
        }
        /*if (speed > MAXVRPM)
        {
            speed = MAXVRPM;
        }*/
    }
}

void Ventilator::run()
{
    while(1)
    {
        if (mode)
        {
           cout << "Mode manuel" << endl;
           this->setSpeedManuel(1000);

        }
        else
        {
            cout << "Mode auto" << endl;
            this->setSpeedAuto(TEMP+((processeur->getTmax()-TEMP)*processeur->percentageUse())/(double)100, capteur->getTension()/(double)38102);
        }
        if (ptrmem!=NULL)
        {
            *ptrmem = speed;
        }

        sleep(temps);
    }
}


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
    m_percentage=(m_freq*20)/(double)13107;
    return m_percentage;
}

void Composant::setFreq(double freq){
    m_freq = freq;
}

void Composant::setFreqRand(){
    int a = 0;
    int b = 5;
    srand(time(NULL)*577);
    double c = 0.5;
    double d = 1;
    double coeff;
    srand(time(NULL)*577);
    coeff = ((rand()/(double)RAND_MAX)*(d-c) + c);
    m_freq = coeff*(13107*((rand()/(double)RAND_MAX)*(b-a) + a)); //En MHz
}

void Composant::run(){
    sleep(1);
    while (1){
        this->setFreqRand();
        if (ptrmem!=NULL)
        {
            *ptrmem = m_freq;
        }
    sleep(2);
    }
}

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


double Ensemble::calculCoeffB(){
     double resultat;
        resultat=((double)(m_ventilo->getSpeed()/(double)5500))/10;
    //   b pourcentage utilisaition des ventilateur entre 0(0%) et 0,1 (100%), Passer b de 0 à 0,2 si Tmax=115 pour descendre sous 100°C
    return resultat;
}

int Ensemble::getTemp(){
    return m_capteurTemp->getTemperature()+((m_processeur->getTmax()-m_capteurTemp->getTemperature())*m_processeur->percentageUse())/(double)100;
}
int Ensemble::getColdTemp(){
    if (m_ventilo->getSpeed() == 0){
        return this->getTemp();
    }
    return this->getTemp()*exp(-this->calculCoeffB());
}
void Ensemble::initialisation(){
    m_processeur->setFreqRand();
    m_capteurTension->setTension(m_processeur->getFreq());
    m_ventilo->setSpeedAuto(this->getTemp(), m_capteurTension->getTension());
}

void Ensemble::run(){
    while (1){
        if (ptrmem!=NULL){
            *ptrmem=this->getTemp();
        }

        if (m_ventilo->getSpeed()==0){
            cout << "Aucune action des ventilateurs, la temperature ne va pas changer." << endl;
        }
        else {
            cout << "Le processeur va atteindre une temperature de " << this->getColdTemp() << " degres Celsius apres l'action des ventilateurs." << endl;
        }
        /*
        m_processeur->setFreqRand();
        m_capteurTension->setTension(m_processeur->getFreq());
        m_ventilo->setSpeedAuto(this->getTemp(), m_capteurTension->getTension());
        m_processeur->run();
        m_capteurTension->run();
        m_ventilo->run(0, this->getTemp(), m_capteurTension->getTension());
        m_capteurTemp->run();*/
        sleep(2);
    }


}




// class ComposantException
int ComposantException::get(){return num;}
string ComposantException::text(){
  string s;
  switch(num){
  case TEMPCRITIQUE : s=string("Temperature superieur a 90 degres, temperature critique, on reduit la duree de vie du processeur");break;
  case TEMPDESTRUCTION : s=string("Temperature superieur a 100 degres, risque de destruction du processeur"); break;
  default: s=string("erreur inconnue");
  }
  return s;
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
