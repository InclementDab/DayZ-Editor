enum BlowoutSound
{
	Blowout_Begin,
	Blowout_FullWave,
	Blowout_Voices,
	Blowout_Hit,
	Blowout_Wave,
	Blowout_Drone,
	APSI_Enable,
	APSI_Detect,
	APSI_Disable
}


class BlowoutEvent
{
	protected APSI m_APSI;
	
	protected ref MaterialEffect m_MatGlow;
	protected ref MaterialEffect m_MatBlur;
	protected ref MaterialEffect m_MatChroma;
	
	protected ref BlowoutEventSettings m_Settings;
	
	protected vector m_Position;
	
	void BlowoutEvent(BlowoutEventSettings settings)
	{
		m_Settings = settings;
	}
	
	void StartBlowout(vector position)
	{
		m_Position = position;
		

		
		thread StartBlowoutClient();
	}
	
	
	void StartBlowoutClient()
	{
		if (!IsMissionClient()) return;
		
		// Init stuff
		m_MatBlur = new MaterialEffect("graphics/materials/postprocess/gauss");
		m_MatGlow = new MaterialEffect("graphics/materials/postprocess/glow");
		m_MatChroma = new MaterialEffect("graphics/materials/postprocess/chromaber");
		
		EntityAI headgear = GetGame().GetPlayer().GetInventory().FindAttachment(InventorySlots.HEADGEAR);
		if (Class.CastTo(m_APSI, headgear)) {
			m_APSI.SwitchOn();
		}		
		
		GetGame().GetWeather().GetOvercast().Set(1, 5, 30);
		
		Sleep(5000);
		
		// Wave phases
		for (int i = 0; i < m_Settings.WaveCount; i++) {
			PlayEnvironmentSound(BlowoutSound.Blowout_Wave, m_Position);
			CreateImpact(1 / m_Settings.WaveCount * i);
			Sleep(m_Settings.TimeBetweenWaves * 1000);
		}
		
		Sleep(5000);
		
		// Actual Event
		PlayEnvironmentSound(BlowoutSound.Blowout_Begin, m_Position);
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
			
			m_MatBlur.LerpParam("Intensity", 0.1, 0, 0.75);
			CreateImpact(0.3);
			
			
		} else {			
			
			m_MatBlur.LerpParam("Intensity", 0.8, 0.1, 0.75);
			m_MatGlow.LerpParam("Vignette", 0.9, 0.25, 0.75);
			CreateImpact(1);
		}
		
		Sleep(3000);
		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			m_APSI.SwitchOff();
		}
		
		// temporary
		GetGame().GetWeather().GetOvercast().Set(0, 0, 30);
	}
		
	private void CreateImpact(float intensity)
	{	
		Print("Creating Impact " + intensity);
		intensity *= m_Settings.Intensity;
		
		GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake(intensity * 2, 2, 5, 1);
		m_MatBlur.LerpParam("Intensity", intensity, 0, 0.75);
		m_MatChroma.LerpParamTo("PowerX", 0.5 * intensity , 0.5 * intensity);
		Sleep(500);
		m_MatChroma.LerpParamTo("PowerX", -0.3 * intensity, 0.35 * intensity);
		Sleep(500);		
		m_MatChroma.LerpParamTo("PowerX", -0.1 * intensity, 0.2 * intensity);
		Sleep(500);		
		m_MatChroma.LerpParamTo("PowerX", 0 * intensity, 0.2 * intensity);
		Sleep(5000);
	}
		

	private void PlaySoundOnPlayer(BlowoutSound sound)
	{
		EffectSound effect;
		GetGame().GetPlayer().PlaySoundSet(effect, typename.EnumToString(BlowoutSound, sound), 0, 0);
		effect.SetSoundAutodestroy(true);
	}
	
	private void PlayEnvironmentSound(BlowoutSound sound, vector position)
	{
		EffectSound effect = SEffectManager.CreateSound(typename.EnumToString(BlowoutSound, sound), position, 0, 0, false, true);
		effect.SetSoundAutodestroy(true);
		effect.SoundPlay();
	}
	
	

}