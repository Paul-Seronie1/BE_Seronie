#include "mydevices.h"

#include <windows.h>

#define sleep(x) Sleep(1000 * (x))



using namespace std;



//classe AnalogSensorTemperature

AnalogSensorTemperature::AnalogSensorTemperature(int d,int  t):Device(),val(t),temps(d){

  alea=1;

}



void AnalogSensorTemperature::run(){

  while(1){

    alea=1-alea;

    if(ptrmem!=NULL)

      *ptrmem=val+alea;

    sleep(temps);

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

