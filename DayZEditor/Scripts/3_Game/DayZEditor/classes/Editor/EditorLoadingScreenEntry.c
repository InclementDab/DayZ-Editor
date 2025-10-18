class EditorLoadingScreenEntry
{
	string FileName;
	string Creator;
	bool HideLogo;
	
	void EditorLoadingScreenEntry(string file, string creator, bool hide_logo = false)
	{
		FileName = file; 
		Creator = creator; 
		HideLogo = hide_logo;
	}
}