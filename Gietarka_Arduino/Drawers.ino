#define ARRAYSIZE 4
#define SELECTED_COLOR 0x00FF
#define WARNING_COLOR 0xF800
#define EDIT_COLOR 0xFF00

void drawMainMenu(int select, boolean edit, boolean onlythisrow) //edit odpowiada za kolor podswietlenia tekstu, onlythisrow przerysowuje tylko wyselectowany wiersz
{
	if (!onlythisrow)
		tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 10);
	tft.setTextColor(TFT_GREEN);


	for (uint8_t i = 0; i < ARRAYSIZE; i++) {
		tft.setTextSize(3);
		if (i == select)
		{
			if (edit)
				tft.setTextColor(TFT_GREEN, EDIT_COLOR);
			else
				tft.setTextColor(TFT_GREEN, SELECTED_COLOR);
		}
		if (i != select)
		{
			tft.setTextColor(TFT_GREEN);
		}
		if (!onlythisrow)
			tft.println(items[i]);
		else
		{
			if (i == select)
				tft.println(items[i]);
			else
				tft.println("");
		}
		tft.setTextSize(1);
		tft.println();

	}

}
//------------------------------------------------------------------------------------
void drawSettingsMenu(int select, boolean edit, boolean onlythisrow) //edit odpowiada za kolor podswietlenia tekstu, onlythisrow przerysowuje tylko wyselectowany wiersz
{
	if (!onlythisrow)
		tft.fillScreen(TFT_BLACK);

	tft.setCursor(120, 10);
	tft.setTextSize(4);
	tft.setTextColor(TFT_GREEN);
	tft.println("USTAWIENIA");
	tft.setCursor(0, 100);

	for (uint8_t i = 0; i < SETTINGS; i++) {
		tft.setTextSize(2);
		if (i == select)
		{
			if (edit)
				tft.setTextColor(TFT_GREEN, EDIT_COLOR);
			else
				tft.setTextColor(TFT_GREEN, SELECTED_COLOR);
		}
		if (i != select)
		{
			tft.setTextColor(TFT_GREEN);
		}
		if (!onlythisrow)
			tft.println(itemsetting[i]);
		else
		{
			if (i == select)
				tft.println(itemsetting[i]);
			else
				tft.println("");
		}
		tft.setTextSize(1);
		tft.println();

	}

	if (select < SETTINGS)
	{
		tft.drawRoundRect(200, 250, 80, 50, 5, TFT_WHITE);
	}
	else
		tft.fillRoundRect(200, 250, 80, 50, 5, SELECTED_COLOR);

	tft.fillTriangle(240, 285, 240, 265, 220, 275, TFT_WHITE);
	tft.fillRect(240, 270, 20, 10, TFT_WHITE);

}
//------------------------------------------------------------------------------------
void drawSetAngle()
{
	uint8_t move = 0;
	int beforemove;
	items[0] = "1. Kat: " + String(defaultAngle);
	drawMainMenu(0, true, true);
	move = waitforactionY(200);
	do
	{
		beforemove = defaultAngle;
		move = waitforactionY(50);
		if (move == 1 && defaultAngle<180)
			defaultAngle++;
		if (move == 3 && defaultAngle>0)
			defaultAngle--;
		items[0] = "1. Kat: " + String(defaultAngle);
		if (String(defaultAngle).length() != String(beforemove).length())
			drawMainMenu(0, true, false);
		else
			drawMainMenu(0, true, true);

	} while (move != 5);
	EEPROM_writeAnything(EEPROM_LAST_ANGLE, defaultAngle);

	drawMainMenu(0, false, true);
}
//------------------------------------------------------------------------------------
void drawSetTemp()
{
	uint8_t move;
	int wait = 100;
	int beforemove;
	items[1] = "2. Temperatura: " + String(defaultTemp) + " C";
	drawMainMenu(1, true, true);
	move = waitforactionY(200);
	do
	{
		beforemove = defaultTemp;
		move = waitforactionY(wait);
		if (joypos == 0 && move == 1 && defaultTemp<490)// na sztywno ustawiona górna granica temperatury aby nie mozna bylo podwyzszac jej w nieskonczonosc
		{
			defaultTemp = defaultTemp + 10;
		}
		if (joypos != 0 && move == 1 && defaultTemp<500)
		{
			defaultTemp++;
		}
		if (joypos > 680 && move == 3 && defaultTemp>10)
		{
			defaultTemp = defaultTemp - 10;
		}
		if (joypos < 680 && move == 3 && defaultTemp>10)
		{
			defaultTemp--;
		}

		items[1] = "2. Temperatura: " + String(defaultTemp) + " C";
		if (String(defaultTemp).length() != String(beforemove).length())
			drawMainMenu(1, true, false);
		else
			drawMainMenu(1, true, true);

	} while (move != 5);

	EEPROM_writeAnything(EEPROM_LAST_TEMP, defaultTemp);

	drawMainMenu(1, false, true);
}
//------------------------------------------------------------------------------------
void drawWorkView()
{
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setCursor(40, 50);
	tft.setTextColor(TFT_GREEN, TFT_BLACK);
	tft.println("Kat = " + String(defaultAngle));
	tft.drawRoundRect(40, 100, 180, 100, 3, TFT_WHITE);  //K¹t

	tft.setCursor(260, 50);
	tft.println("Temperatura = " + String(defaultTemp));
	tft.drawRoundRect(260, 100, 180, 100, 3, TFT_WHITE);  //Temperatura
	delay(200);  //opoznienie zeby mozna bylo w ogole wejsc w ta opcje bo tak od razu wywalo poniewaz przerywalo ponizej
	drawEndWarm();

	while (licznik1 != 500)
	{
		licznik1++;
		if (licznik1 >= defaultTemp)
			licznik1 = defaultTemp;
		drawAngle();
		drawTemperature();

		showWarning(licznik1);
		if (digitalRead(SW_pin) == LOW)
		{
			drawAnswer(true, 1);
			if (Cancel(1))
			{
				licznik1 = 500;
				break;
			}
			else
			{
				tft.fillScreen(TFT_BLACK);
				tft.setTextSize(2);
				tft.setCursor(40, 50);
				tft.setTextColor(TFT_GREEN, TFT_BLACK);
				tft.println("Kat = " + String(defaultAngle));
				tft.drawRoundRect(40, 100, 180, 100, 3, TFT_WHITE);  //K¹t

				tft.setCursor(260, 50);
				tft.println("Temperatura = " + String(defaultTemp));
				tft.drawRoundRect(260, 100, 180, 100, 3, TFT_WHITE);  //Temperatura
				delay(200);  //opoznienie zeby mozna bylo w ogole wejsc w ta opcje bo tak od razu wywalo poniewaz przerywalo ponizej
				drawEndWarm();
			}
		}

		delay(100);
	}


	while (licznik1 != 500)
	{
		licznik1 = waitforactionY(200);
	}
	licznik1 = 0;
	licznik2 = 0;
	drawMainMenu(2, false, false);
}
//------------------------------------------------------------------------------------
void drawAnswer(boolean firsttime, int index)
{
	if (firsttime)
	{
		tft.fillScreen(TFT_BLACK);
		tft.setTextSize(3);
		tft.setCursor(90, 50);
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.println("Czy napewno chcesz");
		tft.setCursor(90, 80);
		tft.println("zakonczyc grzanie?");
		tft.setCursor(100, 220);
		tft.println("TAK");
		tft.setTextColor(TFT_GREEN, SELECTED_COLOR);
		tft.setCursor(335, 220);
		tft.println("NIE");
	}
	if (index == 0)  //lewo
	{
		tft.setTextSize(3);
		tft.setCursor(100, 220);
		tft.setTextColor(TFT_GREEN, SELECTED_COLOR);
		tft.println(answers[index]);

		tft.setCursor(335, 220);
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.println(answers[index + 1]);

	}
	if (index == 1)  //prawo
	{
		tft.setTextSize(3);
		tft.setCursor(335, 220);
		tft.setTextColor(TFT_GREEN, SELECTED_COLOR);
		tft.println(answers[index]);

		tft.setCursor(100, 220);
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.println(answers[index - 1]);

	}
}
//------------------------------------------------------------------------------------
void drawEndWarm()
{
	tft.setCursor(105, 280);
	tft.setTextSize(3);
	tft.setTextColor(WARNING_COLOR, SELECTED_COLOR);
	tft.println("ZAKONCZ GRZANIE");
}
//------------------------------------------------------------------------------------
void showWarning(int temp)
{
	if (temp<defaultTemp)
	{
		tft.setCursor(120, 250);
		tft.setTextColor(WARNING_COLOR);
		tft.setTextSize(1);
		tft.println("Zaczekaj az grzalka osiagnie temperature");
		tft.setTextColor(TFT_GREEN);
	}
	else
	{
		//tft.fillRect(120,250,300,25,TFT_BLACK);
		tft.setCursor(120, 250);
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.setTextSize(1);
		tft.println("       Grzalka osiagnela temperature       ");

	}
}
//------------------------------------------------------------------------------------
void drawTemperature()
{
	tft.fillRect(265, 105, 170, 90, TFT_BLACK);
	int temp = licznik1;  // = String(getTemperature());
	tft.setCursor(280, 130);
	tft.setTextSize(5);
	if (temp<defaultTemp)
	{
		tft.setTextColor(WARNING_COLOR);
		tft.println(String(temp));
		tft.drawCircle(385, 135, 5, WARNING_COLOR);
		tft.setCursor(395, 130);
		tft.println("C");
	}
	else
	{
		tft.setTextColor(TFT_GREEN);
		tft.println(String(temp));
		tft.drawCircle(385, 135, 5, TFT_GREEN);
		tft.setCursor(395, 130);
		tft.println("C");
	}

}
//------------------------------------------------------------------------------------
void drawAngle()
{
	if (ilosc_impulsow <= 0)
		ilosc_impulsow = 0;
	int angle = ilosc_impulsow;
	tft.fillRect(45, 105, 170, 90, TFT_BLACK);
	tft.setCursor(60, 130);
	tft.setTextColor(TFT_GREEN, TFT_BLACK);
	tft.setTextSize(5);
	tft.println(String(int(angle / 2))); //String(getAngle())
	tft.drawCircle(185, 135, 5, TFT_GREEN);

}