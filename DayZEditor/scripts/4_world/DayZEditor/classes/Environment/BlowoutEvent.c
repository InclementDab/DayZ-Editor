enum BlowoutSound
{
	Blowout_Begin,
	Blowout_FullWave,
	Blowout_Voices,
	Blowout_Hit,
	Blowout_Wave,
	Blowout_Drone,
	Blowout_Alarm,
	APSI_Enable,
	APSI_Detect,
	APSI_Disable
}


class BlowoutEvent
{
	private bool m_MissionWeatherState;
	private Weather m_Weather;
	
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
		m_Weather = GetGame().GetWeather();
		
		// Init stuff
		m_MatBlur = new MaterialEffect("graphics/materials/postprocess/gauss");
		m_MatGlow = new MaterialEffect("graphics/materials/postprocess/glow");
		m_MatChroma = new MaterialEffect("graphics/materials/postprocess/chromaber");
		m_MatColors = new MaterialEffect("graphics/materials/postprocess/colors");
		m_MissionWeatherState = m_Weather.GetMissionWeather();
		m_Weather.MissionWeather(false);
		
		thread CreateHit(1);
		return;
		EntityAI headgear = GetGame().GetPlayer().GetInventory().FindAttachment(InventorySlots.HEADGEAR);
		if (Class.CastTo(m_APSI, headgear)) {
			m_APSI.SwitchOn();
		}		
		
		ref array<vector> alarm_positions = m_Settings.GetAlarmPositions();
		foreach (vector pos: alarm_positions) {
			PlayEnvironmentSound(BlowoutSound.Blowout_Alarm, pos);
		}
		
		Sleep(10000);
		
		thread LerpFunction(GetGame().GetWorld(), "SetEyeAccom", 1, 0.06, m_Settings.BlowoutDelay);
		//m_Weather.SetStorm(1, 0.4, m_Settings.BlowoutDelay);
		m_Weather.GetFog().Set(0.4, m_Settings.BlowoutDelay, m_Settings.BlowoutDelay);
		m_Weather.GetOvercast().Set(1, m_Settings.BlowoutDelay, m_Settings.BlowoutDelay);
			
		
		
		// Pregame Phases
		float timepassed;
		while (timepassed < m_Settings.BlowoutDelay * 1000) {
			
			float pregame_phase = 1 / (m_Settings.BlowoutDelay * 1000) * timepassed;
			//m_Weather.SetStorm(1, 0.4, pregame_phase * 10);
			PlayEnvironmentSound(BlowoutSound.Blowout_Wave, m_Position, pregame_phase);
			float _t = 1000 * Math.RandomFloat(0.5, 5);
			timepassed += _t;
			Sleep(_t);
			
			_t = 1000 * Math.RandomFloat(0.5, 5);
			timepassed += _t;
			PlayEnvironmentSound(BlowoutSound.Blowout_Voices, RandomizeVector(m_Player.GetPosition(), 100), 0.1);
			Sleep(_t);
		}	
		
		// Wave phases
		// This is the blowout actually happening
		for (int i = 0; i < m_Settings.WaveCount; i++) {
			thread CreateHit(1 / m_Settings.WaveCount * i);
			Sleep(m_Settings.TimeBetweenWaves * 1000 * Math.RandomFloat(0.7, 1.2));
		}

		// Actual Event		
		for (int j = 0; j < m_Settings.BlowoutCount; j++) {
			thread CreateBlowout();
			Sleep(2000 * Math.RandomFloat(0.7, 1.2));
		}
		
		thread CreateFinalBlowout();
		
