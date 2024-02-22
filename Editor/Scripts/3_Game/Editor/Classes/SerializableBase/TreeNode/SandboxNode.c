enum SandboxNodeInteract
{
	NONE, // darn :(
	
	// The button was pressed, you will have to manually unpress it
	ONCE,
	
	// The button was pressed, it will be cleaned up when you press off of it
	PRESS,
	
	// Button will have to be double clicked
	DOUBLE, // woah
	
	// button MUST be held
	HOLD, // EEEEEK like a poop?
	
	// Button must be in toggle mode lol
	TOGGLE // ying and yang
};

modded class Node
{
	EditorNode GetEditor()
	{
		return EditorNode.Cast(FindAncestor(EditorNode));
	}	
	
	void Node(UUID uuid)
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(Update);
	}
	
	bool CreateContextMenu(inout ObservableCollection<ref MenuNode> list_items)
	{
		//list_items.Insert(new MenuNode(GetDayZGame().GetSandbox().GetCommand("Rename")));
		return true;
	}
	
	void Update(float dt)
	{
		
	}
	
	override void AddState(NodeState state)
	{
		if ((GetStateMask() & state) != 0) {
			super.AddState(state);
		}
	}
	
	override void RemoveState(NodeState state)
	{		
		if ((GetStateMask() & state) != 0) {
			super.RemoveState(state);
		}
	}
	
	SandboxNodeInteract GetInteractType()
	{	
		return SandboxNodeInteract.PRESS;
	}
	
	NodeState GetStateMask()
	{
		return NodeState.HOVER | NodeState.ACTIVE | NodeState.CONTEXT | NodeState.DRAGGING | NodeState.SUPPRESSED;
	}
	
	array<string> GetXorSelections()
	{
		return {};
	}
	
	override NodeState GetDefaultState()
	{
		return NodeState.ACTIVE;
	}
}