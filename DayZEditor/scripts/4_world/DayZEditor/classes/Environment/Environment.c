
modded class Environment
{
	
	void StartEVR()
	{
		// temporary
		GetGame().GetWeather().GetOvercast().Set(0, 0, 0);
		GetGame().GetWeather().GetRain().Set(0, 0, 0);
		
		BlowoutEvent blowout_event = new BlowoutEvent(new BlowoutEventSettings());
		blowout_event.StartBlowout(Vector(7500, 0, 7500));
	}	
}
