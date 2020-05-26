#include "mydevices.h"

#include <windows.h>

#include <cmath> // pour les calculs avec exponentionnel

#define sleep(x) Sleep(1000 * (x))



using namespace std;



//classe AnalogSensorTemperature

AnalogSensorTemperature::AnalogSensorTemperature(int d,int  t):Device(),val(t),temps(d),tempInt(0),m_freq2(0){

  alea=1;
  m_freq2 = new Composant(frequence); // appel du constructeur surchargé de Composant


}

AnalogSensorTemperature::~AnalogSensorTemperature()
{
    delete m_freq2;
}


AnalogSensorTemperature:: AnalogSensorTemperature(AnalogSensorTemperature const& VentiloACopier):Device(),val(t),temps(d),tempInt(0),m_freq2(0){

  alea=1;
  m_freq2 = new Composant(*(VentiloACopier.m_freq2)); // appel du constructeur surchargé de Composant


}





void AnalogSensorTemperature::run(AnalogSensorTemperature const& b){

  while(1){

    // alea=1-alea;

    b->GetTemp();
    val=m_tempInt;
    if(ptrmem!=NULL)

      *ptrmem=val;

    sleep(temps);

  }

}

void AnalogSensorTemperature::TempCold(AnalogSensorTemperature const& b){
    b->GetTemp();
    m_tempCold=m_tempInt+exp(-b.CalculCoeffA()-b.CalculCoeffB());
    if(ptrmem!=NULL)

      *ptrmem=m_tempCold;

    sleep(temps);
}




void AnalogSensorTemperature::GetTemp(){

m_tempInt=TEMP+(m_freq2.getTmax()-TEMP)*m_freq2.PercentageUse();
return m_tempInt;
}


double AnalogSensorTemperature::CalculCoeffA(){


     m_freqSensor=m_freq2.PercentageUse();
     if (m_freqSensor>=10)
        {
            m_CoeffA=-1+(m_freqSensor-10)x0,0111111111111;    // avec a pourcentage d'utilisation du proc entre 1(10%) et 0 (100%)
            else                                              // et entre 10(0%) et 1(10%) avec Temp proc= 25 à 50°C entre 0 et 10% pas besoin de ventilo
                                                              // = utilisation normale ventilo quasi au repos
            {
               m_CoeffA=10-m_freqSensor;
            }

        }

}


double AnalogSensorTemperature::CalculCoeffB(){


     m_SpeedVentilo=m_Vent2.getSpeed();
     m_SpeedVentilo=m_SpeedVentilo/5500;
     if (m_freq2.PercentageUse()>=10)
        {
            m_CoeffB=m_SpeedVentilo/10;    //   b pourcentage utilisaition des ventilateur entre 0(0%) et 0,1 (100%), Passer b de 0 à 0,2 si Tmax=115 pour descendre sous 100°C
            else
            {
               m_CoeffB=0;
            }

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

Composant::Composant() :Device(), m_freq(2,6), m_cycleVie(1000),tmax(100),m_Percentage(50)     //Constructeur par défaut de la classe composant
{

}

Composant::Composant(int F, int Vie;) :Device(), m_freq(F), m_cycleVie(V),tmax(100),m_Percentage(50)  //Constructeur surchargé de la classe composant, pas besoin de destructeur si on travaille sans pointeur
{

}



double Composant::getFreq() const
{
    return m_freq;
}

double Composant::getTmax() const
{
    return m_tmax;
}



int Composant::PercentageUse(int m_freq) const
{

    m_percentage=m_freq*20;
    return m_percentage;
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

