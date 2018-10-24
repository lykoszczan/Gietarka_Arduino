/*
 Name:		Gietarka_Arduino.ino
 Created:	24.10.2018 19:23:46
 Author:	Marcin £ykowski
*/

#include <User_Setup.h>
#include <TFT_HX8357.h> // Hardware-specific library
#include <EEPROM.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include "EEPROMAnything.h"

TFT_HX8357 tft = TFT_HX8357();

#define ARRAYSIZE 4
String items[ARRAYSIZE] = { "1. Kat: ",
"2. Temperatura: ",
"3. Rozpocznij grzanie",
"4. Ustawienia"
};
#define SETTINGS 4
String itemsetting[SETTINGS] = { "1. Domyslny kat: ",
"2. Domyslna temperatura: ",
"3. Kolory",
"4. O programie"
};

String answers[2] = { "TAK", "NIE" };

// adresy w pamiêci EEPROM do zapisu i odczytu ostatniej temperatury
int EEPROM_LAST_ANGLE = 0;
int EEPROM_LAST_TEMP = 2;


int licznik1;
int licznik2;

// kierunek wychylenia ga³ki joysticka
uint8_t dirct;
int joypos;
uint8_t index;

int defaultTemp;
int defaultAngle;

const uint8_t SW_pin = 6; // digital pin connected to switch output
const uint8_t Y_pin = A0; // A0 y - góra/dó³
const uint8_t X_pin = A1; // A1 x - prawo/lewo
const uint8_t A_pin = 19; // enkoder
const uint8_t B_pin = 18; // enkoder


unsigned long time = 0;
long ilosc_impulsow = 0;
long licznik = 0;

void setup() {
	//Serial.begin(9600);
	tft.begin();
	tft.setRotation(3);

	tft.setTextFont(1);
	joypos = 1;
	index = 0;
	// odczyt z EEPROMU ostatniej ustawionej temperatury i kata
	EEPROM_readAnything(EEPROM_LAST_TEMP, defaultTemp);
	EEPROM_readAnything(EEPROM_LAST_ANGLE, defaultAngle);

	pinMode(X_pin, INPUT);
	pinMode(Y_pin, INPUT);
	pinMode(SW_pin, INPUT);
	digitalWrite(SW_pin, HIGH);
	items[0] = items[0] + String(defaultAngle);
	items[1] = items[1] + String(defaultTemp) + " C";

	pinMode(A_pin, INPUT);
	pinMode(B_pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(19), angleMinus, HIGH);
	attachInterrupt(digitalPinToInterrupt(18), anglePlus, HIGH);

	time = millis();
}


void loop(void) {

	drawMainMenu(0, false, false);

	while (true)
	{
		chooseOption();
	}


	//  while (licznik != ilosc_impulsow)
	//  {
	//    licznik = ilosc_impulsow;
	//    tft.fillScreen(TFT_GREEN);
	//    tft.setCursor(100, 100);
	//    tft.setTextSize(4);
	//    tft.println(String(licznik/2.5));
	//  }
}

void anglePlus()
{
	if ((millis() - time) > 1)
		ilosc_impulsow++;
	time = millis();
}

void angleMinus()
{
	if ((millis() - time) > 1)
		ilosc_impulsow--;
	time = millis();
}