modded class DynamicMusicPlayerRegistry
{
	static const ref array<string> EDITOR_MENU_MUSICS = {
		"Music_Menu_SoundSet",
		"Music_Menu2_SoundSet",
		"Music_Menu3_SoundSet"
	};

	protected override void RegisterTracksMenu()
	{
		m_TracksMenu = new array<ref DynamicMusicTrackData>();

		RegisterTrackMenu("Music_Menu_SoundSet");
		RegisterTrackMenu("Music_Menu2_SoundSet");
		RegisterTrackMenu("Music_Menu3_SoundSet");
	}
}