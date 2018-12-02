void drawMainMenu(int select, boolean edit, boolean onlythisrow, bool isNewWindow) //edit odpowiada za kolor podswietlenia tekstu, onlythisrow przerysowuje tylko wyselectowany wiersz
{
	int textColor;

	if (isNewWindow)
		tft.fillScreen(TFT_BLACK);

	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR);
	tft.setCursor(190, 10);
	tft.println("MENU");
	tft.setCursor(0, 70);

	for (uint8_t i = 0; i < MenuItemsCount; i++)
	{
		if (!isAngleSet && i == 0)
		{
			textColor = WARNING_COLOR;
		}
		else if (!isTempSet && i == 1)
		{
			textColor = WARNING_COLOR;
		}
		else
		{
			textColor = GLOBAL_TEXT_COLOR;
		}
		tft.setTextSize(3);
		if (i == select)
		{
			if (edit)
				tft.setTextColor(textColor, EDIT_COLOR);
			else
				tft.setTextColor(textColor, SELECTED_COLOR);
		}
		if (i != select)
		{
			tft.setTextColor(textColor,TFT_BLACK);
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

		drawDegreeSymbol(i, textColor);
	}
	if(onlythisrow || isNewWindow)
	  drawSetHint();

}

void drawSetHint() // funkcja rysujaca podpowiedz zeby ustawic wartosci przed rozpoczeciem grzania, specjalnie dla Skalskiego
{
	tft.setTextSize(4);
	tft.setTextColor(WARNING_COLOR, TFT_BLACK);
	tft.fillRect(0, 270, 480, 50, TFT_BLACK);
	if (!isAngleSet && !isTempSet)
	{
		tft.setCursor(70, 280);
		tft.println("USTAW WARTOSCI");
	}
	else if (!isAngleSet)
	{
		tft.setCursor(140, 280);
		tft.println("USTAW KAT");
	}
	else if (!isTempSet)
	{
		tft.setCursor(30, 280);
		tft.println("USTAW TEMPERATURE");
	}

}

void drawSetHintBind() 
{
	tft.setTextSize(4);
	tft.setTextColor(WARNING_COLOR, TFT_BLACK);
	for (int i = 0; i < 6; i++)
	{
		delay(50);
		tft.fillRect(0, 270, 480, 50, TFT_BLACK);
		delay(50);
		if (!isAngleSet && !isTempSet)
		{
			tft.setCursor(70, 280);
			tft.println("USTAW WARTOSCI");
		}
		else if (!isAngleSet)
		{
			tft.setCursor(140, 280);
			tft.println("USTAW KAT");
		}
		else if (!isTempSet)
		{
			tft.setCursor(30, 280);
			tft.println("USTAW TEMPERATURE");
		}
	}
}

void drawCheckHeater()
{
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
	tft.setCursor(35, 100);
	tft.println("Sprawdzam grzalke");
	//drawLoadGifERROR();
	drawLoadGifOK();
}

void drawLoadGifERROR()
{
	for (int i = 0; i < 36; i++)
	{
		tft.drawPixel(degreesTab[i].x, degreesTab[i].y, TFT_RED);
		delay(50);
	}
	delay(100);

	tft.fillScreen(TFT_RED);
	tft.setTextSize(3);
	tft.setCursor(15, 50);
	tft.setTextColor(TFT_BLACK);
	tft.println("Brak zasilania na grzalce");
	tft.setTextColor(TFT_BLACK, SELECTED_COLOR);
	tft.setCursor(zeroPointX - 20, zeroPointY - 15);
	tft.setTextSize(4);
	tft.println("OK");

	int waitForMove = waitforactionXY(50);
}

