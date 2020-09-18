
class APSI: Clothing
{
	
	void Start()
	{
		if (!IsMissionClient()) return;
		thread _Start();
	}
	
	private void _Start()
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
		GetGame().GetPlayer().PlaySoundSet(effect, typename.EnumToString(BlowoutSound, sound), 0, 0);
		effect.SetSoundAutodestroy(true);
	}
}
