modded class EffectSound
{
	void SetVolumeLevel(float volume) 
	{
		if (m_SoundWaveObject)
			m_SoundWaveObject.SetVolume(volume);
	}
}