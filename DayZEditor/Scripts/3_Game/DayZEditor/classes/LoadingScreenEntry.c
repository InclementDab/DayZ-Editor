class LoadingScreenEntry
{
	string File;
	string Creator;
	bool HideLogo;
	
	void LoadingScreenEntry(string file, string creator, bool hide_logo = false)
	{
		File = file; 
		Creator = creator; 
		HideLogo = hide_logo;
	}
}