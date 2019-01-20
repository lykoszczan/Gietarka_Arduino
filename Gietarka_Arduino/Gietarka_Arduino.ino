/*
 Name:		Gietarka_Arduino.ino
 Created:	24.10.2018 19:23:46
 Author:	Marcin �ykowski
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


// Definicja element�w g��wnego menu
#define MenuItemsCount 5
String items[MenuItemsCount] = { "1. Kat: ",
"2. Temperatura: ",
"3. Wybierz profil",
"4. Rozpocznij grzanie",
"5. Ustawienia"
};

// Definicja element�w menu ustawienie�
#define SETTINGS 4 

String itemsSetting[SETTINGS] = { 
"1. Kalibracja kata",
"2. Nastawy regulatora",
"3. Kolor tekstu",
"4. O programie"
};

// kolor tekstu
int GLOBAL_TEXT_COLOR;

// wlasne kolory
#define SELECTED_COLOR 0x00FF
#define WARNING_COLOR 0xF800
#define EDIT_COLOR 0xBD00

// Definicja dost�pnych kolor�w wy�wietlania tekstu
String Colors[4] = {
	"Zielony",
	"Bialy",
	"\n     Pomaranczowy",
	"Fioletowy"
};

int ColorsCount = 4;

// Definicja warto�ci kolor�w wy�wietlania tekstu
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

#define Profile1 {"PMMA ", 45, 100}
#define Profile2 {"PC   ", 60, 150}
#define Profile3 {"PS   ", 90, 120}
#define Profile4 {"PETG ", 30, 130}
#define Profile5 {"PVC  ", 40, 110}

#define ProfilesCount 5

Profiles ProfilesValues[ProfilesCount] = {
	Profile1,
	Profile2,
	Profile3,
	Profile4,
	Profile5
};

// Definicja element�w wyboru odpowiedzi w wymaganej interakcji uzytkownika
String answers[2] = { "TAK", "NIE" };

struct ViewStyle {
	int textColor;
	int BackgroundColor;
};

//styl dla widoku zatrzymania grzania
#define Style1 {TFT_BLACK,TFT_RED}

ViewStyle Styles[1] = {
	Style1
};


// adresy w pami�ci EEPROM do zapisu i odczytu ustawie�
int EEPROM_LAST_ANGLE = 0;
int EEPROM_LAST_TEMP = 2;
int EEPROM_LAST_COLOR = 5;
int EEPROM_LAST_OFFSET = 8;
int EEPROM_PID_KP = 15;
int EEPROM_PID_KI = 17;
int EEPROM_PID_KD = 19;

// stale dotyczace zakresu temperatury i kata
#define MAX_ANGLE 150
#define MIN_ANGLE 1
#define MAX_TEMP 160
#define MIN_TEMP 30
#define MAX_TABLE_TEMP 50

// czas [ms] po kt�rym grzalka zostanie wylaczona jesli program wykryje ze uzytkownik nie wykonuje zadnych czynnosci
const unsigned long TIME_TO_STOP_HEAT = 300000;
unsigned long timeElapsed;

// kierunek wychylenia ga�ki joysticka
uint8_t dirct;
int joypos;
uint8_t joyIndex;

// zadany kat i temperatura
int tempSetPoint;
int angleSetPoint;

// do obs�ugi czas�w nagrzania drutu i materia�u
long heatTimeWire;
long heatTimeObj;
bool isHeatStartWire;
bool isHeatStartObj;

// czy warto�ci zosta�y osi�gni�te - je�li ta zmienna jest true to wtedy grzalka jest wylaczona
bool targetValuesReached;

// Slide MOSFET MP
const uint8_t MOSFET_PIN = 42;

// joystick
const uint8_t SW_pin = A2; // A2 - ustawione jako cyfrowy pin
const uint8_t Y_pin = A1; // A1 y - g�ra/d�
const uint8_t X_pin = A0; // A0 x - prawo/lewo

// zakresy wychylenia drazka  //przedzialy dla 5V
#define MIDDLE_MIN_X 200	//minimalna wartosc w srodkowym polozeniu
#define MIDDLE_MIN_Y 200	//minimalna wartosc w srodkowym polozeniu
#define MIDDLE_MAX_X 800    //maksymalna wartosc w srodkowym polozeniu
#define MIDDLE_MAX_Y 800    //maksymalna wartosc w srodkowym polozeniu

// dla 3.3V
// przedzialy X: 362 - �rodek, 687 - prawo, 0 - lewo 
// przedzia�y Y: 332 - �rodek, 687 - d�, 0 - g�ra

// Ekran LCD
TFT_HX8357 tft = TFT_HX8357();

// W przyszlosci gdyby ktos chcial zmienic czujnik to domyslnym adresem jest 0x5a, aby zmienic adres czujnika nalezy uzyc funkcji MLX90614_Set_Adress dostepnej w bibliotece SparkFunMLX90614

// Czujnik temperatury drutu
Adafruit_MLX90614 mlxWire = Adafruit_MLX90614(0x5b);
// Czujnik temperatury materialu
Adafruit_MLX90614 mlxObj = Adafruit_MLX90614(0x5a);

// czy zosta�y ustawione warto�ci
bool isTempSet;
bool isAngleSet;

// �yroskop
MPU6050 accelerometer;
int ZaxisOffset;

// Regulator PID
struct PID_Settings {
	String name;
	int value;
};

#define Kp {"Kp",0}
#define Ki {"Ki",0}
#define Kd {"Kd",0}

PID_Settings PID_Items[3]{
	Kp,
	Ki,
	Kd
};

// rysowanie okregu
struct Point {
	int x;
	int y;
};

const int zeroPointX = 240;
const int zeroPointY = 220;
const int radius = 40;
Point degreesTab[36];
int beta = -90;
int pointColor;

//PID
unsigned long PID_time;
float PID_error = 0;
float previous_error = 0;
float PID_timeElapsed, PID_timePrev;
int PID_value = 0;
int PID_p = 0;    int PID_i = 0;    int PID_d = 0;

void setup() {
	Wire.begin();
	Serial.begin(9600);

	accelerometer.initialize();

	tft.begin();
	tft.setRotation(3);
	tft.setTextFont(1);

	mlxWire.begin();
	mlxObj.begin();

	joypos = 1;
	joyIndex = 0;
	
	timeElapsed = 0;

	isTempSet = false;
	isAngleSet = false;

	int colorIndex;
	// odczyt z EEPROMU ostatniego ustawionego koloru
	EEPROM_readAnything(EEPROM_LAST_COLOR, colorIndex);
	GLOBAL_TEXT_COLOR = ColorsValues[colorIndex];

	// odczyt z EEPROMU ostatniej ustawionej temperatury i kata
	EEPROM_readAnything(EEPROM_LAST_TEMP, tempSetPoint);
	EEPROM_readAnything(EEPROM_LAST_ANGLE, angleSetPoint);

	// odczyt z EEPROMU ostatniego ustawionego offsetu akcelerometru
	EEPROM_readAnything(EEPROM_LAST_OFFSET, ZaxisOffset);

	// odczyt z EEPROMU ostatnich nastaw�w regulatora
	EEPROM_readAnything(EEPROM_PID_KP, PID_Items[0].value);
	EEPROM_readAnything(EEPROM_PID_KI, PID_Items[1].value);
	EEPROM_readAnything(EEPROM_PID_KD, PID_Items[2].value);

	pinMode(X_pin, INPUT);
	pinMode(Y_pin, INPUT);
	pinMode(SW_pin, INPUT);
	digitalWrite(SW_pin, HIGH);

	digitalWrite(MOSFET_PIN, LOW);

	items[0] = items[0] + String(angleSetPoint) + " ";
	items[1] = items[1] + String(tempSetPoint) + " C";
	itemsSetting[2] = itemsSetting[2] + ": " + Colors[colorIndex];

	// zebranie punktow na okregu, raz to zrobimy na starcie programu to potem juz nie bedzie trzeba tego robic
	for (int i = 0; i < 36; i++)
	{
		degreesTab[i].x = zeroPointX + radius * cos(beta*(PI / 180.0));
		degreesTab[i].y = zeroPointY + radius * sin(beta*(PI / 180.0));
		beta = beta + 10;
	}

	PID_time = millis();
}


void loop(void) {

	drawMainMenu(0, false, false, true);

	while (true)
	{
		chooseOption();
	}
}
