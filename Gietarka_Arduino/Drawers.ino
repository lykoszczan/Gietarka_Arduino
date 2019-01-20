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

	for (int i = 0; i < 36; i++)
	{
		tft.drawPixel(degreesTab[i].x, degreesTab[i].y, TFT_RED);
		delay(50);
	}
}

void drawLoadGifERROR()
{
	tft.setTextSize(4);
	tft.setTextColor(TFT_RED, TFT_BLACK);
	tft.setCursor(35, 100);
	tft.println("Sprawdzam grzalke");

	tft.drawLine(zeroPointX - 25, zeroPointY - 25, zeroPointX + 25, zeroPointY + 25, TFT_RED);
	tft.drawLine(zeroPointX - 25, zeroPointY + 25, zeroPointX + 25, zeroPointY - 25, TFT_RED);
	delay(200);

	tft.fillScreen(TFT_RED);
	tft.setTextSize(4);
	tft.setCursor(30, 100);
	tft.setTextColor(TFT_BLACK);
	tft.println("Grzalka nie dziala");
	tft.setCursor(160, 130);
	tft.println("poprawnie");
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
		tft.drawPixel(degreesTab[i].x, degreesTab[i].y, TFT_GREEN);
	}
	tft.setTextColor(TFT_GREEN);
	tft.setTextSize(4);
	tft.setCursor(zeroPointX - 20, zeroPointY - 15);
	tft.println("OK");
	delay(200);
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
			position = space * (String(angleSetPoint).length() - 1) + firstPixel;
		}
		if (menuIndex == 1)
		{
			firstPixel = 312;
			topPixel = 107;
			position = space * (String(tempSetPoint).length() - 1) + firstPixel;
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
	tft.setCursor(0, 80);

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
	items[0] = "1. Kat: " + String(angleSetPoint) + " ";
	drawMainMenu(0, true, true, false);
	move = waitforactionXY(200);
	do
	{
		beforemove = angleSetPoint;
		move = waitforactionXY(50);
		if (move == 1 && angleSetPoint<MAX_ANGLE)
			angleSetPoint++;
		if (move == 3 && angleSetPoint>MIN_ANGLE)
			angleSetPoint--;
		items[0] = "1. Kat: " + String(angleSetPoint) + " ";
		if (String(angleSetPoint).length() != String(beforemove).length())
			drawMainMenu(0, true, false, true);
		else
			drawMainMenu(0, true, true, false);

	} while (move != 5 && move != 4);
	EEPROM_writeAnything(EEPROM_LAST_ANGLE, angleSetPoint);

	drawMainMenu(0, false, true, false);
}

void drawSetTemp()
{
	uint8_t move;
	int wait = 30;
	int beforemove;

	isTempSet = true;
	items[1] = "2. Temperatura: " + String(tempSetPoint) + " C";
	drawMainMenu(1, true, true, false);
	move = waitforactionXY(200);
	do
	{
		beforemove = tempSetPoint;
		move = waitforactionXY(wait);
		if (move == 1 && tempSetPoint<MAX_TEMP)
		{
			tempSetPoint++;
		}
		if (move == 3 && tempSetPoint>MIN_TEMP)
		{
			tempSetPoint--;
		}

		items[1] = "2. Temperatura: " + String(tempSetPoint) + " C";
		if (String(tempSetPoint).length() != String(beforemove).length())
			drawMainMenu(1, true, false, true);
		else
			drawMainMenu(1, true, true, false);

	} while (move != 5 && move != 4);

	EEPROM_writeAnything(EEPROM_LAST_TEMP, tempSetPoint);

	drawMainMenu(1, false, true, false);
}

void refreshWorkView()
{
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setCursor(30, 20);
	tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
	tft.println("Kat = " + String(angleSetPoint));
	if (String(angleSetPoint).length() == 3)
	{
		tft.drawCircle(141, 22, 2, GLOBAL_TEXT_COLOR);		
	}
	else
	{
		tft.drawCircle(130, 22, 2, GLOBAL_TEXT_COLOR);
	}
	tft.drawRoundRect(30, 60, 180, 140, 3, TFT_WHITE);  //K¹t

	tft.setCursor(255, 20);
	tft.println("Temperatura = " + String(tempSetPoint));
	if (String(tempSetPoint).length() == 3)
	{
		tft.drawCircle(462, 22, 2, GLOBAL_TEXT_COLOR);
		tft.setCursor(467, 20);
	}
	else
	{
		tft.drawCircle(451, 22, 2, GLOBAL_TEXT_COLOR);
		tft.setCursor(456, 20);
	}
	tft.println("C");
	tft.drawRoundRect(255, 60, 205, 140, 3, TFT_WHITE);  //Temperatura
}

