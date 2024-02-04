class FileNode: TreeNode
{
	protected FileSystem m_File;
	
	void FileNode(string uuid, string display_name, Symbols icon, string file)
	{
		m_File = file;
	}
		
	override TreeNodeState GetStateMask()
	{
		return  TreeNodeState.ACTIVE | TreeNodeState.HOVER | TreeNodeState.EXTEND;
	}
		
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
	}
}