void drawLoadGifOK()
{
	for (int i = 0; i < 36; i++)
	{
		tft.drawPixel(degreesTab[i].x, degreesTab[i].y, TFT_RED);
		delay(20);
	}
	delay(100);
	for (int i = 0; i < 36; i++)
	{
		tft.drawPixel(degreesTab[i].x, degreesTab[i].y, TFT_GREEN);
	}
	tft.setTextColor(TFT_GREEN);
	tft.setTextSize(4);
	tft.setCursor(zeroPointX - 20, zeroPointY - 15);
	tft.println("OK");
	delay(500);
}

void drawDegreeSymbol(int menuIndex, int color)
{
	int firstPixel;
	int topPixel;
	const int space = 20; // szerokosc jednej cyfry w pikselach dla rozmiaru czcionki = 3
	int position;
	if (menuIndex == 0 || menuIndex == 1)
	{
		if (menuIndex == 0)
		{
			firstPixel = 170;
			topPixel = 75;
			position = space * (String(defaultAngle).length() - 1) + firstPixel;
		}
		if (menuIndex == 1)
		{
			firstPixel = 312;
			topPixel = 107;
			position = space * (String(defaultTemp).length() - 1) + firstPixel;
		}
		tft.drawCircle(position, topPixel, 5, color);
	}
}

void drawSettingsMenu(int select, boolean edit, boolean onlythisrow, bool isNewWindow) //edit odpowiada za kolor podswietlenia tekstu, onlythisrow przerysowuje tylko wyselectowany wiersz
{
	if (isNewWindow)
		tft.fillScreen(TFT_BLACK);

	tft.setCursor(120, 10);
	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR);
	tft.println("USTAWIENIA");
	tft.setCursor(0, 100);

	for (uint8_t i = 0; i < SETTINGS; i++) {
		tft.setTextSize(3);
		if (i == select)
		{
			if (edit)
			  tft.setTextColor(GLOBAL_TEXT_COLOR, EDIT_COLOR);
			else
			  tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		}
		if (i != select)
		{
			tft.setTextColor(GLOBAL_TEXT_COLOR,TFT_BLACK);
		}
		if (!onlythisrow)
			tft.println(itemsSetting[i]);
		else
		{
			if (i == select)
			  tft.println(itemsSetting[i]);
			else
			  tft.println("");
		}
		tft.setTextSize(1);
		tft.println();

	}

	if (select < SETTINGS)
	{
		tft.fillRoundRect(200, 250, 80, 50, 5, TFT_BLACK);
		tft.drawRoundRect(200, 250, 80, 50, 5, TFT_WHITE);
	}
	else
		tft.fillRoundRect(200, 250, 80, 50, 5, SELECTED_COLOR);

	tft.fillTriangle(240, 285, 240, 265, 220, 275, TFT_WHITE);
	tft.fillRect(240, 270, 20, 10, TFT_WHITE);

}

void drawSetAngle()
{
	uint8_t move = 0;
	int beforemove;

	isAngleSet = true;
	items[0] = "1. Kat: " + String(defaultAngle) + " ";
	drawMainMenu(0, true, true, false);
	move = waitforactionXY(200);
	do
	{
		beforemove = defaultAngle;
		move = waitforactionXY(50);
		if (move == 1 && defaultAngle<MAX_ANGLE)
			defaultAngle++;
		if (move == 3 && defaultAngle>MIN_ANGLE)
			defaultAngle--;
		items[0] = "1. Kat: " + String(defaultAngle) + " ";
		if (String(defaultAngle).length() != String(beforemove).length())
			drawMainMenu(0, true, false, true);
		else
			drawMainMenu(0, true, true, false);

	} while (move != 5 && move != 4);
	EEPROM_writeAnything(EEPROM_LAST_ANGLE, defaultAngle);

	drawMainMenu(0, false, true, false);
}

