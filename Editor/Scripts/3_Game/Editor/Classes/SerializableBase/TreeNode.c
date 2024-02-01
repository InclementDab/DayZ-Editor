class TreeNode: SerializableBase
{
	static const string PATH_SEPERATOR = "\\";
	static ref TreeNodeStateMachine StateMachine = new TreeNodeStateMachine();
		
	protected string m_UUID;
	protected UAInput m_Input;
	protected TreeNodeInteract m_TreeNodeInteract = GetInteractType();
	protected TreeNodeState m_TreeNodeState = GetDefaultState();
	protected ref map<int, TreeNodeState> m_TreeNodeStateHistory = new map<int, TreeNodeState>();
	ref map<string, ref TreeNode> Children = new map<string, ref TreeNode>();

	protected string m_Icon, m_DisplayName;
	
	TreeNode Parent;
	ref TreeView View;
	ref ScriptInvoker State_OnChanged = new ScriptInvoker();
	
	void TreeNode(string uuid, string display_name, Symbols icon)
	{
		m_UUID = uuid;
		m_DisplayName = display_name;
		m_Icon = icon;

		StateMachine[m_TreeNodeState].Insert(this);
		
		m_Input = GetUApi().GetInputByName(GetInputName());
		if (m_Input.ID() != -1) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateInputs);
		}
		
#ifndef SERVER
#ifndef WORKBENCH
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
#endif
#endif
	}
	
	void ~TreeNode()
	{
		if (StateMachine[m_TreeNodeState]) {
			StateMachine[m_TreeNodeState].RemoveItem(this);
		}
		
		delete View;
		
		if (GetGame() && GetGame().GetUpdateQueue(CALL_CATEGORY_GUI)) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateInputs);
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		}
	}
	
	void Update(float dt)
	{
		// Please dont use me much :(
	}
	
	protected void UpdateInputs()
	{
		switch (GetInteractType()) {
			case TreeNodeInteract.ONCE:
			case TreeNodeInteract.PRESS: {
				if (m_Input.LocalPress()) {
					AddState(TreeNodeState.ACTIVE);
					break;
				}
				
				break;
			}
			
			case TreeNodeInteract.DOUBLE: {
				if (m_Input.LocalDoubleClick()) {
					AddState(TreeNodeState.ACTIVE);
					break;
				}
				
				break;
			}
			
			case TreeNodeInteract.HOLD: {
				if (m_Input.LocalHoldBegin()) {
					AddState(TreeNodeState.ACTIVE);
					break;
				}
				
				if (m_Input.LocalRelease()) {
					RemoveState(TreeNodeState.ACTIVE);
					break;
				}
				
				break;
			}
			
			case TreeNodeInteract.TOGGLE: {
				if (m_Input.LocalPress()) {
					if (HasState(TreeNodeState.ACTIVE)) {
						RemoveState(TreeNodeState.ACTIVE);
					} else {
						AddState(TreeNodeState.ACTIVE);
					}
					
					break;
				}
				
				break;
			}
		}
	}
		
	void AddState(TreeNodeState state)
	{
		//PrintFormat("[%1], AddState=%2, StateMask=%3, Result=%4", m_UUID, typename.EnumToString(TreeNodeState, state), GetStateMask(), state & GetStateMask());
		state &= GetStateMask();
		if (state == 0) {
			return;
		}
		
		StateMachine[state].Insert(this);		
		m_TreeNodeState |= state;
		m_TreeNodeStateHistory.Insert(DateTime.Now(), state);
		OnStateChanged(state, m_TreeNodeState);
		State_OnChanged.Invoke(this, state);
	}
	
	void RemoveState(TreeNodeState state)
	{
		state &= GetStateMask();
		if (state == 0) {
			return;
		}
		
		StateMachine[state].RemoveItem(this);
		m_TreeNodeState &= ~state;
		m_TreeNodeStateHistory.Insert(DateTime.Now(), state);
		OnStateChanged(state, m_TreeNodeState);
		State_OnChanged.Invoke(this, state);
	}
	
	// lmao
	bool CheckIfStateChangedInPeriod(DateTime min, DateTime max, TreeNodeState state)
	{
		for (int i = min; i < max; i++) {
			if (m_TreeNodeStateHistory[i] == state) {
				return true;
			}
		}
		
		return false;
	}
	
	void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
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
		TreeNode former_parent = node.Parent;
		
		Children[uuid] = node;
		node.Parent = this;
		
#ifndef SERVER
#ifndef WORKBENCH
		if (!View) {
			//Print("No view for " + m_UUID);
		} else {
			View.AddView(node.CreateView());
		}
		
		if (former_parent && former_parent.View) {
			former_parent.View.RecalculateSize();
		}
#endif
#endif
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
				break;
			}
		}
		
		return node;
	}

	TreeNode GetNode(string uuid)
	{
		return this[uuid];
	}
	
	TreeView CreateView()
	{		
		View = new TreeView(this);
		foreach (string uuid, TreeNode node: Children) {
			// Initialize all child nodes aswell :)
			View.AddView(node.CreateView());
		}
				
		return View;
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
			
	void OnSynchronized()
	{
	}
	
	EditorNode GetEditor()
	{
		return EditorNode.Cast(FindParentOfType(EditorNode));
	}
	
	TreeNode FindParentOfType(typename type)
	{
		TreeNode parent = this;
		while (parent) {
			if (parent.IsInherited(type)) {
				return parent;
			}
						
			parent = parent.Parent;
		}
		
		return null;
	}
	
	TreeNode GetParentAtDepth(int depth)
	{
		TreeNode parent = this;
		while (depth > 0) {
			if (!parent.Parent) {
				Error("GetParentAtDepth ran out of depth " + depth);
				return parent;
			}
			
			parent = parent.Parent;
			depth--;
		}
		
		return parent;
	}
	
	int GetParentDepth()
	{
		int depth;
		TreeNode parent = Parent;
		while (parent) {
			depth++;
			parent = parent.Parent;
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
				
				node.Parent = this;
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
		
	TreeNodeState GetDefaultState()
	{
		return TreeNodeState.EMPTY;
	}
	
	// Override to enable different states
	TreeNodeState GetStateMask()
	{
		return TreeNodeState.EMPTY;
	}
		
	TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.NONE;
	}
	
	bool HasState(TreeNodeState state)
	{
		return (m_TreeNodeState & state) == state;
	}
	
	TreeNodeState GetState()
	{
		return m_TreeNodeState;
	}
	
	string GetShortcutString() 
	{
		string result;
		UAInput inp = GetUApi().GetInputByName(GetInputName());
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
	
	string GetInputName()
	{
		return string.Format("UAEditor%1", m_UUID);
	}
	
	array<string> GetXorSelections()
	{
		return {};
	}
				
#ifdef DIAG_DEVELOPER
	void Debug(int depth)
	{
		string tabs;
		for (int i = 0; i < depth; i++) {
			tabs += "\t";
		}
		
		PrintFormat("[%4]%3[%1] %2: State:%3", m_UUID, m_DisplayName, tabs, depth, m_TreeNodeState);
		
		foreach (string uuid, TreeNode node: Children) {
			node.Debug(depth + 1);
		}
	}
#endif
}