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
#include <TFT_HX8357.h> // Bibilioteka ekranu LCD
#include <EEPROM.h>
#include "EEPROMAnything.h"


// Definicja elementów g³ównego menu
#define MenuItemsCount 5
String items[MenuItemsCount] = { "1. Kat: ",
"2. Temperatura: ",
"3. Wybierz profil",
"4. Rozpocznij grzanie",
"5. Ustawienia"
};

// Definicja elementów menu ustawienieñ
#define SETTINGS 2 
String itemsSetting[SETTINGS] = { 
"1. Kolor tekstu",
"2. O programie"
};

// Definicja dostêpnych kolorów wyœwietlania tekstu
String Colors[4] = {
	"Zielony",
	"Bialy",
	"\n     Pomaranczowy",
	"Fioletowy"
};

int ColorsCount = 4;

// Definicja wartoœci kolorów wyœwietlania tekstu
int ColorsValues[4] = {
	TFT_GREEN,
	TFT_WHITE,
	TFT_ORANGE,
	TFT_PURPLE
};

// Profile temperatury i kata
struct Profiles
{
	String name;
	int angleValue;
	int tempValue;
};

#define Profile1 {"PMMA ", 45, 200}
#define Profile2 {"PC   ", 60, 150}
#define Profile3 {"PS   ", 90, 250}
#define Profile4 {"PETG ", 30, 300}
#define Profile5 {"PVC  ", 40, 380}

#define ProfilesCount 5

Profiles ProfilesValues[ProfilesCount] = {
	Profile1,
	Profile2,
	Profile3,
	Profile4,
	Profile5
};

// Definicja elementów wyboru odpowiedzi w przypadku przerwania grzania
String answers[2] = { "TAK", "NIE" };

// adresy w pamiêci EEPROM do zapisu i odczytu ustawieñ
int EEPROM_LAST_ANGLE = 0;
int EEPROM_LAST_TEMP = 2;
int EEPROM_LAST_COLOR = 5;

// stale dotyczace zakresu temperatury i kata
#define MAX_ANGLE 150
#define MIN_ANGLE 1
#define MAX_TEMP 380
#define MIN_TEMP 30

// czas po którym grzalka zostanie wylaczona jesli program wykryje ze uzytkownik nie wykonuje zadnych czynnosci
const uint16_t TIME_TO_STOP_HEAT = 3000;
uint16_t timeElapsed;

// kierunek wychylenia ga³ki joysticka
uint8_t dirct;
int joypos;
uint8_t joyIndex;

// zadany kat i temperatura
int defaultTemp;
int defaultAngle;

// kolor tekstu
int GLOBAL_TEXT_COLOR;

// joystick
const uint8_t SW_pin = 6; // digital pin connected to switch output
const uint8_t Y_pin = A0; // A0 y - góra/dó³
const uint8_t X_pin = A1; // A1 x - prawo/lewo

struct joyctickMove {
	int direction; // 1 - góra, 3 - dó³, 2 - prawo, 4 - lewo
	int turn;
};

// Ekran LCD
TFT_HX8357 tft = TFT_HX8357();

// Czujnik temperatury
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// ¯yroskop
MPU6050 accelerometer;

// drugi sposob pomiaru kata - usunac w finalnej wersji
//const float alpha = 0.5;
//
//double fXg = 0;
//double fYg = 0;
//double fZg = 0;


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
	joyIndex = 0;
	
	timeElapsed = 0;

	int colorIndex;
	// odczyt z EEPROMU ostatniego ustawionego koloru
	EEPROM_readAnything(EEPROM_LAST_COLOR, colorIndex);
	GLOBAL_TEXT_COLOR = ColorsValues[colorIndex];

	// odczyt z EEPROMU ostatniej ustawionej temperatury i kata
	EEPROM_readAnything(EEPROM_LAST_TEMP, defaultTemp);
	EEPROM_readAnything(EEPROM_LAST_ANGLE, defaultAngle);

	pinMode(X_pin, INPUT);
	pinMode(Y_pin, INPUT);
	pinMode(SW_pin, INPUT);
	digitalWrite(SW_pin, HIGH);
	items[0] = items[0] + String(defaultAngle) + " ";
	items[1] = items[1] + String(defaultTemp) + " C";
	itemsSetting[0] = itemsSetting[0] + ": " + Colors[colorIndex];
}


void loop(void) {

	drawMainMenu(0, false, false, true);

	while (true)
	{
		chooseOption();
	}
}
