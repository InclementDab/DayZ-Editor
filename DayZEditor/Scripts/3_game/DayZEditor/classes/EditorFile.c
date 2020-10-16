class EditorFile
{
	string FileName;
	string FileDirectory;
	FileSearchMode FileAtrributes;
	
	void EditorFile(string name, string directory, FileSearchMode attributes) {
		FileName = name; FileDirectory = directory; FileAtrributes = attributes;
	}
	
	string GetFile() {
		return string.Format("%1\\%2", FileDirectory, FileName);
    }
}
