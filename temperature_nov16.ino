
/**
 * Lecture de température avec un ou plusieurs
 * capteurs DS18B20
 */

/** 
 *  Nous utilisons la librairie OneWire
 *  Elle doit être présente dans le répertoire libraries
 *  situé dans le répertoire des croquis/sketchs
 *  voir dans le menu Préférences
 *  cf: https://github.com/PaulStoffregen/OneWire/archive/master.zip
 */
#include <OneWire.h>
#include <Wire.h>

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

  //nous attendons ... avant de recommencer la boucle
      delay(1000);
    }

 

