/*
 Name:		Gietarka_Arduino.ino
 Created:	24.10.2018 19:23:46
 Author:	Marcin £ykowski
*/

#include <Wire.h>
#include <math.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Adafruit_MLX90614.h>
#include <User_Setup.h>
#include <TFT_HX8357.h> // Hardware-specific library
#include <EEPROM.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include "EEPROMAnything.h"


#define ARRAYSIZE 4
#define SETTINGS 2 

String items[ARRAYSIZE] = { "1. Kat: ",
"2. Temperatura: ",
"3. Rozpocznij grzanie",
"4. Ustawienia"
};

String itemsetting[SETTINGS] = { 
"1. Kolor tekstu",
"2. O programie"
};

String Colors[4] = {
	"Zielony",
	"Bialy",
	"Pomaranczony",
	"Fioletowy"
};

int ColorsValues[4] = {
	TFT_GREEN,
	TFT_WHITE,
	TFT_ORANGE,
	TFT_PURPLE
};

String answers[2] = { "TAK", "NIE" };

// adresy w pamiêci EEPROM do zapisu i odczytu ostatniej temperatury
int EEPROM_LAST_ANGLE = 0;
int EEPROM_LAST_TEMP = 2;
int EEPROM_LAST_COLOR = 5;

int licznik1;
int licznik2;

// kierunek wychylenia ga³ki joysticka
uint8_t dirct;
int joypos;
uint8_t index;

// zadany kat i temperatura
int defaultTemp;
int defaultAngle;

// kolor tekstu
int GLOBAL_TEXT_COLOR;

const uint8_t SW_pin = 6; // digital pin connected to switch output
const uint8_t Y_pin = A0; // A0 y - góra/dó³
const uint8_t X_pin = A1; // A1 x - prawo/lewo

/* do usuniecia po obsludze pomiaru kata*/
unsigned long time = 0;
long ilosc_impulsow = 0;
long licznik = 0;

TFT_HX8357 tft = TFT_HX8357();
Adafruit_MLX90614 mlx = Adafruit_MLX90614();



//-------------- zyroskop

MPU6050 accelerometer;

const float pi = 3.141592;
const int sample_no = 100; // liczba probek potrzebnych do jednego pomiaru kata
int16_t ax, ay, az;
float x, y, z;
int  sample;
float _angle_x, angle_x, _angle_y, angle_y;
long ax_sum, ay_sum, az_sum;




void setup() {
	Wire.begin();
	Serial.begin(38400);
	accelerometer.initialize();

	tft.begin();
	tft.setRotation(3);
	tft.setTextFont(1);

	mlx.begin();

	joypos = 1;
	index = 0;
	
	int index;
	EEPROM_readAnything(EEPROM_LAST_COLOR, index);
	GLOBAL_TEXT_COLOR = ColorsValues[index];

	// odczyt z EEPROMU ostatniej ustawionej temperatury i kata
	EEPROM_readAnything(EEPROM_LAST_TEMP, defaultTemp);
	EEPROM_readAnything(EEPROM_LAST_ANGLE, defaultAngle);

	pinMode(X_pin, INPUT);
	pinMode(Y_pin, INPUT);
	pinMode(SW_pin, INPUT);
	digitalWrite(SW_pin, HIGH);
	items[0] = items[0] + String(defaultAngle);
	items[1] = items[1] + String(defaultTemp) + " C";
}


void loop(void) {

	drawMainMenu(0, false, false);

	while (true)
	{
		chooseOption();
	}
}
