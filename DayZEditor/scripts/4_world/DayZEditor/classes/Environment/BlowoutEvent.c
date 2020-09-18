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
	protected PlayerBase m_Player;
	protected APSI m_APSI;
	
	private bool m_MissionWeatherState;
	private Weather m_Weather;
	
	
	
	protected autoptr ref MaterialEffect m_MatGlow;
	protected autoptr ref MaterialEffect m_MatBlur;
	protected autoptr ref MaterialEffect m_MatChroma;
	protected autoptr ref MaterialEffect m_MatColors;
	
	protected ref BlowoutEventSettings m_Settings;
	
	protected vector m_Position;
	
	protected ref array<ref EffectSound> m_AlarmSounds = {};
	
	void BlowoutEvent(BlowoutEventSettings settings)
	{
		m_Settings = settings;
	}
	
	void ~BlowoutEvent()
	{
		Print("BlowoutEvent");
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
		
		PlaySoundOnPlayer(BlowoutSound.Blowout_Begin, 0);
		return;
		
		EntityAI headgear = GetGame().GetPlayer().GetInventory().FindAttachment(InventorySlots.HEADGEAR);
		if (Class.CastTo(m_APSI, headgear)) {
			m_APSI.SwitchOn();
		}		
		
		ref array<vector> alarm_positions = m_Settings.GetAlarmPositions();
		foreach (vector pos: alarm_positions) {
			m_AlarmSounds.Insert(PlayEnvironmentSound(BlowoutSound.Blowout_Alarm, pos, 1, 0, 10));
		}
		
		thread LerpFunction(GetGame().GetWorld(), "SetEyeAccom", 1, 0.07, m_Settings.BlowoutDelay);
		
		Sleep(10000);
		
		m_Weather.SetStorm(0.5, 1, m_Settings.BlowoutDelay);
		m_Weather.GetFog().Set(0.4, m_Settings.BlowoutDelay, m_Settings.BlowoutDelay);
		m_Weather.GetOvercast().Set(1, m_Settings.BlowoutDelay, m_Settings.BlowoutDelay);
			
		
		
		// Pregame Phases
		float timepassed;
		while (timepassed < m_Settings.BlowoutDelay * 1000) {
			
			float pregame_phase = 1 / (m_Settings.BlowoutDelay * 1000) * timepassed;
			//m_Weather.SetStorm(1, 0.4, pregame_phase * 10);
			PlayEnvironmentSound(BlowoutSound.Blowout_Wave, m_Position, pregame_phase);
			float _t = 1000 * Math.RandomFloat(2, 10);
			timepassed += _t;
			Sleep(_t);
			
			_t = 1000 * Math.RandomFloat(2, 10);
			timepassed += _t;
			PlayEnvironmentSound(BlowoutSound.Blowout_Voices, RandomizeVector(m_Player.GetPosition(), 100), 0.1, 2, 2);
			Sleep(_t);
		}
		
		// Wave phases
		// This is the blowout actually happening
		for (int i = 0; i < m_Settings.WaveCount; i++) {
			thread CreateHit(1 / m_Settings.WaveCount * i);
			Sleep(m_Settings.TimeBetweenWaves * 1000 * Math.RandomFloat(0.7, 1.2));
		}

		// Actual Event		
		thread CreateBlowout();
		Sleep(8000);
		for (int j = 0; j < m_Settings.BlowoutCount; j++) {
			
			for (int k = 0; k < j; k++) {
				PlaySoundOnPlayer(BlowoutSound.Blowout_Begin);
			}
			
			thread CreateBlowout();
			Sleep(3000 * Math.RandomFloat(0.7, 1.2));
		}
		
		thread CreateFinalBlowout();
		
		// make longer
		Sleep(3000);
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			m_APSI.SwitchOff();
		}
		
		m_Weather.MissionWeather(m_MissionWeatherState);
		thread LerpFunction(GetGame().GetWorld(), "SetEyeAccom", GetGame().GetWorld().GetEyeAccom(), 1, 10);	
		
		Sleep(10000);	
		
		foreach (EffectSound alarm: m_AlarmSounds) {
			if (alarm) {
				alarm.SoundStop();
			}
		}
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
		intensity = Math.Clamp(intensity, 0.1, 1);
		intensity *= CalculateIntensity(vector.Distance(m_Player.GetPosition(), m_Position));
		
		vector pos = RandomizeVector(m_Player.GetPosition(), 100);
		
		PlayEnvironmentSound(BlowoutSound.Blowout_Hit, pos, intensity);
		Sleep(vector.Distance(pos, m_Player.GetPosition()) * 0.343);
		
		
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
		PlaySoundOnPlayer(BlowoutSound.Blowout_Begin);
		
		m_Player.GetStaminaHandler().DepleteStamina(EStaminaModifiers.JUMP);		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
		} else {
			
			CreateCameraShake(1);
			
			m_MatBlur.LerpParam("Intensity", 0.8, 0.1, 0.75);
			m_MatGlow.LerpParam("Vignette", 0.9, 0.25, 0.75);
			m_MatChroma.LerpParam("PowerX", 2.5, 0, 1);
			/*
			m_MatBlur.LerpParam("Intensity", 0.8, 0.1, 0.75);
			m_MatGlow.LerpParam("Vignette", 0.9, 0.25, 0.75);
			m_MatChroma.LerpParamTo("PowerX", 0.75, 0.5);
			Sleep(500);
			m_MatChroma.LerpParamTo("PowerX", -0.3, 0.35);
			Sleep(500);		
			m_MatChroma.LerpParamTo("PowerX", -0.2, 0.35);
			Sleep(500);	
			m_MatChroma.LerpParamTo("PowerX", 0, 0.5);*/
		}
	}
	
	private void CreateFinalBlowout()
	{
		m_Player.AddHealth("", "Shock", -m_Settings.ImpactShockDamage);
		PlaySoundOnPlayer(BlowoutSound.Blowout_Begin, 1.5);
		PlaySoundOnPlayer(BlowoutSound.Blowout_FullWave);
		CreateBlowout();
		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
		} else {
			if (m_Player.m_Environment.IsSafeFromEVR()) {		
				return;
			}
			
			m_Player.StartCommand_Unconscious(0);
			PPEffects.SetUnconsciousnessVignette(true);
		}
	}
	
	private float CalculateIntensity(float distance)
	{
		float x = (1 / m_Settings.BlowoutSize) * distance;
		return Math.Clamp(x, 0.1, 1);
	}
	
	private void CreateCameraShake(float intensity)
	{
		m_Player.GetCurrentCamera().SpawnCameraShake(Math.Clamp(intensity, 0.2, 1), 2, 5, 3.5);
	}
		

	private void PlaySoundOnPlayer(BlowoutSound sound, float volume = 1)
	{
		//EffectSound effect = SEffectManager.CreateSound(typename.EnumToString(BlowoutSound, sound), m_Player.GetPosition());
		EffectSound effect;
		
		SoundParams p = new SoundParams(typename.EnumToString(BlowoutSound, sound));
		Print(p.IsValid());
		SoundObjectBuilder builder = new SoundObjectBuilder(p);
		SoundObject so = builder.BuildSoundObject();
		so.SetKind(WaveKind.WAVEENVIRONMENTEX);
		so.SetPosition(m_Player.GetPosition());
		
		
		AbstractWave wave = GetGame().GetSoundScene().Play2D(so, builder);
		
		wave.SetVolume(1);
		wave.Play();
		
		
		
		//SoundOnVehicle v = m_Player.PlaySound(typename.EnumToString(BlowoutSound, sound), 1);
		
		//effect.SetSoundVolume(volume);
		//effect.SoundPlay();
	
		
	}
	
	private EffectSound PlayEnvironmentSound(BlowoutSound sound, vector position, float volume = 1, float fadein = 0, float fadeout = 0)
	{
		EffectSound effect = SEffectManager.CreateSound(typename.EnumToString(BlowoutSound, sound), position, fadein, fadeout, false, true);
		effect.SetSoundAutodestroy(true);
		effect.SetVolumeLevel(volume);
		effect.SoundPlay();
		return effect;
	}
	
	
	void LerpFunction(Class inst, string function, float start, float finish, float duration)
	{		
		int i = 0;
		while (i < duration * 1000) {
			g_Script.CallFunctionParams(inst, function, null, new Param1<float>(Math.Lerp(start, finish, (1 / duration) * i / 1000)));
			Sleep(10);
			i += 10;
		}
	}
	

}