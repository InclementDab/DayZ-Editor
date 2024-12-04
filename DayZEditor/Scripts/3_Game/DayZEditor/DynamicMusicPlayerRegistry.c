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

		array<string> musics = {};
		musics.Copy(EDITOR_MENU_MUSICS);

		for (int i = musics.Count() - 1; i >= 0; i--) {
			int idx = musics.GetRandomIndex();
			RegisterTrackMenu(musics[idx]);
			musics.Remove(idx);
		}
	}
}