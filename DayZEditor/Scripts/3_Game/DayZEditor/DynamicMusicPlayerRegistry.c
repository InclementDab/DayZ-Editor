modded class DynamicMusicPlayerRegistry
{
	protected override void RegisterTracksMenu()
	{
		m_TracksMenu = new array<ref DynamicMusicTrackData>();
		
		RegisterTrackMenu("Music_Menu_SoundSet", true);
		RegisterTrackMenu("Music_Menu_2_SoundSet", true);
		RegisterTrackMenu("Music_Menu_3_SoundSet", true);
	}

	protected override void RegisterTracksTime()
	{
		m_TracksTime = new array<ref DynamicMusicTrackData>();
	}

	protected override void RegisterTracksLocationStatic()
	{
		m_TracksLocationStatic = new array<ref DynamicMusicTrackData>();
		m_TracksLocationStaticPrioritized = new array<ref DynamicMusicTrackData>();
	}

	protected override void RegisterTracksLocationDynamic()
	{
		m_TracksLocationDynamic = new array<ref DynamicMusicTrackData>();
	}
}

modded class DynamicMusicPlayer
{
	protected override void DetermineTrackByCategory(EDynamicMusicPlayerCategory category)
	{
		if (category == EDynamicMusicPlayerCategory.MENU) {
			super.DetermineTrackByCategory(category);
		}
	}
}