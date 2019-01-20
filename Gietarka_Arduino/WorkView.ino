void RunWorkView()
{
	bool isCanceled = false;
	double currentTempObj, currentTempWire, currentAngle, currentTableTemp;
	double lastAngle = 0;
	int move, moveHorz, moveVert;
	long timeToProbe;
	
	targetValuesReached = false;
	refreshWorkView();
	drawEndHeat();

	currentAngle = 0;
	timeToProbe = millis();

	heatTimeWire = 0;
	heatTimeObj = 0;
	isHeatStartWire = true;
	isHeatStartObj = true;
	while (true)
	{
		//'{@Plot.Values.PID.Red PID_value};{@Plot.Values.Temperature.Green currentTempWire};{@Plot.Values.Temperature.Blue currentTempObj}'

		currentAngle = getAngle();
		currentTempObj = mlxObj.readObjectTempC();
		currentTempWire = mlxWire.readObjectTempC();
		currentTableTemp = 10; //readThermistorTemperature()

		if (currentTableTemp >= MAX_TABLE_TEMP)
		{
			StopPIDController();
			drawTableIsHot();
			waitforactionXY(50);
			refreshWorkView();
		}

		drawAngle(currentAngle);
		drawTemperature(currentTempWire, currentTempObj);				
		drawTemperatureTimes(currentTempWire, currentTempObj);

		// w momencie wykrycia giêcia materia³u zatrzymujemy grzanie
		if ((lastAngle > currentAngle + 5) || (lastAngle < currentAngle - 5))
		{
			drawStartHeat(); // grzanie zakonczone - zmieniamy komunikat
			targetValuesReached = true; // spowoduje to zakonczenie grzania
		}

		if (!targetValuesReached && (currentTempWire <= MAX_TEMP || currentTempObj > 90)) //temperatura mniejsza od max, ze wzglêdu gdyby nie by³o materia³u i wtedy ci¹gle by grza³o
		{
			RunPIDController();
		}
		else
		{
			StopPIDController();
		}

		// jezeli uzytkownik nie wykona zadnej czynnosci przez ustalony z gory czas. wylaczamy grzalke
		// ----- w celach bezpieczenstwa
		moveHorz = analogRead(X_pin);
		moveVert = analogRead(Y_pin);
		if (moveVert < MIDDLE_MIN_Y || moveVert > MIDDLE_MAX_Y || moveHorz > MIDDLE_MAX_X)
		{
			if (targetValuesReached)
			{
				RunPIDController();
				drawEndHeat();
				targetValuesReached = false;
			}
			timeElapsed = millis();

		}
		// -----

		if (digitalRead(SW_pin) == LOW) // czy jest wcisniety analog
		{
			if (!targetValuesReached)
			{
				StopPIDController();
				drawAnswer(true, 1, 0);
				if (getYesNoAwser(1, 0))
				{
					isCanceled = true;
					break;
				}
				else
				{
					RunPIDController();
					timeElapsed = millis();
					refreshWorkView();
					drawEndHeat();
				}
			}
			else
			{
				RunPIDController();
				drawEndHeat();
				targetValuesReached = false;
				timeElapsed = millis();
				lastAngle = currentAngle;
			}
		}
		if (moveHorz < MIDDLE_MIN_X) // czy jest wychylony w lewo analog
		{
			if (!targetValuesReached)
			{
				StopPIDController();
				drawAnswer(true, 1, 0);
				if (getYesNoAwser(1, 0))
				{
					isCanceled = true;
					break;
				}
				else
				{
					RunPIDController();
					timeElapsed = millis();
					refreshWorkView();
					drawEndHeat();
				}
			}
			else
			{
				isCanceled = true;
				break;
			}
		}

		if ((millis() - timeElapsed) > TIME_TO_STOP_HEAT &&  !targetValuesReached)
		{
			StopPIDController();
			drawStopHeat();
			if (getYesNoAwser(1, 1))
			{
				timeElapsed = millis();
				refreshWorkView();
				drawEndHeat();
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
		move = waitforactionY(200);
		break;
	}

	StopPIDController();
	drawMainMenu(3, false, false, true);
}
