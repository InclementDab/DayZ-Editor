
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


modded class Environment
{
	protected float m_EVRStrength = 0.5;
	
	void StartEVR()
	{
		if (!m_Player) return;
		
		bool APSI_Enable = true; // todo
		if (APSI_Enable) {
			thread StartAPSI();
		}
		
		// temp setting blowout to ontop of player
		thread StartBlowout(m_Player.GetPosition());
	}
	
	private void StartBlowout(vector position)
	{
		PlayEnvironmentSound(BlowoutSound.Blowout_Begin, position);
		thread LerpBlur(0.8, 0.1, 0.75);
		Sleep(500);
	}
	
	private void StartAPSI()
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
	
	private void LerpBlur(float start, float finish, float duration)
	{
		PPEffects.SetBlur(start);
		
		int i = 0;
		while (i < duration * 1000) {
			PPEffects.SetBlur(Math.Lerp(start, finish, (1 / duration) * i / 1000));
			Sleep(1);
			i += 10;
		}
		
		PPEffects.SetBlur(finish);
	}
}
