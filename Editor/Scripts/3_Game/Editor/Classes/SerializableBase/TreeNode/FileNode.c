class FileNode: NamedNode
{
	protected FileSystem m_File;
	
	void FileNode(UUID uuid, string display_name, Symbols icon, LinearColor color, string file)
	{
		m_File = file;
	}
		
	override NodeState GetStateMask()
	{
		return  NodeState.ACTIVE | NodeState.HOVER | NodeState.EXTEND;
	}
		
	override SandboxNodeInteract GetInteractType()
	{
		return SandboxNodeInteract.PRESS;
	}
}