void drawSetTemp()
{
	uint8_t move;
	int wait = 30;
	int beforemove;

	isTempSet = true;
	items[1] = "2. Temperatura: " + String(defaultTemp) + " C";
	drawMainMenu(1, true, true, false);
	move = waitforactionXY(200);
	do
	{
		beforemove = defaultTemp;
		move = waitforactionXY(wait);
		if (move == 1 && defaultTemp<MAX_TEMP)
		{
			defaultTemp++;
		}
		if (move == 3 && defaultTemp>MIN_TEMP)
		{
			defaultTemp--;
		}

		items[1] = "2. Temperatura: " + String(defaultTemp) + " C";
		if (String(defaultTemp).length() != String(beforemove).length())
			drawMainMenu(1, true, false, true);
		else
			drawMainMenu(1, true, true, false);

	} while (move != 5 && move != 4);

	EEPROM_writeAnything(EEPROM_LAST_TEMP, defaultTemp);

	drawMainMenu(1, false, true, false);
}

void drawWorkView()
{
	bool isCanceled = false;
	double currentTemp, currentAngle;
	double lastAngle;
	int moveHorz, moveVert;
	long timeToAprox;

	refreshWorkView();
	currentAngle = 0;
	timeToAprox = millis();
	while (true)
	{
		// probkowanie co 10 sek czy zmienil sie kat od ostatniego pobranego, jesli nie to po okreslonym czasie bezczynnosc
		if ((millis() - timeToAprox) > 10000)
		{
			lastAngle = currentAngle;
			timeToAprox = millis();
		}

		currentTemp = mlx.readObjectTempC();
		currentAngle = getAngle();

		drawAngle();
		drawTemperature();
		//RunPIDController

		showWarning(currentTemp);

		/*  po zakonczneniu grzania material musi ostygnac, ewentualnie w momencie rozpoczecia giecia wylaczyc grzalke - do ustalenia
		if (currentTemp >= defaultTemp && currentAngle >= defaultAngle)
		{
		//StopPIDController
		//break
		}
		*/


		// jezeli uzytkownik nie wykona zadnej czynnosci przez ustalony z gory czas. wylaczamy grzalke
		// ----- w celach bezpieczenstwa
		moveHorz = analogRead(X_pin);
		moveVert = analogRead(Y_pin);
		if (moveVert < MIDDLE_MIN_Y || moveVert > MIDDLE_MAX_Y || moveHorz < MIDDLE_MIN_X || moveHorz > MIDDLE_MAX_X)
		{
			timeElapsed = millis();
		}
		if ((lastAngle > currentAngle + 5) || (lastAngle < currentAngle - 5))
		{
			timeElapsed = millis();
		}
		// -----

		if (digitalRead(SW_pin) == LOW || (moveHorz < MIDDLE_MIN_X)) // czy jest wcisniety analog lub wychylony w lewo
		{
			drawAnswer(true, 1, 0);
			if (getYesNoAwser(1,0))
			{
				isCanceled = true;
				break;
			}
			else
			{
				timeElapsed = millis();
				refreshWorkView();
			}
		}
		if ((millis() - timeElapsed) > TIME_TO_STOP_HEAT)
		{
			//StopPIDController;
			drawStopHeat();
			if (getYesNoAwser(1,1))
			{
				timeElapsed = millis();
				refreshWorkView();
			}
			else
			{				
				isCanceled = true;
				break;
			}
		}
	}

	while (!isCanceled)
	{
		int move = waitforactionY(200);
		break;
	}

	drawMainMenu(3, false, false, true);
}

void refreshWorkView()
{
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setCursor(40, 50);
	tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
	tft.println("Kat = " + String(defaultAngle));
	tft.drawRoundRect(40, 100, 180, 100, 3, TFT_WHITE);  //K¹t

	tft.setCursor(260, 50);
	tft.println("Temperatura = " + String(defaultTemp));
	tft.drawRoundRect(260, 100, 180, 100, 3, TFT_WHITE);  //Temperatura
	delay(200);  //opoznienie zeby mozna bylo w ogole wejsc w ta opcje bo tak od razu wywalo poniewaz przerywalo ponizej
	drawEndWarm();
}

