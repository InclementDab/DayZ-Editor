class FileNode: TreeNode
{
	protected FileSystem m_File;
	
	void FileNode(string uuid, string display_name, Symbols icon, string file)
	{
		m_File = file;
		
		if (!m_File.Exists()) {
			Error(string.Format("File not found %1", m_File));
		}
		
		switch (m_File.GetExtension()) {
			
			case ".xml": {
				m_Icon = Symbols.FILE_XML;
				break;
			}
			
			case ".json": {
				m_Icon = Symbols.FILE_CODE;
				break;
			}
			
			case string.Empty: {
				Directory directory = m_File;
				array<string> files = directory.EnumerateFiles();
				foreach (string f: files) {
					string file_formatted = f;
					file_formatted.Replace(m_File, "");
					Add(new FileNode(UUID.Generate(), file_formatted, Symbols.FILE, f));
				}
				break;
			}
			
			default: {
				m_Icon = Symbols.FILE;
				break;
			}
		}
	}
		
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.ACTIVE | TreeNodeState.HOVER | TreeNodeState.EXTEND;
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}