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
	protected ref MaterialEffect m_MatColors;
	
	protected ref BlowoutEventSettings m_Settings;
	
	protected vector m_Position;
	
	protected PlayerBase m_Player;
	
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
		m_Player = GetGame().GetPlayer();
		
		// Init stuff
		m_MatBlur = new MaterialEffect("graphics/materials/postprocess/gauss");
		m_MatGlow = new MaterialEffect("graphics/materials/postprocess/glow");
		m_MatChroma = new MaterialEffect("graphics/materials/postprocess/chromaber");
		m_MatColors = new MaterialEffect("graphics/materials/postprocess/colors");
		
		EntityAI headgear = GetGame().GetPlayer().GetInventory().FindAttachment(InventorySlots.HEADGEAR);
		if (Class.CastTo(m_APSI, headgear)) {
			m_APSI.SwitchOn();
		}		
		
		float distance_to_storm = vector.Distance(m_Player.GetPosition(), m_Position);
		
		GetGame().GetWeather().SetStorm(1, 0.2, 3);
		GetGame().GetWeather().GetFog().Set(0.4, m_Settings.BlowoutDelay, m_Settings.BlowoutDelay);
		GetGame().GetWeather().GetOvercast().Set(1, m_Settings.BlowoutDelay, m_Settings.BlowoutDelay);
		
		// Dont compute rain until AFTER storm is done
		GetGame().GetWeather().GetRain().SetNextChange(m_Settings.BlowoutDelay);
		
		// Pregame Phases
		PlaySoundOnPlayer(BlowoutSound.Blowout_Voices);
		int timepassed;
		while (timepassed < m_Settings.BlowoutDelay * 1000) {
			float pregame_phase = 1 / m_Settings.BlowoutDelay * 1000 * timepassed;
			PlayEnvironmentSound(BlowoutSound.Blowout_Wave, m_Position, pregame_phase);
			int _t = 1000 * Math.RandomFloat(0.5, 5);
			timepassed += _t;
			Sleep(_t);
		}	
		
		
		
		// Wave phases
		// This is the blowout actually happening
		for (int i = 0; i < m_Settings.WaveCount; i++) {
			PlayEnvironmentSound(BlowoutSound.Blowout_Hit, m_Position, m_Settings.Intensity);
			
			// Wait for speed of sound to catch up
			Sleep(distance_to_storm / 0.343);
			if (m_APSI && m_APSI.IsSwitchedOn()) {
				
			} else {	
				CreateImpact(1 / m_Settings.WaveCount * i);
			}
			
			Sleep(m_Settings.TimeBetweenWaves * 1000 * Math.RandomFloat(0.7, 1.2));
		}
		
		Sleep(3000);
		
		// Actual Event
		PlayEnvironmentSound(BlowoutSound.Blowout_Begin, m_Position, m_Settings.Intensity);
		Sleep(distance_to_storm / 0.343);
		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
			m_MatBlur.LerpParam("Intensity", 0.2, 0, 0.75);
			CreateCameraShake(0.3);

		} else {			
					

			m_MatBlur.LerpParam("Intensity", 0.8, 0.1, 0.75);
			m_MatGlow.LerpParam("Vignette", 0.9, 0.25, 0.75);
			CreateImpact(1);
			// adds xtra spice
			m_Player.GetStaminaHandler().DepleteStamina(EStaminaModifiers.JUMP);
		}
		
		Sleep(3000);
		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			m_APSI.SwitchOff();
		}
	}
		
	private void CreateImpact(float intensity)
	{	
		Print("Creating Impact " + intensity);
		intensity *= m_Settings.Intensity;
		
		m_Player.GetStaminaHandler().DepleteStamina(EStaminaModifiers.JUMP);
		m_Player.AddHealth("", "Shock", -m_Settings.ImpactShockDamage);
		
		CreateCameraShake(intensity);
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
	
	private void CreateCameraShake(float intensity)
	{
		intensity = Math.Clamp(intensity, 0.1, 1);
		m_Player.GetCurrentCamera().SpawnCameraShake(intensity * 2, 2, 5, 1);
	}
		

	private void PlaySoundOnPlayer(BlowoutSound sound)
	{
		EffectSound effect;
		GetGame().GetPlayer().PlaySoundSet(effect, typename.EnumToString(BlowoutSound, sound), 0, 0);
		effect.SetSoundAutodestroy(true);
	}
	
	private void PlayEnvironmentSound(BlowoutSound sound, vector position, float volume = 1)
	{
		EffectSound effect = SEffectManager.CreateSound(typename.EnumToString(BlowoutSound, sound), position, 0, 0, false, true);
		effect.SetSoundAutodestroy(true);
		effect.SetSoundVolume(volume);
		effect.SoundPlay();
	}
	
	

}