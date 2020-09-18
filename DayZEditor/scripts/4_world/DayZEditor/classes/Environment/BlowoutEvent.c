enum BlowoutSound
{
	Blowout_Begin,
	Blowout_FullWave,
	Blowout_Voices,
	Blowout_Hit,
	Blowout_Wave,
	APSI_Enable,
	APSI_Detect,
	APSI_Disable
}


class MaterialEffect
{
	private Material m_Material;
	
	private ref map<string, float> param_values = new map<string, float>();
	
	void MaterialEffect(string material)
	{
		m_Material = GetGame().GetWorld().GetMaterial(material);
	}
	
	void ~MaterialEffect()
	{
		foreach (string name, float value: param_values) {
			m_Material.ResetParam(name);
		}
	}
	
	private void _LerpParam(string param, float start, float finish, float duration)
	{
		int i = 0;
		while (i < duration * 1000) {
			m_Material.SetParam(param, Math.Lerp(start, finish, (1 / duration) * i / 1000));
			Sleep(1);
			i += 10;
		}
		param_values.Set(param, finish);
	}
	
	void LerpParam(string param, float start, float finish, float duration)
	{
		thread _LerpParam(param, start, finish, duration);
	}
	
	void LerpParamTo(string param, float finish, float duration)
	{
		float start;
		if (!param_values.Find(param, start)) {
			param_values.Insert(param, 0);
		}
		
		LerpParam(param, start, finish, duration);
	}
}


class BlowoutEvent
{
	protected PlayerBase m_Player;
	
	protected autoptr MaterialEffect m_MatGlow;
	protected autoptr MaterialEffect m_MatBlur;
	protected autoptr MaterialEffect m_MatChroma;
		
	void StartBlowout(PlayerBase player)
	{
		thread StartBlowoutClient(player, player.GetPosition());
	}
	
	void StartBlowoutClient(PlayerBase player, vector position)
	{
		m_Player = player;
		
		InitEffects();
		
		bool APSI_Enable = true; // m_Player contains an APSI Unit
		if (APSI_Enable) {
			thread StartAPSI();
		}
		
		//Sleep(5000);
		//thread GroundShake(1);
		m_Player.GetCurrentCamera().SpawnCameraShake(1);
	
		
		// Effects Wave 1
		PlayEnvironmentSound(BlowoutSound.Blowout_Begin, position);
		m_MatBlur.LerpParam("Intensity", 0.8, 0.1, 0.75);
		m_MatGlow.LerpParam("Vignette", 0.9, 0.25, 0.75);
		m_MatChroma.LerpParamTo("PowerX", 0.25, 0.5);
		Sleep(500);		
		m_MatChroma.LerpParamTo("PowerX", -0.15, 0.35);
		Sleep(500);		
		m_MatChroma.LerpParamTo("PowerX", -0.05, 0.2);
		Sleep(500);		
		m_MatChroma.LerpParamTo("PowerX", 0, 0.2);
		Sleep(500);
		
	}
	
	private void InitEffects()
	{
		m_MatBlur = new MaterialEffect("graphics/materials/postprocess/gauss");
		m_MatGlow = new MaterialEffect("graphics/materials/postprocess/glow");
		m_MatChroma = new MaterialEffect("graphics/materials/postprocess/chromaber");
	}
	
	void GroundShake(float intensity)
	{
		m_Player.GetCurrentCamera().SpawnCameraShake(intensity);
	}
	
	void StartAPSI()
	{
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(200);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(500);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(100);
		PlaySoundOnPlayer(BlowoutSound.APSI_Detect);
		Sleep(2000);
		PlaySoundOnPlayer(BlowoutSound.APSI_Enable);
	}
	
	private void PlaySoundOnPlayer(BlowoutSound sound)
	{
		EffectSound effect;
		m_Player.PlaySoundSet(effect, typename.EnumToString(BlowoutSound, sound), 0, 0);
		effect.SetSoundAutodestroy(true);
	}
	
	private void PlayEnvironmentSound(BlowoutSound sound, vector position)
	{
		EffectSound effect = SEffectManager.CreateSound(typename.EnumToString(BlowoutSound, sound), position, 0, 0, false, true);
		effect.SetSoundAutodestroy(true);
		effect.SoundPlay();
	}
	
	

}