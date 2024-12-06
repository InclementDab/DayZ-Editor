modded class DynamicMusicPlayerRegistry
{
	protected override void RegisterTracksMenu()
	{
		m_TracksMenu = new array<ref DynamicMusicTrackData>();
		
		RegisterTrackMenu("Music_Menu_SoundSet", true);
		RegisterTrackMenu("Music_Menu_2_SoundSet", true);
		RegisterTrackMenu("Music_Menu_3_SoundSet", true);
	}
}