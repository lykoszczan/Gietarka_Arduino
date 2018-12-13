void RunPIDController()
{
	int kp = PID_Items[0].value;
	int ki = PID_Items[1].value;
	int kd = PID_Items[2].value;

	//Kalkulacja b³êdu pomiêdzy wartoœci¹ zadan¹ i wielkoœci¹ rzeczywist¹
	PID_error = defaultTemp - mlx.readObjectTempC();
	//Wartoœæ cz³onu P
	PID_p = 0.01*kp * PID_error;
	//Wartoœæ cz³onu I
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
	//Now we can write the PWM signal to the mosfet on digital pin D3
	//Since we activate the MOSFET with a 0 to the base of the BJT, we write 255-PID value (inverted)
	analogWrite(MOSFET_PIN,PID_value);
	previous_error = PID_error;     //Remember to store the previous error for next loop.

	delay(250); //Refresh rate + delay of LCD print
	
	Serial.println("PID_value: " + String(PID_value));

}

void StopPIDController()
{
	PID_value = 0;
	analogWrite(MOSFET_PIN, PID_value);
	Serial.println("PID_value: " + String(PID_value));
}