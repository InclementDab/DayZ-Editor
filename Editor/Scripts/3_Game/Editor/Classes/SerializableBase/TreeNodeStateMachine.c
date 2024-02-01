enum TreeNodeInteract
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
	TOGGLE, // ying and yang
	
	// For radio buttons
	ONLY, // its parents are constantly angry
};

class TreeNodeState: int
{
	static const int EMPTY = 0x00;
	static const int HOVER = 0x01;
	static const int ACTIVE = 0x02;
	static const int CONTEXT = 0x04;
	static const int DRAGGING = 0x08;
	static const int FOCUS = 0x10;
	//static const int FOCUS = 0x20;
	//static const int FOCUS = 0x40;
		
	bool IsEmpty()
	{
		return value == 0x00;
	}
	
	bool IsHover()
	{
		return (value & HOVER) == HOVER;
	}
	
	bool IsActive()
	{
		return (value & ACTIVE) == ACTIVE;
	}
	
	bool IsContext()
	{
		return (value & CONTEXT) == CONTEXT;
	}	
	
	bool IsDragging()
	{
		return (value & DRAGGING) == DRAGGING;
	}
	
	bool IsFocus()
	{
		return (value & FOCUS) == FOCUS;
	}
}

typedef int TreeNodeState;

class TreeNodeStateMachine: map<int, ref array<TreeNode>>
{
	void TreeNodeStateMachine()
	{
		this[TreeNodeState.EMPTY] = {};
		this[TreeNodeState.HOVER] = {};
		this[TreeNodeState.ACTIVE] = {};
		this[TreeNodeState.CONTEXT] = {};
		this[TreeNodeState.DRAGGING] = {};
		this[TreeNodeState.FOCUS] = {};
	}
		
	void AddAllStates(TreeNodeState state)
	{
		foreach (TreeNodeState node_state, array<TreeNode> nodes: this) {
			if (node_state == state) {
				continue;
			}
			
			foreach (TreeNode node: nodes) {				
				node.AddState(state);
			}
		}
	}
	
	void RemoveAllStates(TreeNodeState state)
	{
		array<TreeNode> nodes = {};
		nodes.Copy(this[state]);
		foreach (TreeNode node: nodes) {
			if (node) {
				node.RemoveState(state);
			}
		}
	}
	
#ifdef DIAG_DEVELOPER
	void Debug()
	{
		foreach (int id, array<TreeNode> nodes: this) {
			Print(typename.EnumToString(TreeNodeState, id));
			foreach (TreeNode node: nodes) {
				PrintFormat("\t%1", node.GetDisplayName());
			}
		}
	}
#endif
}