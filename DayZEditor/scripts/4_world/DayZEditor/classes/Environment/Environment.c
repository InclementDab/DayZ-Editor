
modded class Environment
{
	
	void StartEVR()
	{
		BlowoutEvent blowout_event = new BlowoutEvent(new BlowoutEventSettings());
		blowout_event.StartBlowout(m_Player.GetPosition());
	}	
}
