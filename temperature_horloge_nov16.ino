/**
 * Lecture de température avec capteur DS18B20
 */

/** 
 *  Nous utilisons la librairie OneWire
 *  Elle doit être présente dans le répertoire libraries
 *  situé dans le répertoire des croquis/sketchs
 *  voir dans le menu Préférences
 *  cf: https://github.com/PaulStoffregen/OneWire/archive/master.zip
 */
#include <OneWire.h>
/**
 * Tiny RTC module horloge
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

  // Initialisation de nos librairies
  sensors.begin();

   Wire.begin();
      RTC.begin();
      if (! RTC.isrunning())
      {
        Serial.println("RTC is NOT running!");
      }
      // Décommenter cette ligne pour mettre à jour l'heure dans le bidule
      RTC.adjust(DateTime(__DATE__, __TIME__));
}
 
 
void loop(void)
{
  /**
   * Nous interrogeons notre capteur
   */
  Serial.print(" Demande de temperature...");
  sensors.requestTemperatures(); // envoi de la demande
  Serial.println("Fait");

  Serial.print("La temperature du premier capteur est : ");
  Serial.println(sensors.getTempCByIndex(0));
  Serial.print("La temperature du deuxieme capteur est : ");
  Serial.println(sensors.getTempCByIndex(1));
  /**
   * 0 correspond au premier capteur du bus
   * nous pouvons en relier plusieurs
   * et pouvons les interroger en changeant cet index
   */
//nous affichons la date et l'heure exacte
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
  //nous attendons ... avant de recommencer la boucle
      delay(1000);
    }

 