void drawStopHeat() //-- po bezczynnosci
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

void drawTableIsHot()
{
	tft.fillScreen(TFT_RED);
	tft.setTextSize(3);
	tft.setCursor(10, 80);
	tft.setTextColor(TFT_BLACK, TFT_BLACK);
	tft.println("Grzanie zostalo wylaczone");// \nUrzadzenie jest za gorace\n     Poczekaj az ostygnie");
	tft.setCursor(12, 110);
	tft.println("Urzadzenie jest za gorace");
	tft.setCursor(60, 140);
	tft.println("Poczekaj az ostygnie");

	tft.setTextColor(TFT_BLACK, SELECTED_COLOR);
	tft.setCursor(zeroPointX - 20, zeroPointY - 15);
	tft.setTextSize(4);
	tft.println("OK");
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

void drawEndHeat()
{
	tft.fillRect(80, 270, 400, 50, TFT_BLACK);
	tft.setCursor(105, 280);
	tft.setTextSize(3);
	tft.setTextColor(WARNING_COLOR, SELECTED_COLOR);
	tft.println("ZAKONCZ GRZANIE");
}

void drawStartHeat()
{
	tft.setCursor(85, 280);
	tft.setTextSize(3);
	tft.setTextColor(TFT_GREEN, TFT_BLACK);
	tft.println("ROZPOCZNIJ GRZANIE");
}

void showWarning(double temp)
{
	if (!targetValuesReached)
	{
		if (temp<tempSetPoint)
		{
			tft.setCursor(120, 230);
			tft.setTextColor(WARNING_COLOR, TFT_BLACK);
			tft.setTextSize(2);
			tft.println(" Zaczekaj az grzalka     \n          osiagnie temperature            ");
			tft.setTextColor(TFT_GREEN);
		}
		else
		{
			tft.setCursor(120, 230);
			tft.setTextColor(TFT_GREEN, TFT_BLACK);
			tft.setTextSize(2);
			tft.println(" Grzalka osiagnela      \n              temperature                ");

		}
	}
	else
	{
		tft.setCursor(120, 230);
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.setTextSize(2);
		tft.println("Grzalka jest wylaczona      \n     Wybierz aby uruchomic ponownie        ");
	}
}

void drawTemperatureTimes(double currentTempWire, double currentTempObj)
{
	if (currentTempWire >= tempSetPoint - 5)
	{
		if (isHeatStartWire)
		{
			heatTimeWire = millis();
			isHeatStartWire = false;
		}

		tft.setCursor(260, 110);
		tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println("t = " + String(int((millis() - heatTimeWire) / 1000)) + "s");
	}
	else
	{
		isHeatStartWire = true;
		tft.setCursor(260, 110);
		tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println("             ");
	}

	if (currentTempObj >= tempSetPoint - 5)
	{
		if (isHeatStartObj)
		{
			heatTimeObj = millis();
			isHeatStartObj = false;
		}
		tft.setCursor(260, 175);
		tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println("t = " + String(int((millis() - heatTimeObj) / 1000)) + "s");
	}
	else
	{
		isHeatStartObj = true;
		tft.setCursor(260, 175);
		tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println("             ");
	}
}

void drawTemperature(double tempWire, double tempObj)
{
	String outText; 

	// Temperatura drutu
	outText = String(tempWire);
	outText = outText.substring(0, 5);
	tft.setCursor(260, 83);
	if (tempWire<tempSetPoint)
	{
		tft.setTextColor(WARNING_COLOR, TFT_BLACK);
		tft.setTextSize(2);
		tft.println("Drut:");
		tft.setTextSize(3);
		tft.setCursor(330, 80);
		tft.println(outText);
		tft.drawCircle(425, 85, 5, WARNING_COLOR);
		tft.setCursor(435, 80);
		tft.println("C");
	}
	else
	{
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.setTextSize(2);
		tft.println("Drut:");
		tft.setTextSize(3);
		tft.setCursor(330, 80);
		tft.println(outText);
		tft.drawCircle(425, 85, 5, TFT_GREEN);
		tft.setCursor(435, 80);
		tft.println("C");
	}

	// Temperatura materia³u
	tft.setCursor(260, 148);
	outText = String(tempObj);
	outText = outText.substring(0, 5);
	if (tempObj<tempSetPoint)
	{
		tft.setTextColor(WARNING_COLOR, TFT_BLACK);
		tft.setTextSize(2);
		tft.println("Mat:");
		tft.setTextSize(3);
		tft.setCursor(330, 145);
		tft.println(outText);
		tft.drawCircle(425, 150, 5, WARNING_COLOR);
		tft.setCursor(435, 145);
		tft.println("C");
	}
	else
	{
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.setTextSize(2);
		tft.println("Mat:");
		tft.setTextSize(3);
		tft.setCursor(330, 145);
		tft.println(outText);
		tft.drawCircle(425, 150, 5, TFT_GREEN);
		tft.setCursor(435, 145);
		tft.println("C");
	}


	showWarning(tempWire);
}

void drawAngle(double currentAngle)
{
	String outText = String(abs(currentAngle));
    
	if (outText.length() < 5)
	{
		outText = outText + "0";
	}

	outText = outText.substring(0, 5);

	tft.setCursor(40, 110);
	tft.setTextSize(5);
	if (currentAngle<angleSetPoint)
	{
		tft.setTextColor(WARNING_COLOR, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(195, 115, 5, WARNING_COLOR);
	}
	else
	{
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(195, 115, 5, TFT_GREEN);
	}

}

void drawAngleCalibration()
{
	double currentAngle = abs(getAngle());
	String outText = String(abs(currentAngle));

	if (outText.length() < 5)
	{
		outText = outText + "0";
	}

	outText = outText.substring(0, 5);

	tft.setCursor(160, 130);
	tft.setTextSize(4);
	if (currentAngle < 90 || currentAngle >= 91)
	{
		tft.setTextColor(WARNING_COLOR, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(285, 135, 5, WARNING_COLOR);
	}
	else
	{
		tft.setTextColor(TFT_GREEN, TFT_BLACK);
		tft.println(outText);
		tft.drawCircle(285, 135, 5, TFT_GREEN);
	}

	tft.setCursor(90, 250);
	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR, EDIT_COLOR);
	tft.println("Offset = " + String(ZaxisOffset));
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

	drawSettingsMenu(2, true, true, false);
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
			drawSettingsMenu(2, true, false, true);
		else
			drawSettingsMenu(2, false, false, true);

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
	tft.setTextColor(GLOBAL_TEXT_COLOR);
	tft.setCursor(130, 10);
	tft.println("KALIBRACJA");
	tft.setTextSize(3);
	tft.setCursor(70, 60);
	tft.println("Ustaw kat 90 stopni");
	tft.drawRoundRect(140, 100, 180, 100, 3, TFT_WHITE);  //K¹t
}

void drawPIDTuning(int select, bool isEdit, bool isNewWindow)
{
	if(isNewWindow)
		tft.fillScreen(TFT_BLACK);
	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
	tft.setCursor(120, 10);
	tft.println("NASTAWY PID");

	for (uint8_t i = 0; i < 3; i++)
	{
		tft.setTextSize(3);
		if (i == select)
		{
			if(isEdit)
				tft.setTextColor(GLOBAL_TEXT_COLOR, EDIT_COLOR);
			else
				tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		}
		if (i != select)
		{
			tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		}
		tft.setCursor(20, 80 + 50*i);
		tft.println(PID_Items[i].name + " = " + String(PID_Items[i].value));
	}

	if (!isEdit)
	{
		if (select == 0 || select == 2)
		{
			tft.fillRoundRect(200, 250, 80, 50, 5, TFT_BLACK);
			tft.drawRoundRect(200, 250, 80, 50, 5, TFT_WHITE);
		}
		else if (select == 3)
			tft.fillRoundRect(200, 250, 80, 50, 5, SELECTED_COLOR);

		tft.fillTriangle(240, 285, 240, 265, 220, 275, TFT_WHITE);
		tft.fillRect(240, 270, 20, 10, TFT_WHITE);
	}
}