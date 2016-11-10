/**
 * Écriture sur une carte SD
 *
 * SD card reliée au bus SPI :
 * MOSI       - pin 11
 * MISO       - pin 12
 * CLK ou SCK - pin 13
 * CS         - pin 4
 *
 * SPI pour Serial Peripheral Interface
 *
 * created  24 Nov 2010
 * modified 9 Apr 2012
 * by Tom Igoe
 * cf: https://www.arduino.cc/en/Tutorial/Datalogger
 */
#include <SPI.h>
#include <SD.h>
#define TENMIN (1000UL * 60 * 10);
// Arduino Uno pin 4
// cf: https://www.arduino.cc/en/Reference/SPI
const int chipSelect = 4;

unsigned long rolltime = millis() + TENMIN;

#include <DallasTemperature.h>

/**
 * Lecture de température avec un seul
 * capteur DS18B20 (le premier)
 */

/** 
 *  Nous utilisons la librairie OneWire
 *  Elle doit être présente dans le répertoire libraries
 *  situé dans le répertoire des croquis/sketchs
 *  voir dans le menu Préférences
 *  cf: https://github.com/PaulStoffregen/OneWire/archive/master.zip
 */
#include <OneWire.h>

// nous utilisons la librairie RTClib
//cf: https://github.com/adafruit/RTClib
 #include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

/**
 * Nous utilisons aussi la librairie DallasTemperature
 * cf: https://github.com/milesburton/Arduino-Temperature-Control-Library
 */
#include <DallasTemperature.h>
 
// Le fil des données (jaune?) est relié au pin 2 du Arduino
#define ONE_WIRE_BUS 2
 
/** 
 * Création d'une instance oneWire afin de pouvoir 
 * communiquer avec des périphériques OneWire
 * Nb: pas seulement des capteurs Maxim/Dallas
 */ 
OneWire oneWire(ONE_WIRE_BUS);
 
/**
 * Nous passons notre instance oneWire à notre 
 * objet sensors utilisant DallasTemperature
 */
DallasTemperature sensors(&oneWire);
 
void setup(void)
{
  // Initialisation du port série
  Serial.begin(9600);
  Serial.println("Demo de la librairie Dallas Temperature");

  // Initialisation de notre librairie
  sensors.begin();

   Wire.begin();
      RTC.begin();
      if (! RTC.isrunning())
      {
        Serial.println("RTC is NOT running!");
      }
      // Décommenter cette ligne pour mettre à jour l'heure dans le bidule
      // RTC.adjust(DateTime(__DATE__, __TIME__));


//zone SD
  while (!Serial)
    {
    ; // wait for serial port to connect. Needed for native USB port only
 
    }
  
  Serial.println("Initialisation de la carte SD ...");
 
  // on verifie que la carte est présente et peut être initialisée
  if (!SD.begin(chipSelect))
    {
    Serial.println("Carte Sd inaccesible ou absente");
    // don't do anything more:
    return;
    }
  Serial.println("Carte OK");
}
 
 
void loop(void)
{
   if((long)(millis() - rolltime) >= 0) 
{
  /**
   * Nous interrogeons notre capteur
  
  Serial.print(" Demande de temperature...");
  */
  sensors.requestTemperatures(); // envoi de la demande
 /*
  Serial.println("Fait");

  Serial.print("La temperature du premier capteur est : ");
  Serial.println(sensors.getTempCByIndex(0));
  Serial.print("La temperature du deuxieme capteur est : ");
  Serial.println(sensors.getTempCByIndex(1)); 
  */
  /**
   * 0 correspond au premier capteur du bus
   * nous pouvons en relier plusieurs
   * et pouvons les interroger en changeant cet index
   */

    DateTime now = RTC.now();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
      delay(1000);

             
 

// on enregistre année; moi; jour; heure; minute; seconde; T1; T2
//Zone SD
  // nous créons une chaîne de caractères pour
  // concaténer les données à écrire :
  String dataString = "";

  // nous convertissons la valeur 
  // avec l'objet String() afin de pouvoir 
  // l'écrire sur la carte
  dataString += String(now.year(), DEC);
  dataString += ";";
  dataString += String(now.month(), DEC);
  dataString += ";";
  dataString += String(now.day(), DEC);
  dataString += ";";
  dataString += String(now.hour(), DEC);
  dataString += ";";
  dataString += String(now.minute(), DEC);
  dataString += ";";
  dataString += String(now.second(), DEC);
  dataString += ";";
  dataString += String(sensors.getTempCByIndex(0));
  dataString +=  ";";
  dataString += String(sensors.getTempCByIndex(1));
  dataString += ";";
  
  /** lecture de trois capteurs reliés de A0 à A2:
  for (int analogPin = 0; analogPin < 3; analogPin++) 
  {

    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) 
    {
      // Nous ajoutons un caractère de séparation
      dataString += ";";
    }
    
  }
*/

  /**
   * nous ouvrons le fichier
   * Nb: un seul fichier peut être ouvert à la fois
   * le fichier se nomme : journal.csv
   */
  File dataFile = SD.open("journal.csv", FILE_WRITE);

  // si le fichier est disponnible, nous écrivons dedans :
  if (dataFile) {
    Serial.print("journal ouvert");
    dataFile.println(dataString);
    dataFile.close();

    // nous affichons aussi notre chaîne sur le port série :
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  // Si le fichier n'est pas ouvert nous affichons
  // un message d'erreur
  else {
    Serial.println("nous ne pouvons ouvrir journal.csv");

  }
  
  
  rolltime += TENMIN;
   }//fin du if rolltime
}//fin de loop
