#include <unistd.h>

#include "core_simulation.h"

#include <windows.h>

#define sleep(x) Sleep(1000 * (x))

// la fonction d'initialisation d'arduino

void Board::setup(){

  // on configure la vitesse de la liaison

  Serial.begin(9600);

// on fixe les pin en entree et en sorite en fonction des capteurs/actionneurs mis sur la carte

  pinMode(1,INPUT2);
  pinMode(2, INPUT2);
  pinMode(0,OUTPUT);
  pinMode(3, INPUT2);
  pinMode(4, INPUT2);
  pinMode(5, INPUT2);


}
// la boucle de controle arduino

void Board::loop(){

  char buf[100];

  int tempAmb;

  double volt;

  int vitesse;

  double freq;

  int tempProc;

  static int cpt=0;

  static int bascule=0;

  int i=0;


  for(i=0;i<10;i++){

    // lecture sur la pin 1 : capteur de temperature
    tempAmb=analogRead(1);
    sprintf(buf,"Temperature Ambiante %d(C)",tempAmb);
    Serial.println(buf);

    tempProc=digitalRead(5);
    sprintf(buf,"Temperature Processeur %d(C)",tempProc);
    Serial.println(buf);

    freq=digitalRead(4)/(double)13107;
    sprintf(buf,"Frequence %f GHz",freq);
    Serial.println(buf);

    volt=digitalRead(2)/(double)38102;
    sprintf(buf,"Tension Processeur %f V",volt);
    Serial.println(buf);

    vitesse=digitalRead(3);
    sprintf(buf,"Vitesse Ventilateur %d tr/min",vitesse);
    Serial.println(buf);


    if(cpt%5==0){

        // tous les 5 fois on affiche sur l ecran la temperature

      sprintf(buf,"%d",tempAmb);

      bus.write(1,buf,100);

    }

    cpt++;

    sleep(2);

  }

// on eteint et on allume la LED

  if(bascule)

    digitalWrite(0,HIGH);

  else

    digitalWrite(0,LOW);

  bascule=1-bascule;
}