void drawStopHeat()
{
	tft.fillScreen(TFT_RED);
	tft.setTextSize(3);
	tft.setCursor(5, 50);
	tft.setTextColor(TFT_BLACK, TFT_BLACK);
	tft.println("Urzadzenie jest nieuzywane \n Grzalka zostala wylaczona  \n    Rozpoczac grzanie?");
	tft.setCursor(100, 220);
	tft.println("TAK");
	tft.setTextColor(TFT_BLACK, SELECTED_COLOR);
	tft.setCursor(335, 220);
	tft.println("NIE");
}

void drawAnswer(boolean firsttime, int index, int StyleIndex)
{
	if (firsttime)
	{
		tft.fillScreen(TFT_BLACK);
		tft.setTextSize(3);
		tft.setCursor(90, 50);
		tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println("Czy napewno chcesz");
		tft.setCursor(90, 80);
		tft.println("zakonczyc grzanie?");
		tft.setCursor(100, 220);
		tft.println("TAK");
		tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		tft.setCursor(335, 220);
		tft.println("NIE");
	}
	if (index == 0)  //lewo
	{
		tft.setTextSize(3);
		tft.setCursor(100, 220);
		if(StyleIndex == 1)
		  tft.setTextColor(Styles[0].textColor, SELECTED_COLOR);
		else
		  tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		tft.println(answers[index]);

		tft.setCursor(335, 220);
		if (StyleIndex == 1)
		  tft.setTextColor(Styles[0].textColor, Styles[0].BackgroundColor);
		else
		  tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println(answers[index + 1]);

	}
	if (index == 1)  //prawo
	{
		tft.setTextSize(3);
		tft.setCursor(335, 220);
		if (StyleIndex == 1)
			tft.setTextColor(Styles[0].textColor, SELECTED_COLOR);
		else
			tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		tft.println(answers[index]);

		tft.setCursor(100, 220);
		if (StyleIndex == 1)
			tft.setTextColor(Styles[0].textColor, Styles[0].BackgroundColor);
		else
			tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println(answers[index - 1]);

	}
}

void drawEndWarm()
{
	tft.setCursor(105, 280);
	tft.setTextSize(3);
	tft.setTextColor(WARNING_COLOR, SELECTED_COLOR);
	tft.println("ZAKONCZ GRZANIE");
}

void showWarning(double temp)
{
	if (temp<defaultTemp)
	{
		tft.setCursor(120, 250);
		tft.setTextColor(WARNING_COLOR, TFT_BLACK);
		tft.setTextSize(1);
		tft.println("Zaczekaj az grzalka osiagnie temperature");
		tft.setTextColor(TFT_GREEN);
	}
	else
	{
		tft.setCursor(120, 250);
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.setTextSize(1);
		tft.println("       Grzalka osiagnela temperature       ");

	}
}

