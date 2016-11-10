
//nous créons un compteur pour lire les valeurs toutes les 10 minutes
#define TENMIN (1000UL * 60 * 10);
unsigned long rolltime = millis() + TENMIN;


/** 
 *  Nous utilisons la librairie OneWire
 *  Elle doit être présente dans le répertoire libraries
 *  situé dans le répertoire des croquis/sketchs
 *  voir dans le menu Préférences
 *  cf: https://github.com/PaulStoffregen/OneWire/archive/master.zip
 */
#include <OneWire.h>


/**
 * nous utilisons Tiny RTC module horloge
  * DS1307 pour bus I2C
 * avec batterie au lithium CR1225
 * Le port I2C de l'Arduino est situé
 * sur les pin A4 et A5
 *
 * Analog pin A5 <-> SCL
 * Analog pin A4 <-> SDA
 */
// nous utilisons la librairie RTClib
//pour lire les données de l'horloge
//cf: https://github.com/adafruit/RTClib
 #include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

// nous utilisons la librairie RTClib
//cf: https://github.com/adafruit/RTClib
 #include <Wire.h>
#include "RTClib.h"


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

// Arduino Uno pin 4
// cf: https://www.arduino.cc/en/Reference/SPI
const int chipSelect = 4;


/**
 * Lecture de température avec un seul
 * capteur DS18B20 
 */
/**
 * Nous utilisons aussi la librairie DallasTemperature
 * cf: https://github.com/milesburton/Arduino-Temperature-Control-Library
 */
#include <DallasTemperature.h>
 
// Le fil des données jaune est relié au pin 2 du Arduino
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
      // Décommenter cette ligne pour mettre à jour l'heure dans RTC
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
    // ne fait rien d'autre
    return;
    }
  Serial.println("Carte OK");
}
 
 
void loop(void)
{
   if((long)(millis() - rolltime) >= 0) 
   //si il s'est écoulé 10 minutes depuis la dernière demande de température
{
  DateTime now = RTC.now();
  /**
   * Nous interrogeons notre capteur
   */
  sensors.requestTemperatures(); // envoi de la demande successivement aux différents capteurs reliés ici index 0 et 1 
 /* on pourrait l'afficher avec :
  Serial.print("La temperature du premier capteur est : ");
  Serial.println(sensors.getTempCByIndex(0));
  Serial.print("La temperature du deuxieme capteur est : ");
  Serial.println(sensors.getTempCByIndex(1)); 
  */
     

// on enregistre sur SD année; mois; jour; heure; minute; seconde; T1; T2
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
  
 
  /**
   * nous ouvrons le fichier
   * Nb: un seul fichier peut être ouvert à la fois
   * le fichier se nomme : journal.csv
   */
  File dataFile = SD.open("journal.csv", FILE_WRITE);

  // si le fichier est disponible, nous écrivons dedans :
  if (dataFile) {
    Serial.print("journal ouvert");
    dataFile.println(dataString);
    dataFile.close();

    // nous affichons aussi notre chaîne sur le port série :
    Serial.println(dataString);
  }
 
  // Si le fichier n'est pas ouvert nous affichons
  // un message d'erreur
  else {
    Serial.println("nous ne pouvons ouvrir journal.csv");
  }
  
  //nous ajoutons 10 minutes à rolltime pour que arduino interroge à nouveau la température au bout de 10 minutes
  rolltime += TENMIN;
   }//fin du if rolltime
}//fin de loop
