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
				zwrot = 2;  // LEWO 
			else
				zwrot = 4;  // PRAWO  ------  jako wcisniecie 
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
		accelerometer.setZAccelOffset(2500);

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

			if (angle_y < 0)
				angle_y = 0;

			// Reset values for next aproximation   
			sample = 0;
			ax_sum = 0;
			ay_sum = 0;
			az_sum = 0;

			Serial.print(angle_x);
			Serial.print("\t"); // \t = tablator 
			Serial.println(angle_y);

			return angle_y;
			break;

		}
	}

}

bool checkHeater()
{
	drawCheckHeater();
	return true;
}

