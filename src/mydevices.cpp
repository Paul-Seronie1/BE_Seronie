#include "mydevices.h"

#include <windows.h>

#include <cmath> // pour les calculs avec exponentionnel

#define sleep(x) Sleep(1000 * (x))


using namespace std;


//classe AnalogSensorTemperature

AnalogSensorTemperature::AnalogSensorTemperature(int d, int t,double frequence, int vie):Device(),val(t),temps(d),m_tempInt(0),m_freq2(0){

  alea=1;
  m_freq2 = new Composant(frequence, vie); // appel du constructeur surchargé de Composant


}

AnalogSensorTemperature::~AnalogSensorTemperature()
{
    delete m_freq2;
}


/*AnalogSensorTemperature:: AnalogSensorTemperature(AnalogSensorTemperature const& VentiloACopier,):Device(),val(VentiloACopier.val),temps(VentiloACopier.temps),tempInt(VentiloACopier.tempInt),m_freq2(0){

  alea=1;
  m_freq2 = new Composant(*(VentiloACopier.m_freq2)); // appel du constructeur surchargé de Composant


}
*/




void AnalogSensorTemperature::run(){

  while(1){

    // alea=1-alea;

    this->GetTemp();
    val=m_tempInt;
    if(ptrmem!=NULL)
      *ptrmem=val;
    sleep(temps);

  }
}

void AnalogSensorTemperature::TempCold(){

   while(1){
    m_tempCold=TEMP + (m_freq2->getTmax()-TEMP)*exp(-(this->calculCoeffA()+this->calculCoeffB()));
    if(ptrmem!=NULL)
      *ptrmem=m_tempCold;
    sleep(temps);
  }
}

void AnalogSensorTemperature::GetTemp(){
    m_tempInt=TEMP+(m_freq2->getTmax()-TEMP)*m_freq2->percentageUse();
}


double AnalogSensorTemperature::calculCoeffA(){

     m_freqSensorPer=m_freq2->percentageUse();
     if (m_freqSensorPer>=10)
    {
        m_CoeffA=-1+(m_freqSensorPer-10)*0.0111111111111;
    }                                                       // avec a pourcentage d'utilisation du proc entre 1(10%) et 0 (100%)
    else{
        m_CoeffA=10-m_freqSensorPer;
    }
}



double AnalogSensorTemperature::calculCoeffB(){


     m_SpeedVentiloPer=(double)m_Vent2->getSpeed()/5500;
     if (m_freq2->percentageUse()>=10)
    {
        m_CoeffB=m_SpeedVentiloPer/10;
    }  //   b pourcentage utilisaition des ventilateur entre 0(0%) et 0,1 (100%), Passer b de 0 à 0,2 si Tmax=115 pour descendre sous 100°C
    else
    {
        m_CoeffB=0;
    }
}








//class TensionSensor
TensionSensor::TensionSensor(int v, int t):Device(),val(v), temps(t)
{
}

void TensionSensor::run()
{
    while(1)
    {
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
    speed = 0;
}

int Ventilator::getSpeed()
{
    return speed;
}

void Ventilator::setSpeed(int s, int temp, float tension)
{
    if (mode) //mode manuel
    {
        speed = s;
        if (speed > MAXVRPM)
        {
            speed = MAXVRPM;
        }
    }
    else
    {
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
}

void Ventilator::run()
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
    return m_freq;
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



//Classe Ensemble

Ensemble::Ensemble(Composant processeur, Ventilator ventilo, AnalogSensorTemperature capteurTemp):Device(),m_processeur(processeur),m_ventilo(ventilo), m_capteurTemp(capteurTemp)
{

}

Ensemble::~Ensemble(){}


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

