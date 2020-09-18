
modded class Environment
{
	protected float m_EVRStrength = 0.5;
	
	void StartEVR()
	{
		BlowoutEvent blowout_event = new BlowoutEvent();
		blowout_event.StartBlowout(m_Player);
	}	
}
