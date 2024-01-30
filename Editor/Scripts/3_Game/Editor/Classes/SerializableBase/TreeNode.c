enum TreeNodeInteract
{
	NONE, // darn :(
	PRESS, // 100ms
	DOUBLE, // woah
	HOLD, // EEEEEK like a poop?
	TOGGLE // ying and yang
};

class TreeNodeState: int
{
	static const TreeNodeState EMPTY = 0x0;
	static const TreeNodeState HOVER = 0x1;
	static const TreeNodeState ACTIVE = 0x2;
	static const TreeNodeState CONTEXT = 0x4;
	
	bool IsEmpty()
	{
		return value == 0x0;
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
	}
		
	void SetAllStates(TreeNodeState state)
	{
		foreach (TreeNodeState node_state, array<TreeNode> nodes: this) {
			if (node_state == state) {
				continue;
			}
			
			foreach (TreeNode node: nodes) {				
				node.SetState(state);
			}
		}
	}
}

class TreeNodeChildren: map<string, ref TreeNode>
{
	void SetAllStates(TreeNodeState state, bool recursively = false)
	{
		foreach (string uuid, TreeNode node: this) {
			if (recursively) {
				node.Children.SetAllStates(state, recursively);
			}
			
			if (node.GetState() == state) {
				continue;
			}
						
			node.SetState(state);
		}
	}
}

class TreeNode: SerializableBase
{
	static const string PATH_SEPERATOR = "\\";
	static ref TreeNodeStateMachine StateMachine = new TreeNodeStateMachine();
		
	protected string m_UUID;
	protected TreeNodeInteract m_TreeNodeInteract = TreeNodeInteract.NONE;
	protected TreeNodeState m_TreeNodeState = GetDefaultState();
	ref TreeNodeChildren Children = new TreeNodeChildren();

	protected string m_Icon, m_DisplayName;
	protected TreeNode m_Parent;
	
	protected ref TreeView m_NodeView;
		
	ref ScriptInvoker State_OnChanged = new ScriptInvoker();
	
	void TreeNode(string uuid, string display_name, Symbols icon)
	{
		m_UUID = uuid;
		m_DisplayName = display_name;
		m_Icon = icon;

		StateMachine[m_TreeNodeState].Insert(this);
	}
	
	void ~TreeNode()
	{
		StateMachine[m_TreeNodeState].RemoveItem(this);
		delete m_NodeView;
	}
	
	void ToggleState()
	{
		switch (m_TreeNodeState) {
			case TreeNodeState.ACTIVE: {
				SetState(TreeNodeState.EMPTY);
				break;
			}
			
			case TreeNodeState.HOVER:
			case TreeNodeState.EMPTY: {
				SetState(TreeNodeState.ACTIVE);
				break;
			}
		}
	}
	
	bool SetState(TreeNodeState state)
	{
		if (GetValidStates().Find(state) == -1) {
			PrintFormat("[%1] Could not change state %2 => %3", m_UUID, typename.EnumToString(TreeNodeState, m_TreeNodeState), typename.EnumToString(TreeNodeState, state));
			return false;
		}
		
		StateMachine[m_TreeNodeState].RemoveItem(this);	
		StateMachine[state].Insert(this);
		
		m_TreeNodeState = state;		
		State_OnChanged.Invoke(m_TreeNodeState);
		return true;
	}
	
	bool AddState(TreeNodeState state)
	{
		if (GetValidStates().Find(state) == -1) {
			PrintFormat("[%1] Could not add state %2", m_UUID, typename.EnumToString(TreeNodeState, state));
			return false;
		}
		
		StateMachine[state].Insert(this);
		
		m_TreeNodeState |= state;		
		State_OnChanged.Invoke(m_TreeNodeState);
		return true;
	}
	
	void RemoveState(TreeNodeState state)
	{
		StateMachine[state].RemoveItem(this);	
		Print(m_TreeNodeState);
		m_TreeNodeState &= ~state;
		Print(m_TreeNodeState);
		State_OnChanged.Invoke(m_TreeNodeState);
	}
	
	bool HasState(TreeNodeState state)
	{
		return (m_TreeNodeState & state) == state;
	}
	
	TreeNodeState GetState()
	{
		return m_TreeNodeState;
	}
	
	bool CreateContextMenu(inout ObservableCollection<ref ScriptView> list_items)
	{
		return false;
	}
			
	void Synchronize(PlayerIdentity identity = null)
	{	
		ScriptRPC rpc = new ScriptRPC();
		int tree_depth = GetParentDepth();
		rpc.Write(tree_depth);

		for (int i = tree_depth - 1; i >= 0; i--) {
			TreeNode parent = GetParentAtDepth(i); 
			rpc.Write(parent.GetUUID());
			rpc.Write(parent.Type().ToString());
		}
			
		Write(rpc, 0);
		rpc.Send(null, DayZGame.RPC_NODE_SYNC, true, identity);
	}
	
	bool Contains(string uuid)
	{
		array<string> full_path = {};
		uuid.Split(PATH_SEPERATOR, full_path);
		TreeNode node = this;
		for (int i = 0; i < full_path.Count(); i++) {
			if (!node) {
				Error("Could not find child with id " + full_path[i]);
				break;
			}
			
			node = node.Children[uuid];
		}
		
		return node != null;
	}
							
	void Add(notnull TreeNode node)
	{
		Set(node.GetUUID(), node);
	}
	
	void Set(string uuid, notnull TreeNode node)
	{
		Children[uuid] = node;

		node.SetParent(this);
	}
	
	void Remove(notnull TreeNode node)
	{
		Remove(node.GetUUID());
	}
	
	void Remove(string uuid)
	{
		Children.Remove(uuid);
	}
	
