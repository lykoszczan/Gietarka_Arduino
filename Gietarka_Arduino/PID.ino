void RunPIDController()
{
	int kp = PID_Items[0].value;
	int ki = PID_Items[1].value;
	int kd = PID_Items[2].value;

	//Kalkulacja b��du pomi�dzy warto�ci� zadan� i wielko�ci� rzeczywist�
	PID_error = tempSetPoint - mlxObj.readObjectTempC() + 3; //sterujemy temperatur� materia�u a nie drutu
	//Warto�� cz�onu P
	PID_p = 0.01*kp * PID_error;
	//Warto�� cz�onu I
	PID_i = 0.01*PID_i + (ki * PID_error);


	//For derivative we need real Time_PID to calculate speed change rate
	PID_timePrev = PID_time;                            // the previous Time_PID is stored before the actual Time_PID read
	PID_time = millis();                            // actual Time_PID read
	PID_timeElapsed = (PID_time - PID_timePrev) / 1000;
	//Now we can calculate the D calue
	PID_d = 0.01*kd*((PID_error - previous_error) / PID_timeElapsed);
	//Final total PID value is the sum of P + I + D
	PID_value = PID_p + PID_i + PID_d;

	//We define PWM range between 0 and 255
	if (PID_value < 0)
	{
		PID_value = 0;
	}
	if (PID_value > 255)
	{
		PID_value = 255;
	}

	analogWrite(MOSFET_PIN,PID_value);
	previous_error = PID_error;     //Remember to store the previous error for next loop.
}

void StopPIDController()
{
	PID_value = 0;
	analogWrite(MOSFET_PIN, PID_value);
}