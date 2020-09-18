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
	
	protected ref array<ref AbstractWave> m_AlarmSounds = {};
	
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
		
		
		
		EntityAI headgear = GetGame().GetPlayer().GetInventory().FindAttachment(InventorySlots.HEADGEAR);
		if (Class.CastTo(m_APSI, headgear)) {
			m_APSI.SwitchOn();
		}		
		
		ref array<vector> alarm_positions = m_Settings.GetAlarmPositions();
		foreach (vector pos: alarm_positions) {
			m_AlarmSounds.Insert(PlayEnvironmentSound(BlowoutSound.Blowout_Alarm, pos, 0.4, 1.2));
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
			float _t = 1000 * Math.RandomFloat(0.5, 2);
			timepassed += _t;
			Sleep(_t);
			
			_t = 1000 * Math.RandomFloat(0.5, 2);
			timepassed += _t;
			float inverse_phase = Math.AbsFloat(pregame_phase - 1);
			inverse_phase *= 100;
			PlayEnvironmentSound(BlowoutSound.Blowout_Voices, RandomizeVector(m_Player.GetPosition(), inverse_phase, inverse_phase + 50), pregame_phase * 0.25, 1.6);
			Sleep(_t);
		}
		
		// Wave phases
		// This is the blowout actually happening
		for (int i = 0; i < m_Settings.WaveCount; i++) {
			thread CreateHit(1 / m_Settings.WaveCount * i);
			Sleep(m_Settings.TimeBetweenWaves * 1000 * Math.RandomFloat(0.7, 1.2));
		}

		
		// Actual Event	
		PlaySoundOnPlayer(BlowoutSound.Blowout_Begin);
		thread CreateBlowout(0.2);
		Sleep(8000);
		for (int j = 0; j < m_Settings.BlowoutCount; j++) {
			
			float phase = (1 / m_Settings.BlowoutCount) * j;
			phase = Math.Clamp(phase, 0.1, FLT_MAX);
			
			for (int k = 0; k < j; k++) {
				PlaySoundOnPlayer(BlowoutSound.Blowout_Begin, phase);
			}
			
			PlaySoundOnPlayer(BlowoutSound.Blowout_Begin, phase);
			thread CreateBlowout(phase);
			Sleep(3000 * Math.RandomFloat(0.7, 1.2));
		}
		
		thread CreateFinalBlowout();
		return;
		// make longer
		Sleep(3000);
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			m_APSI.SwitchOff();
		}
		
		m_Weather.MissionWeather(m_MissionWeatherState);
		thread LerpFunction(GetGame().GetWorld(), "SetEyeAccom", GetGame().GetWorld().GetEyeAccom(), 1, 10);	
		
		Sleep(10000);
		
		foreach (AbstractWave alarm: m_AlarmSounds) {
			if (alarm) {
				alarm.Stop();
			}
		}
	}
	
	// add min and max to this
	private vector RandomizeVector(vector in, float rand)
	{
		for (int i = 0; i < 3; i++)
			in[i] = Math.RandomFloat(in[i] - rand, in[i] + rand);
		
		return in;
	}
	
	private vector RandomizeVector(vector in, float min, float max)
	{
		for (int i = 0; i < 3; i++) {
			in[i] = Math.RandomFloat(in[i] + Math.RandomFloat(-min, min), in[i] + Math.RandomFloat(-max, max));
		}
		
		return in;
	}
	
		
	private void CreateHit(float intensity)
	{	
		Print("CreateHit " + intensity);
		intensity = Math.Clamp(intensity, 0.3, 1);
		intensity *= CalculateIntensity(vector.Distance(m_Player.GetPosition(), m_Position));
		
		
		float phase = intensity;
		phase *= 100;
		vector pos = RandomizeVector(m_Player.GetPosition(), phase, phase + 25);
		PlayEnvironmentSound(BlowoutSound.Blowout_Hit, pos, intensity * 1.4);
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
	
	private void CreateBlowout(float intensity)
	{	
		m_Player.GetStaminaHandler().DepleteStamina(EStaminaModifiers.JUMP);		
		if (m_APSI && m_APSI.IsSwitchedOn()) {
			
		} else {
			
			CreateCameraShake(intensity);
			
			m_MatBlur.LerpParam("Intensity", 0.8 * intensity, m_MatBlur.GetParamValue("Intensity") + 0.12, 0.75);
			m_MatGlow.LerpParam("Vignette", 1.2 * intensity, m_MatBlur.GetParamValue("Vignette") + 0.25, 0.75);
			m_MatChroma.LerpParam("PowerX", 0.3 * intensity, m_MatBlur.GetParamValue("Vignette") + 0.02, 2.5);
		}
	}
	
	private void CreateFinalBlowout()
	{
		m_Player.AddHealth("", "Shock", -m_Settings.ImpactShockDamage);
		PlaySoundOnPlayer(BlowoutSound.Blowout_Begin);
		Sleep(100);
		PlaySoundOnPlayer(BlowoutSound.Blowout_FullWave, 0.25);
		thread CreateBlowout(1);
		
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
		

	private AbstractWave PlaySoundOnPlayer(BlowoutSound sound, float volume = 1)
	{
		SoundObjectBuilder builder = new SoundObjectBuilder(new SoundParams(typename.EnumToString(BlowoutSound, sound)));
		SoundObject sound_object = builder.BuildSoundObject();
		sound_object.SetKind(WaveKind.WAVEENVIRONMENTEX);
		sound_object.SetPosition(m_Player.GetPosition());
		
		AbstractWave wave = GetGame().GetSoundScene().Play2D(sound_object, builder);
		wave.SetVolume(volume);
		wave.Play();
		return wave;
	}
	
	private AbstractWave PlayEnvironmentSound(BlowoutSound sound, vector position, float volume = 1, float frequency_random = 0)
	{
		SoundObjectBuilder builder = new SoundObjectBuilder(new SoundParams(typename.EnumToString(BlowoutSound, sound)));
		SoundObject sound_object = builder.BuildSoundObject();
		sound_object.SetKind(WaveKind.WAVEENVIRONMENTEX);
		sound_object.SetPosition(position);
		
		AbstractWave wave = GetGame().GetSoundScene().Play3D(sound_object, builder);
		wave.SetFrequency(wave.GetFrequency() * Math.RandomFloat(1 - frequency_random, 1 + frequency_random));
		wave.SetVolume(volume);
		wave.Play();
		return wave;
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