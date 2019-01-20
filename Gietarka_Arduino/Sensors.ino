// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    

uint16_t samples[NUMSAMPLES];

int waitforactionY(int wait) //oś pionowa
{
	int joymove;
	int zwrot = 0;
	// Delay dlatego, że jeśli jest za krótki to przeskakuje kilka razy 
	delay(wait);
	joymove = 1;  // góra
	do
	{
		joymove = analogRead(Y_pin);  // przedzialy X: 362 - środek, 687 - prawo, 0 - lewo 
								   // przedziały Y: 332 - środek, 687 - dół, 0 - góra
		joypos = joymove;
		if (digitalRead(SW_pin) == LOW)
		{
			zwrot = 5;
			break;
		}
	} while (joymove > 220 && joymove < 450);
	if (zwrot == 0)
	{
		if (joymove >= 450)
			zwrot = 3;  // dół
		else
			zwrot = 1;  //góra    
	}
	return zwrot;
}

int waitforactionXY(int wait) // obie osie
{
	int moveHorz, moveVert;
	int zwrot = 0;
	// Delay dlatego, że jeśli jest za krótki to przeskakuje kilka razy 
	delay(wait);
	do
	{
		moveVert = analogRead(Y_pin);  // pionowo
		moveHorz = analogRead(X_pin);  // poziomo

		if (digitalRead(SW_pin) == LOW)
		{
			zwrot = 5;
			break;
		}
	} while ((moveVert > MIDDLE_MIN_Y && moveVert < MIDDLE_MAX_Y) && (moveHorz > MIDDLE_MIN_X && moveHorz < MIDDLE_MAX_X));
	if (zwrot == 0)
	{
		if (moveVert > MIDDLE_MIN_Y && moveVert < MIDDLE_MAX_Y)
		{
			if (moveHorz >= MIDDLE_MAX_X)
				zwrot = 2;  // PRAWO  ------  jako wcisniecie  
			else
				zwrot = 4;  // LEWO
		}
		else
		{
			if (moveVert >= MIDDLE_MAX_Y)
				zwrot = 3;  // dół
			else
				zwrot = 1;  //góra    
		}
	}
	return zwrot;
}

int waitforactionX(int wait) // oś pozioma
{
	int joymove;
	int zwrot = 0;
	// Delay dlatego, że jeśli jest za krótki to przeskakuje kilka razy 
	delay(wait);
	joymove = 2;  // góra
	do
	{
		joymove = analogRead(X_pin);  // przedzialy X: 362 - środek, 687 - prawo, 0 - lewo 
								   // przedziały Y: 332 - środek, 687 - dół, 0 - góra
		joypos = joymove;
		if (digitalRead(SW_pin) == LOW)
		{
			zwrot = 5;
			break;
		}
	} while (joymove > MIDDLE_MIN_X && joymove < MIDDLE_MAX_X);
	if (zwrot == 0)
	{
		if (joymove >= MIDDLE_MAX_X)
			zwrot = 2;  // prawo
		else
			zwrot = 4;  //lewo    
	}
	return zwrot;
}

double getAngle()
{
	const float pi = 3.141592;
	const int sample_no = 100; // no of samples for aproximation
	int16_t ax, ay, az;
	float x, y, z;
	int  sample = 0;
	float _angle_x, angle_x, _angle_y, angle_y;
	long ax_sum = 0, ay_sum = 0, az_sum = 0;
	
	while (true)
	{
		//accelerometer.setXAccelOffset(0);
		//accelerometer.setYAccelOffset(0);
		accelerometer.setZAccelOffset(ZaxisOffset);

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

			angle_y = (atan2(-y, z)*180.0) / M_PI;
			angle_y = 180 - abs(angle_y);

			// Reset values for next aproximation   
			sample = 0;
			ax_sum = 0;
			ay_sum = 0;
			az_sum = 0;


			// debug
			//Serial.print(angle_x);
			//Serial.print("\t"); // \t = tablator 
			//Serial.println(angle_y);

			return angle_y;
			break;

		}
	}

}

