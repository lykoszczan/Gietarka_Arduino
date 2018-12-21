void chooseOption()
{
	dirct = waitforactionXY(200);
	switch (dirct)
	{
		case 1: //góra
		{
			if (joyIndex != 0)
			{
				joyIndex--;
				drawMainMenu(joyIndex, false, false, false);
			}
			else
			{
				joyIndex = MenuItemsCount - 1;
				drawMainMenu(joyIndex, false, false, false);
			}
			break;
		}
		case 2: //prawo równie¿ traktowaæ jako wciœniêcie
		{
			SelectActionMainMenu(joyIndex);
			break;
		}
		case 3: //dó³
		{
			if (joyIndex != MenuItemsCount - 1)
			{
				joyIndex++;
				drawMainMenu(joyIndex, false, false, false);
			}
			else
			{
				joyIndex = 0;
				drawMainMenu(joyIndex, false, false, false);
			}
			break;
		}
		case 5: //wcisniecie
		{
			SelectActionMainMenu(joyIndex);
			break;
		}

	}
}

void SelectActionMainMenu(int index)
{
	switch (index)
	{
		case 0:
		{
			drawSetAngle();
			break;
		}
		case 1:
		{
			drawSetTemp();
			break;
		}
		case 2:
		{
			drawProfileList(0, true);
			chooseProfile();
			break;
		}
		case 3:
		{
			if (!isAngleSet || !isAngleSet)
			{
				drawSetHintBind();
			}
			else
			{
				if (checkHeater())
				{
					timeElapsed = millis();
					RunWorkView();
				}
				else
				{
					drawMainMenu(3, false, false, true);
				}

			}
			break;
		}
		case 4:
		{
			drawSettingsMenu(0, false, false, true);
			chooseOptionSettings();
			break;
		}

	}
}

boolean getYesNoAwser(int startIndex, int styleNumber)
{
	uint8_t end = 1;
	while (end != 0)
	{
		dirct = waitforactionX(300);
		switch (dirct)
		{
			case 2:  //prawo
			{
				if (startIndex == 1)
				{
					startIndex = 0;
					drawAnswer(false, startIndex, styleNumber);
				}
				else
				{
					startIndex = 1;
					drawAnswer(false, startIndex, styleNumber);
				}
				break;
			}
			case 4: //lewo
			{
				if (startIndex == 1)
				{
					startIndex = 0;
					drawAnswer(false, startIndex, styleNumber);
				}
				else
				{
					startIndex = 1;
					drawAnswer(false, startIndex, styleNumber);
				}
				break;
			}
			case 5:
			{
				if (startIndex == 0)
					return true;
				else
					return false;

				end = 0;
				break;
			}

		}
	}
}

void chooseOptionSettings()
{
	uint8_t end = 1;
	joyIndex = 0;
	while (end != 0)
	{
		dirct = waitforactionXY(200);
		switch (dirct)
		{
			case 1: //góra
			{
				if (joyIndex != 0)
				{
					joyIndex--;
					drawSettingsMenu(joyIndex, false, false, false);
				}
				else
				{
					joyIndex = SETTINGS; // tutaj juz nie ma - 1 ze wzgledu ze na 2 indeksie jest przycisk powroc do menu
					drawSettingsMenu(joyIndex, false, false, false);
				}
				break;
			}
			case 2: // prawo traktowac jako wcisniecie
			{
				switch (joyIndex)
				{
					case 0:			//kalibracja
					{
						calibration();
						drawSettingsMenu(joyIndex, false, false, true);
						break;
					}
					case 1:			//nastawy regulatora
					{
						PIDTuning();
						drawSettingsMenu(joyIndex, false, false, true);
						break;
					}
					case 2:       //Kolory
					{
						drawSetColor();
						break;
					}
					case 3:       //O programie
					{
						drawAbout();
						drawSettingsMenu(joyIndex, false, false, true);
						break;
					}
				}
				break;
			}
			case 3: //dó³
			{
				if (joyIndex != SETTINGS)
				{
					joyIndex++;
					drawSettingsMenu(joyIndex, false, false, false);
				}
				else
				{
					joyIndex = 0;
					drawSettingsMenu(joyIndex, false, false, false);
				}
				break;
			}
			case 4: //lewo - na strzalce oznacza to powrot, na innej pozycji nic nie robi
			{
				if (joyIndex == SETTINGS)
				{
					joyIndex = 0;
					end = 0;
				}
				break;
			}
			case 5: //wcisniecie
			{
				switch (joyIndex)
				{
					case 0:			//kalibracja
					{
						calibration();
						drawSettingsMenu(joyIndex, false, false, true);
						break;
					}
					case 1:			//nastawy regulatora
					{
						PIDTuning();
						drawSettingsMenu(joyIndex, false, false, true);
						break;
					}
					case 2:       //Kolory
					{
						drawSetColor();
						break;
					}
					case 3:       //O programie
					{
						drawAbout();
						drawSettingsMenu(joyIndex, false, false, true);
						break;
					}
					case 4:		// strzalka
					{
						joyIndex = 0;
						end = 0;
						break;
					}

				}
				break;
			}

		}
	}
	drawMainMenu(0, false, false, true);
	chooseOption();
}

void chooseProfile()
{
	uint8_t end = 1;
	joyIndex = 0;
	while (end != 0)
	{
		dirct = waitforactionXY(200);
		switch (dirct)
		{
			case 1: //góra
			{
				if (joyIndex != 0)
				{
					joyIndex--;
					drawProfileList(joyIndex,false);
				}
				else
				{
					joyIndex = ProfilesCount;
					drawProfileList(joyIndex, false);
				}
				break;
			}
			case 2: // prawo traktowac jako wcisniecie
			{
				if (joyIndex != ProfilesCount)
				{
					isTempSet = true;
					defaultAngle = ProfilesValues[joyIndex].angleValue;
					defaultTemp = ProfilesValues[joyIndex].tempValue;
					EEPROM_writeAnything(EEPROM_LAST_ANGLE, defaultAngle);
					EEPROM_writeAnything(EEPROM_LAST_TEMP, defaultTemp);
					items[0] = "1. Kat: " + String(defaultAngle) + " ";
					items[1] = "2. Temperatura: " + String(defaultTemp) + " C";
					joyIndex = 0;
					end = 0;
				}
				break;
			}
			case 3: //dó³
			{
				if (joyIndex != ProfilesCount)
				{
					joyIndex++;
					drawProfileList(joyIndex, false);
				}
				else
				{
					joyIndex = 0;
					drawProfileList(joyIndex, false);
				}
				break;
			}
			case 4: //lewo - na strzalce oznacza to powrot, na innej pozycji nic nie robi
			{
				if (joyIndex == ProfilesCount)
				{
					joyIndex = 0;
					end = 0;
				}
				break;
			}
			case 5: //wcisniecie
			{
				if (joyIndex == ProfilesCount)
				{
					joyIndex = 0;
					end = 0;
				}
				else
				{
					isTempSet = true;
					defaultAngle = ProfilesValues[joyIndex].angleValue;
					defaultTemp = ProfilesValues[joyIndex].tempValue;
					EEPROM_writeAnything(EEPROM_LAST_ANGLE, defaultAngle);
					EEPROM_writeAnything(EEPROM_LAST_TEMP, defaultTemp);
					items[0] = "1. Kat: " + String(defaultAngle) + " ";
					items[1] = "2. Temperatura: " + String(defaultTemp) + " C";
					joyIndex = 0;
					end = 0;
				}

				break;				
			}

		}
	}
	drawMainMenu(0, false, false, true);
	chooseOption();
}