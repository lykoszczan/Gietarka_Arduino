void RunWorkView()
{
	bool isCanceled = false;
	double currentTemp, currentAngle;
	double lastAngle;
	int move, moveHorz, moveVert;
	long timeToAprox;
	bool targetValuesReached = false;

	refreshWorkView();
	drawEndHeat();

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

		//'{@Plot.Values.PID.Red PID_value};{@Plot.Values.Temperature.Green currentTemp};'

		currentTemp = mlx.readObjectTempC();
		currentAngle = getAngle();

		drawAngle();
		drawTemperature();



		// po osiagnieciu zadanych wartosci zatrzymujemy grzanie w celu ostygniecia materialu
		if (currentTemp >= defaultTemp && currentAngle >= defaultAngle)
		{
			drawStartHeat(); // grzanie zakonczone - zmieniamy komunikat
			targetValuesReached = true; // spowoduje to zakonczenie grzania
		}

		if (!targetValuesReached)
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
		if ((lastAngle > currentAngle + 5) || (lastAngle < currentAngle - 5))
		{
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
