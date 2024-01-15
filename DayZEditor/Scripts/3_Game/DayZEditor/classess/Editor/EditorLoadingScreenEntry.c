class EditorLoadingScreenEntry
{
	string File;
	string Creator;
	bool HideLogo;
	
	void EditorLoadingScreenEntry(string file, string creator, bool hide_logo = false)
	{
		File = file; 
		Creator = creator; 
		HideLogo = hide_logo;
	}
}