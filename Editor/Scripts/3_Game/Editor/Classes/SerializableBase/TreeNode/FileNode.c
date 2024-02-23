class FileNode: NamedNode
{
	protected FileSystem m_File;
	
	void FileNode(UUID uuid, string display_name, Symbols icon, LinearColor color, string file)
	{
		m_File = file;
	}
		
	override NodeState GetStateMask()
	{
		return  NodeState.ACTIVE | NodeState.EXTEND;
	}
		
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		ToggleState(NodeState.ACTIVE);
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}