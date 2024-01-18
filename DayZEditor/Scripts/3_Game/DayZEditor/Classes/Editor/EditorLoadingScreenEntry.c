class EditorLoadingScreenEntry
{
	string Image;
	string Creator;
	bool HideLogo;
	
	void EditorLoadingScreenEntry(string file, string creator, bool hide_logo = false)
	{
		Image = file; 
		Creator = creator; 
		HideLogo = hide_logo;
	}
}