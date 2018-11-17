int waitforactionY(int wait)
{
	int joymove;
	int zwrot = 0;
	// Delay dlatego, ¿e jeœli jest za krótki to przeskakuje kilka razy 
	delay(wait);
	joymove = 1;  // góra
	do
	{
		joymove = analogRead(Y_pin);  // przedzialy X: 362 - œrodek, 687 - prawo, 0 - lewo 
								   // przedzia³y Y: 332 - œrodek, 687 - dó³, 0 - góra
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
			zwrot = 3;  // dó³
		else
			zwrot = 1;  //góra    
	}
	return zwrot;
}

int waitforactionXY(int wait)
{
	int moveHorz, moveVert;
	int zwrot = 0;
	// Delay dlatego, ¿e jeœli jest za krótki to przeskakuje kilka razy 
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
				zwrot = 3;  // dó³
			else
				zwrot = 1;  //góra    
		}
	}
	return zwrot;
}

int waitforactionX(int wait)
{
	int joymove;
	int zwrot = 0;
	// Delay dlatego, ¿e jeœli jest za krótki to przeskakuje kilka razy 
	delay(wait);
	joymove = 2;  // góra
	do
	{
		joymove = analogRead(X_pin);  // przedzialy X: 362 - œrodek, 687 - prawo, 0 - lewo 
								   // przedzia³y Y: 332 - œrodek, 687 - dó³, 0 - góra
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

	//double pitch, roll;
	//int Xg, Yg, Zg;

	//accelerometer.getAcceleration(&Xg, &Yg, &Zg);

	////Low Pass Filter
	//fXg = Xg * alpha + (fXg * (1.0 - alpha));
	//fYg = Yg * alpha + (fYg * (1.0 - alpha));
	//fZg = Zg * alpha + (fZg * (1.0 - alpha));

	////Roll & Pitch Equations
	//if (fZg != 0)
	//	roll = (atan2(-fYg, fZg)*180.0) / M_PI;
	//else
	//	roll = 0;

	//return (180 - roll); // poniewaz plytka jest odwrocona do gory nogami, zobaczymy jak bedzie na urzadzeniu umieszczona
	//-----
	
	
	while (true)
	{
		accelerometer.getAcceleration(&ax, &ay, &az);
		ax_sum = ax_sum + ax;
		ay_sum = ay_sum + ay;
		az_sum = az_sum + az;
		sample++;

		if (sample == sample_no)
		{
			// Uœrednienie wartoœci
			x = ax_sum / sample_no;
			y = ay_sum / sample_no;
			z = az_sum / sample_no;

			angle_y = (atan2(-y, z)*180.0) / M_PI;
			angle_y = 180 - abs(angle_y);

			// Obliczenie kata wychylenia					
			//if (z < 0)
			//	angle_y = atan2(y, sqrt(square(x) + square(z))) / (pi / 180);
			//else
			//{
			//	angle_y = atan2(y, sqrt(square(x) + square(z))) / (pi / 180);
			//	angle_y = 180 - angle_y;
			//}
			// Reset wartoœci przed kolejnym próbkowaniem
			sample = 0;
			ax_sum = 0;
			ay_sum = 0;
			az_sum = 0;

			return angle_y;

			// W celach debugowania
			Serial.println("Angle: " + String(angle_y));
			Serial.println("Y: " + String(y));
			Serial.println("X: " + String(x));
			Serial.println("Z: " + String(z));

			break;

		}
	}
}