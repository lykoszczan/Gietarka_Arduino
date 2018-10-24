int waitforactionY(int wait)
{
	int joymove;
	int zwrot = 0;
	// tutaj rozdzielenie prawo lewo gora dol wcisniecie i przypisanie odpowiedniego indeksu do zmiennej joymove
	delay(wait);
	joymove = 1;  // g�ra
	do
	{
		joymove = analogRead(A0);  // przedzialy X: 362 - �rodek, 687 - prawo, 0 - lewo 
								   // przedzia�y Y: 332 - �rodek, 687 - d�, 0 - g�ra
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
			zwrot = 3;  // d�
		else
			zwrot = 1;  //g�ra    
	}
	return zwrot;
}

int waitforactionX(int wait)
{
	int joymove;
	int zwrot = 0;
	// tutaj rozdzielenie prawo lewo gora dol wcisniecie i przypisanie odpowiedniego indeksu do zmiennej joymove
	delay(wait);
	joymove = 2;  // g�ra
	do
	{
		joymove = analogRead(A1);  // przedzialy X: 362 - �rodek, 687 - prawo, 0 - lewo 
								   // przedzia�y Y: 332 - �rodek, 687 - d�, 0 - g�ra
		joypos = joymove;
		if (digitalRead(SW_pin) == LOW)
		{
			zwrot = 5;
			break;
		}
	} while (joymove > 240 && joymove < 450);
	if (zwrot == 0)
	{
		if (joymove >= 450)
			zwrot = 2;  // prawo
		else
			zwrot = 4;  //lewo    
	}
	return zwrot;
}