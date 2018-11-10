void chooseOption()
{
	dirct = waitforactionY(300);
	switch (dirct)
	{
	case 1: //góra
	{
		if (index != 0)
		{
			index--;
			drawMainMenu(index, false, false);
		}
		else
		{
			index = ARRAYSIZE - 1;
			drawMainMenu(index, false, false);
		}
		break;
	}
	case 3: //dó³
	{
		if (index != ARRAYSIZE - 1)
		{
			index++;
			drawMainMenu(index, false, false);
		}
		else
		{
			index = 0;
			drawMainMenu(index, false, false);
		}
		break;
	}
	case 5: //wcisniecie
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
			drawWorkView();
			break;
		}
		case 3:
		{
			drawSettingsMenu(0, false, false);
			chooseOptionSettings();
			break;
		}

		}
		break;
	}

	}
}

boolean Cancel(int startindex)
{
	uint8_t end = 1;
	while (end != 0)
	{
		dirct = waitforactionX(300);
		switch (dirct)
		{
		case 2:  //prawo
		{
			if (startindex == 1)
			{
				startindex = 0;
				drawAnswer(false, startindex);
			}
			else
			{
				startindex = 1;
				drawAnswer(false, startindex);
			}
			break;
		}
		case 4: //lewo
		{
			if (startindex == 1)
			{
				startindex = 0;
				drawAnswer(false, startindex);
			}
			else
			{
				startindex = 1;
				drawAnswer(false, startindex);
			}
			break;
		}
		case 5:
		{
			if (startindex == 0)
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
	index = 0;
	while (end != 0)
	{
		dirct = waitforactionY(300);
		switch (dirct)
		{
		case 1: //góra
		{
			if (index != 0)
			{
				index--;
				drawSettingsMenu(index, false, false);
			}
			else
			{
				index = SETTINGS; // tutaj juz nie ma - 1 ze wzgledu ze na 4 indeksie jest przycisk powroc do menu
				drawSettingsMenu(index, false, false);
			}
			break;
		}
		case 3: //dó³
		{
			if (index != SETTINGS)
			{
				index++;
				drawSettingsMenu(index, false, false);
			}
			else
			{
				index = 0;
				drawSettingsMenu(index, false, false);
			}
			break;
		}
		case 5: //wcisniecie
		{
			switch (index)
			{
			case 0:       //Kolory
			{
				drawSetColor();
				break;
			}
			case 1:       //O programie
			{
				drawAbout();
				drawSettingsMenu(index, false, false);
				break;
			}
			case 2:
			{
				index = 0;
				end = 0;
				break;
			}

			}
			break;
		}

		}
	}
	drawMainMenu(0, false, false);
	chooseOption();
}
