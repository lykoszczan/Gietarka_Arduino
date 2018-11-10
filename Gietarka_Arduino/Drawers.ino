#define ARRAYSIZE 4
#define SELECTED_COLOR 0x00FF
#define WARNING_COLOR 0xF800
#define EDIT_COLOR 0xFF00

void drawMainMenu(int select, boolean edit, boolean onlythisrow) //edit odpowiada za kolor podswietlenia tekstu, onlythisrow przerysowuje tylko wyselectowany wiersz
{
	if (!onlythisrow)
		tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 10);
	tft.setTextColor(GLOBAL_TEXT_COLOR);


	for (uint8_t i = 0; i < ARRAYSIZE; i++) {
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
			tft.setTextColor(GLOBAL_TEXT_COLOR);
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

void drawSettingsMenu(int select, boolean edit, boolean onlythisrow) //edit odpowiada za kolor podswietlenia tekstu, onlythisrow przerysowuje tylko wyselectowany wiersz
{
	if (!onlythisrow)
		tft.fillScreen(TFT_BLACK);

	tft.setCursor(120, 10);
	tft.setTextSize(4);
	tft.setTextColor(GLOBAL_TEXT_COLOR);
	tft.println("USTAWIENIA");
	tft.setCursor(0, 100);

	for (uint8_t i = 0; i < SETTINGS; i++) {
		tft.setTextSize(2);
		if (i == select)
		{
			if (edit)
			  tft.setTextColor(GLOBAL_TEXT_COLOR, EDIT_COLOR);
			else
			  tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		}
		if (i != select)
		{
			tft.setTextColor(GLOBAL_TEXT_COLOR);
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

void drawWorkView()
{
	bool isCanceled = false;

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

	while (true)
	{
		drawAngle();
		drawTemperature();

		showWarning(mlx.readObjectTempC());

		if (digitalRead(SW_pin) == LOW)
		{
			drawAnswer(true, 1);
			if (Cancel(1))
			{
				isCanceled = true;
				break;
			}
			else
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
		}

		delay(100);
	}


	while (!isCanceled)
	{
		int move = waitforactionY(200);
		break;
	}

	drawMainMenu(2, false, false);
}

void drawAnswer(boolean firsttime, int index)
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
		tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		tft.println(answers[index]);

		tft.setCursor(335, 220);
		tft.setTextColor(GLOBAL_TEXT_COLOR, TFT_BLACK);
		tft.println(answers[index + 1]);

	}
	if (index == 1)  //prawo
	{
		tft.setTextSize(3);
		tft.setCursor(335, 220);
		tft.setTextColor(GLOBAL_TEXT_COLOR, SELECTED_COLOR);
		tft.println(answers[index]);

		tft.setCursor(100, 220);
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
	while (true)
	{
		accelerometer.getAcceleration(&ax, &ay, &az);
		ax_sum = ax_sum + ax;
		ay_sum = ay_sum + ay;
		az_sum = az_sum + az;
		sample++;

		if (sample == sample_no)
		{
			// mean values
			x = ax_sum / sample_no;
			y = ay_sum / sample_no;
			z = az_sum / sample_no;

			// Calculate of roll and pitch in deg
			
			angle_x = atan2(x, sqrt(square(y) + square(z))) / (pi / 180);
			if (z < 0)
			  angle_y = atan2(y, sqrt(square(x) + square(z))) / (pi / 180);
			else
			{
				angle_y = atan2(y, sqrt(square(x) + square(z))) / (pi / 180);
				angle_y = 180 - angle_y;
			}
			// Reset values for next aproximation   
			sample = 0;
			ax_sum = 0;
			ay_sum = 0;
			az_sum = 0;

			Serial.println("Angle: " + String(angle_y));
			Serial.println("Y: " + String(y));
			Serial.println("X: " + String(x));
			Serial.println("Z: " + String(z));

			break;

			//Serial.print(angle_x);
			//Serial.print("\t"); // \t = tablator 

		}
	}

	String outText = String(abs(angle_y));
    
	if (outText.length() < 5)
	{
		outText = outText + "0";
	}

	outText = outText.substring(0, 5);

	tft.setCursor(60, 130);
	tft.setTextSize(4);
	if (angle_y<defaultAngle)
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
	
	for (int i = 0; i < 4; i++)
	{
		if (ColorsValues[i] == GLOBAL_TEXT_COLOR)
		{
			index = i;
			break;
		}
	}

	String actualColor = Colors[index];

	itemsetting[0] = "1. Kolor tekstu: " + String(actualColor);
	drawSettingsMenu(0, true, true);
	move = waitforactionY(200);
	do
	{
		move = waitforactionY(150);
		if (move == 1)
		{
			if (index < 2)
				index++;
			else
				index = 0;
		}
		if (move == 3)
		{
			if (index > 0)
				index--;
			else
				index = 3;
		}
		actualColor = Colors[index];
		GLOBAL_TEXT_COLOR = ColorsValues[index];
		itemsetting[0] = "1. Kolor tekstu: " + String(actualColor);
		if (move != 5)
			drawSettingsMenu(0, true, false);
		else
			drawSettingsMenu(0, false, true);

	} while (move != 5);
	EEPROM_writeAnything(EEPROM_LAST_COLOR, index);

}