void drawTemperature()
{
	double temp = mlx.readObjectTempC(); 
	String outText = String(temp);
	outText = outText.substring(0, 5);
	tft.setCursor(270, 130);
	tft.setTextSize(4);
	if (temp<defaultTemp)
	{
		tft.setTextColor(WARNING_COLOR, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(400, 135, 5, WARNING_COLOR);
		tft.setCursor(410, 130);
		tft.println("C");
	}
	else
	{
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(400, 135, 5, TFT_GREEN);
		tft.setCursor(410, 130);
		tft.println("C");
	}


}

void drawAngle()
{
	double currentAngle = abs(getAngle());
	String outText = String(abs(currentAngle));
    
	if (outText.length() < 5)
	{
		outText = outText + "0";
	}

	outText = outText.substring(0, 5);

	tft.setCursor(60, 130);
	tft.setTextSize(4);
	if (currentAngle<defaultAngle)
	{
		tft.setTextColor(WARNING_COLOR, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(185, 135, 5, WARNING_COLOR);
	}
	else
	{
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(185, 135, 5, TFT_GREEN);
	}

}

void drawAbout()
{
	tft.fillScreen(TFT_BLACK);
	String about = " Politechnika Warszawska \n\n      Sterownik zespolu grzewczego \n   stanowiska do wyginania materialow \n            termoplastycznych \n\n          Marcin Lykowski 2018 \n          lykoszczan@gmail.com\n              Wersja 1.0.0";
	tft.setCursor(20, 100);
	tft.setTextSize(2);
	tft.setTextColor(GLOBAL_TEXT_COLOR);
	for (int i = 0; i < about.length(); i++)
	{
		if (i < 24)
			tft.setTextSize(3);
		else
			tft.setTextSize(2);
		tft.print(about[i]);
		if (about[i] != ' ')
			delay(40);
		else
			delay(5);
	}
	delay(3000);
}

void drawSetColor()
{
	uint8_t move = 0;
	int index = 0;
	
	for (int i = 0; i < ColorsCount; i++)
	{
		if (ColorsValues[i] == GLOBAL_TEXT_COLOR)
		{
			index = i;
			break;
		}
	}

	String actualColor = Colors[index];

	drawSettingsMenu(0, true, true, false);
	move = waitforactionXY(200);
	do
	{
		move = waitforactionXY(150);
		if (move == 1)
		{
			if (index < ColorsCount - 1)
				index++;
			else
				index = 0;
		}
		if (move == 3)
		{
			if (index > 0)
				index--;
			else
				index = ColorsCount - 1;
		}
		actualColor = Colors[index];
		GLOBAL_TEXT_COLOR = ColorsValues[index];
		itemsSetting[2] = "3. Kolor tekstu: " + String(actualColor);
		if (move != 5 && move !=4)
			drawSettingsMenu(0, true, false, true);
		else
			drawSettingsMenu(0, false, false, true);

	} while (move != 5 && move != 4);
	EEPROM_writeAnything(EEPROM_LAST_COLOR, index);

}

void drawProfileList(int select, bool isNewWindow)
{
	if(isNewWindow)
      tft.fillScreen(TFT_BLACK);

	tft.setCursor(70, 10);
	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR);
	tft.println("WYBIERZ PROFIL");
	tft.setCursor(0, 70);


	for (uint8_t i = 0; i < ProfilesCount; i++)
	{
		tft.setTextSize(3);
		if (i == select)
		{
			tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		}
		if (i != select)
		{
			tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		}
		tft.println(ProfilesValues[i].name + " K: " + String(ProfilesValues[i].angleValue) + " T: " + String(ProfilesValues[i].tempValue));
	}

	if (select < ProfilesCount)
	{
		tft.fillRoundRect(200, 250, 80, 50, 5, TFT_BLACK);
		tft.drawRoundRect(200, 250, 80, 50, 5, TFT_WHITE);
	}
	else
		tft.fillRoundRect(200, 250, 80, 50, 5, SELECTED_COLOR);

	tft.fillTriangle(240, 285, 240, 265, 220, 275, TFT_WHITE);
	tft.fillRect(240, 270, 20, 10, TFT_WHITE);
}

void drawCalibartion()
{
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
	tft.setCursor(60, 100);
	tft.println("Kalibrowanie \nczujnika kata");
	for (int i = 0; i < 10; i++)
	{
		for (int i = 0; i < 36; i++)
		{
			tft.drawPixel(degreesTab[i].x, degreesTab[i].y, TFT_RED);
			delay(10);
		}
		for (int i = 0; i < 36; i++)
		{
			tft.drawPixel(degreesTab[i].x, degreesTab[i].y, TFT_BLACK);
			delay(10);
		}
	}

	tft.setTextColor(TFT_GREEN);
	tft.setTextSize(4);
	tft.setCursor(zeroPointX - 20, zeroPointY - 15);
	tft.println("OK");

	int waitForMowe = waitforactionXY(50);

	drawSettingsMenu(0, false, false, true);

}