bool checkHeater()
{
	double lastTemp = mlxWire.readObjectTempC();
	analogWrite(MOSFET_PIN, 255);

	drawCheckHeater();

	if (lastTemp + 10 < mlxWire.readObjectTempC())
	{
		analogWrite(MOSFET_PIN, 0);
		drawLoadGifOK();
	}
	else
	{
		analogWrite(MOSFET_PIN, 0);
		drawLoadGifERROR();
	}


	return true;
}

void calibration()
{
	int move;

	drawCalibartion();
	while (true)
	{
		drawAngleCalibration();

		move = waitforactionXY(50);

		if (move == 1) // góra
		{
			ZaxisOffset += 1;
		}
		else if (move == 3) // dół
		{
			ZaxisOffset -= 1;
		}
		else if (move == 4 || move == 5) // lewo - powrót
		{
			break;
		}
	}
	EEPROM_writeAnything(EEPROM_LAST_OFFSET, ZaxisOffset);
	delay(50);

}

void PIDTuning()
{
	int move;
	uint8_t end = 1;
	int position = 0;
	drawPIDTuning(position, false, true);

	while (end != 0)
	{
		move = waitforactionXY(200);
		switch (move)
		{
			case 1: // góra
			{
				if (position == 0)
					position = 3;
				else
					position--;
				drawPIDTuning(position, false, false);
				break;
			}
			case 2: // prawo - wybór
			{
				drawPIDTuning(position, true, false);
				break;
			}
			case 3: // dół
			{
				if (position == 3)
					position = 0;
				else
					position++;
				drawPIDTuning(position, false, false);
				break;
			}
			case 4: // lewo - powrót na strzalce
			{
				if (position == 3)
					end = 0;
				else
					drawPIDTuning(position, false, false);
				break;
			}
			case 5: // wcisniecie
			{
				if (position == 3)
					end = 0;
				else
				{
					setPIDValues(position);
				}

				break;
			}
		}


	}
}

void setPIDValues(int position)
{
	uint8_t move = 0;
	int beforemove;

	drawPIDTuning(position, true, false);
	move = waitforactionXY(200);
	do
	{
		beforemove = PID_Items[position].value;
		move = waitforactionXY(50);
		if (move == 1)
			PID_Items[position].value++;
		if (move == 3 && PID_Items[position].value>0)
			PID_Items[position].value--;
		if(String(beforemove).length() != String(PID_Items[position].value).length())
			drawPIDTuning(position, true, true);
		else
			drawPIDTuning(position, true, false);

	} while (move != 5 && move != 4);

	EEPROM_writeAnything(EEPROM_PID_KP, PID_Items[0].value);
	EEPROM_writeAnything(EEPROM_PID_KI, PID_Items[1].value);
	EEPROM_writeAnything(EEPROM_PID_KD, PID_Items[2].value);

	drawPIDTuning(position, false, false);
}

double readThermistorTemperature()
{
	uint8_t i;
	float average;

	// take N samples in a row, with a slight delay
	for (i = 0; i< NUMSAMPLES; i++) {
		samples[i] = analogRead(THERMISTORPIN);
		delay(10);
	}

	// average all the samples out
	average = 0;
	for (i = 0; i< NUMSAMPLES; i++) {
		average += samples[i];
	}
	average /= NUMSAMPLES;

	Serial.print("Average analog reading ");
	Serial.println(average);

	// convert the value to resistance
	average = 1023 / average - 1;
	average = SERIESRESISTOR / average;
	Serial.print("Thermistor resistance ");
	Serial.println(average);

	float steinhart;
	steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
	steinhart = log(steinhart);                  // ln(R/Ro)
	steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
	steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
	steinhart = 1.0 / steinhart;                 // Invert
	steinhart -= 273.15;                         // convert to C

	Serial.print("Temperature ");
	Serial.print(steinhart);
	Serial.println(" *C");

	return steinhart;
}