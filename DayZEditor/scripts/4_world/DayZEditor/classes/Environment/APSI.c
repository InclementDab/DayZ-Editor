
class APSI: Clothing
{
	
	void SwitchOn()
	{
		ComponentEnergyManager em = GetCompEM();
		if (em && em.CanSwitchOn()) {
			em.SwitchOn();
		}
	}
	
	void SwitchOff()
	{
		ComponentEnergyManager em = GetCompEM();
		if (em && em.CanSwitchOff()) {
			em.SwitchOff();
		}
	}
	
	bool IsSwitchedOn()
	{
		return GetCompEM().IsSwitchedOn();
	}
	

		
	override void OnWorkStart()
	{
		Print("OnWorkStart");
		if (!IsMissionClient()) return;
		thread PlayStartupEffect();
	}
		
	override void OnWorkStop()
	{
		Print("OnWorkStop");
		if (!IsMissionClient()) return;
		thread PlayShutdownEffect();
	}
	
	private void PlayStartupEffect()
	{
		PPEffects.SetVignette(0.25, 0, 0, 0);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(200);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(500);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(100);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(2000);
		PlaySoundOnPlayer(BlowoutSound.APSI_Enable);	
		auto x = new MaterialEffect("graphics/materials/postprocess/gauss");
		x.LerpParam("Intensity", 0.4, 0, 1);
	}
	
	private void PlayShutdownEffect()
	{	
		PPEffects.SetVignette(0, 0, 0, 0);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(200);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(200);
		PlaySoundOnPlayer(BlowoutSound.APSI_Disable);
	}

	private void PlaySoundOnPlayer(BlowoutSound sound)
	{		
		EffectSound effect;
		GetGame().GetPlayer().PlaySoundSet(effect, typename.EnumToString(BlowoutSound, sound), 0, 0);
		effect.SetSoundAutodestroy(true);
	}
}