		// make longer
		Sleep(3000);
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			m_APSI.SwitchOff();
		}
		
		m_Weather.MissionWeather(m_MissionWeatherState);
		thread LerpFunction(GetGame().GetWorld(), "SetEyeAccom", 1, 1, 10);
		delete m_MatGlow;
		delete m_MatBlur;
		delete m_MatColors;
		delete m_MatChroma;
		
	}
	
	private vector RandomizeVector(vector in, float rand)
	{
		for (int i = 0; i < 3; i++)
			in[i] = Math.RandomFloat(in[i] - rand, in[i] + rand);
		
		return in;
	}
		
	private void CreateHit(float intensity)
	{	
		Print("CreateHit " + intensity);
		intensity *= CalculateIntensity(vector.Distance(m_Player.GetPosition(), m_Position));
		
		PlayEnvironmentSound(BlowoutSound.Blowout_Hit, RandomizeVector(m_Player.GetPosition(), 100), intensity);
		
		
		m_Player.GetStaminaHandler().DepleteStamina(EStaminaModifiers.JUMP);
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
		} else {
			
			CreateCameraShake(intensity * 3);
			m_MatBlur.LerpParam("Intensity", 0.4 * intensity, 0.1, 0.75);
			m_MatGlow.LerpParam("Vignette", 0.4 * intensity, 0, 0.75);
			m_MatChroma.LerpParam("PowerX", 1.5 * intensity, 0, 1);
		}
	}
	
	private void CreateBlowout()
	{	
		PlaySoundOnPlayer(BlowoutSound.Blowout_Begin, m_Settings.Intensity);
		
		m_Player.GetStaminaHandler().DepleteStamina(EStaminaModifiers.JUMP);		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
		} else {
			
			CreateCameraShake(m_Settings.Intensity);
			m_MatBlur.LerpParam("Intensity", 0.8, 0.1, 0.75);
			m_MatGlow.LerpParam("Vignette", 0.9, 0.25, 0.75);
			m_MatChroma.LerpParamTo("PowerX", 0.75, 0.5);
			Sleep(500);
			m_MatChroma.LerpParamTo("PowerX", -0.3, 0.35);
			Sleep(500);		
			m_MatChroma.LerpParamTo("PowerX", -0.2, 0.35);
			Sleep(500);	
			m_MatChroma.LerpParamTo("PowerX", 0, 0.5);
		}
	}
	
	private void CreateFinalBlowout()
	{
		m_Player.AddHealth("", "Shock", -m_Settings.ImpactShockDamage);
		PlaySoundOnPlayer(BlowoutSound.Blowout_Begin);
		PlaySoundOnPlayer(BlowoutSound.Blowout_FullWave);
		CreateBlowout();
		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
		} else {
			//m_Player.StartCommand_Unconscious(0);
		}

	}
	
	private float CalculateIntensity(float distance)
	{
		float x = (1 / m_Settings.BlowoutSize) * distance;
		return Math.Clamp(x, 0.1, 1);
	}
	
	private void CreateCameraShake(float intensity)
	{
		intensity = Math.Clamp(intensity, 0.1, 1);
		m_Player.GetCurrentCamera().SpawnCameraShake(intensity, 2, 5, 2);
	}
		

	private void PlaySoundOnPlayer(BlowoutSound sound, float volume = 1)
	{
		EffectSound effect = SEffectManager.CreateSound(typename.EnumToString(BlowoutSound, sound), m_Player.GetPosition());
		effect.SetSoundAutodestroy(true);
		effect.SetParent(m_Player);
		effect.SetVolumeLevel(volume);
		effect.SoundPlay();
	}
	
	private void PlayEnvironmentSound(BlowoutSound sound, vector position, float volume = 1)
	{
		EffectSound effect = SEffectManager.CreateSound(typename.EnumToString(BlowoutSound, sound), position, 0, 0, false, true);
		effect.SetSoundAutodestroy(true);
		effect.SetVolumeLevel(volume);
		effect.SoundPlay();
	}
	
	
	void LerpFunction(Class inst, string function, float start, float finish, float duration)
	{		
		int i = 0;
		while (i < duration * 1000) {
			g_Script.CallFunctionParams(inst, function, null, new Param1<float>(Math.Lerp(start, finish, (1 / duration) * i / 1000)));
			Sleep(1);
			i += 10;
		}
	}
	

}