	TreeNode Get(string uuid)
	{
		if (Children.Contains(uuid)) {
			return Children[uuid];
		}
	
		array<string> full_path = {};
		uuid.Split(PATH_SEPERATOR, full_path);
		TreeNode node = this;
		for (int i = 0; i < full_path.Count(); i++) {			
			node = node.Children[full_path[i]];
			
			if (!node) {
				Error(string.Format("[%1] Could not find child with id %2", m_UUID, full_path[i]));
				break;
			}
		}
		
		return node;
	}

	TreeNode GetNode(string uuid)
	{
		return this[uuid];
	}
	
	ScriptReadWriteContext CreateCopy()
	{
		ScriptReadWriteContext ctx = new ScriptReadWriteContext();
		ctx.GetWriteContext().Write(GetFullPath());		
		Write(ctx.GetWriteContext(), 0);
		return ctx;
	}
	
	// {uuid}{PATH_SEPERATOR}{uuid}
	string GetFullPath()
	{
		string full_path;
		for (int i = GetParentDepth(); i >= 0; i--) {
			TreeNode node = GetParentAtDepth(i);
			full_path += node.GetUUID();
			if (i != 0) {
				full_path += PATH_SEPERATOR;
			}
		}
		
		return full_path;
	}
			
	void SetParent(TreeNode parent)
	{
		m_Parent = parent;
		
		// Update visual display
#ifndef SERVER
#ifndef WORKBENCH
		if (m_Parent) {
			m_Parent.GetNodeView().GetTemplateController().ChildrenItems.Insert(GetNodeView());
		}
#endif
#endif
	}
	
	TreeNode GetParent()
	{
		return m_Parent;
	}
		
	void OnSynchronized()
	{
	}
	
	Editor GetEditor()
	{
		return Editor.Cast(FindParentOfType(Editor));
	}
	
	TreeNode FindParentOfType(typename type)
	{
		TreeNode parent = this;
		while (parent) {
			if (parent.IsInherited(type)) {
				return parent;
			}
						
			parent = parent.GetParent();
		}
		
		return null;
	}
	
	TreeNode GetParentAtDepth(int depth)
	{
		TreeNode parent = this;
		while (depth > 0) {
			if (!parent.GetParent()) {
				Error("GetParentAtDepth ran out of depth " + depth);
				return parent;
			}
			
			parent = parent.GetParent();
			depth--;
		}
		
		return parent;
	}
	
	int GetParentDepth()
	{
		int depth;
		TreeNode parent = GetParent();
		while (parent) {
			depth++;
			parent = parent.GetParent();
		}
		
		return depth;
	}
	
	bool CanDelete()
	{
		return false;
	}
							
	override void Write(Serializer serializer, int version)
	{		
		serializer.Write(m_UUID);
		serializer.Write(m_DisplayName);
		serializer.Write(m_Icon);
		
		serializer.Write(Children.Count());
		foreach (string uuid, TreeNode node: Children) {			
			serializer.Write(node.GetUUID());
			serializer.Write(node.Type().ToString());
			
			node.Write(serializer, version);
		}
		
		super.Write(serializer, version);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(m_UUID);
		serializer.Read(m_DisplayName);	
		serializer.Read(m_Icon);
			
		int count;
		serializer.Read(count);
		//Print(count);
		for (int i = 0; i < count; i++) {
			string uuid;
			serializer.Read(uuid);
			string type;
			serializer.Read(type);
			TreeNode node = Children[uuid];
			if (!node) {				
				node = TreeNode.Cast(type.ToType().Spawn());
				if (!node) {
					Error("Invalid node type " + type);
					return false;
				}
				
				node.SetParent(this);
				Children[uuid] = node;
			}
			
			node.Read(serializer, version);
			node.OnSynchronized();
		}
		
		return super.Read(serializer, version);
	}
		
	string GetUUID()
	{
		return m_UUID;
	}
	
	int GetVersion()
	{
		return 0;
	}
	
	string GetDisplayName()
	{
		return m_DisplayName;
	}
	
	Symbols GetIcon()
	{
		return m_Icon;
	}
		
	TreeView GetNodeView()
	{
		if (!m_NodeView) {
			m_NodeView = new TreeView(this);
		}
		
		return m_NodeView;
	}
	
	TreeNodeState GetDefaultState()
	{
		return TreeNodeState.EMPTY;
	}
	
	array<TreeNodeState> GetValidStates()
	{
		return { TreeNodeState.EMPTY, TreeNodeState.HOVER, TreeNodeState.ACTIVE };
	}
	
	TreeNodeInteract GetInteractType()
	{
		return m_TreeNodeInteract;
	}
	
	string GetShortcutString() 
	{
		string result;
		UAInput inp = GetUApi().GetInputByName(m_UUID);
		for (int i = 0; i < inp.BindKeyCount(); i++) { 
			if (inp.CheckBindDevice(i, EInputDeviceType.MOUSE_AND_KEYBOARD)) {
				string button_name = GetUApi().GetButtonName(inp.GetBindKey(i));
				button_name.Replace("Left ", "");
				button_name.Replace("Right ", "R");
				
				result += button_name;
			}
			
			if (i != inp.BindKeyCount() - 1) {
				result += " + ";
			}
		}
		
		return result;
	}
				
#ifdef DIAG_DEVELOPER
	void Debug(int depth)
	{
		string tabs;
		for (int i = 0; i < depth; i++) {
			tabs += "\t";
		}
		
		PrintFormat("[%4]%3[%1] %2:", m_UUID, m_DisplayName, tabs, depth);
		
		foreach (string uuid, TreeNode node: Children) {
			node.Debug(depth + 1);
		}
	}
#endif
}