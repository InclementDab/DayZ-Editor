class EditorLoadingScreenEntry
{
	string MyFile;
	string Creator;
	bool HideLogo;

	void EditorLoadingScreenEntry(string file, string creator, bool hide_logo = false)
	{
		MyFile = file;
		Creator = creator;
		HideLogo = hide_logo;
	}
}