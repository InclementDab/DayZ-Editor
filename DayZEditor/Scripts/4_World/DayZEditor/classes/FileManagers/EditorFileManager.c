class EditorFileManager
{
	static void GetSafeFileName(out string file_name, string extension)
	{
		TStringArray file_split = {};
		file_name.Split(".", file_split);
		if (file_split.Count() == 1 || file_split[1] != extension) {
			file_name = file_split[0] + extension;
		}
	}
}