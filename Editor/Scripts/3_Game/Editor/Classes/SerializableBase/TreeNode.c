class DefaultNode: AttributeBase
{	
	void DefaultNode(string display_name, Symbols symbol = string.Empty)
	{
		TreeNode.ROOT.Add(new TreeNode(Field.DefaultValue, display_name, symbol));
	}
}

class RootNode: TreeNode
{	
	void RootNode(string uuid, string display_name, Symbols icon)
	{
		this[COMMANDS].Add(new AfterlifeToggle("Afterlife", "View Hidden", Symbols.GHOST));
		this[COMMANDS].Add(new AddLayerCommand("AddLayer", "Add Layer", Symbols.LAYER_PLUS));
		this[COMMANDS].Add(new SetLayerActiveCommand("SetLayerActive", "Set Layer Active", string.Empty));
		this[COMMANDS].Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT));
		this[COMMANDS].Add(new PianoCommand("Piano", "Drop Piano", Symbols.PIANO));
		this[COMMANDS].Add(new BoxCommand("Box", "Box Selection", Symbols.SQUARE_DASHED));
		this[COMMANDS].Add(new CommandNode("Camera", "Camera", Symbols.CAMERA));
		this[COMMANDS].Add(new EllipseCommand("Ellipse", "Ellipse Selection", Symbols.CIRCLE_DASHED));
		this[COMMANDS].Add(new CommandNode("Copy", "Copy", Symbols.COPY));
		this[COMMANDS].Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS));
		this[COMMANDS].Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH));
		this[COMMANDS].Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE));
		this[COMMANDS].Add(new LassoCommand("Lasso", "Lasso Select", Symbols.LASSO));
		this[COMMANDS].Add(new CommandNode("Lock", "Lock", Symbols.LOCK));
		this[COMMANDS].Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET));
		this[COMMANDS].Add(new CommandNode("New", "New", Symbols.FILE));
		this[COMMANDS].Add(new CommandNode("Open", "Open", Symbols.FOLDER_OPEN));
		this[COMMANDS].Add(new CommandNode("Paste", "Paste", Symbols.PASTE));
		this[COMMANDS].Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT));
		this[COMMANDS].Add(new RenameCommand("Rename", "Rename", string.Empty));
		this[COMMANDS].Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN));
		this[COMMANDS].Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK));
		this[COMMANDS].Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK));
		this[COMMANDS].Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT));
		this[COMMANDS].Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN));
		this[COMMANDS].Add(new WeatherToggle("Weather", "Weather", Symbols.SUN));
		this[COMMANDS].Add(new CursorToggle("Cursor", "Toggle Cursor", Symbols.ARROW_POINTER));
		this[COMMANDS].Add(new HudToggle("Hud", "Toggle Hud", Symbols.EYE));
		this[COMMANDS].Add(new MapToggle("Map", "Toggle Map", Symbols.MAP));
		this[COMMANDS].Add(new ControlToggle("Control", "Toggle Control", Symbols.PERSON_PREGNANT));
		this[COMMANDS].Add(new CreateFolder("CreateFolder", "Create Folder", Symbols.FOLDER_PLUS));
		this[COMMANDS].Add(new CommandNode("Console", "Toggle Console", Symbols.XBOX));
	}
}

class TreeNode: SerializableBase
{
	static const string PATH_SEPERATOR = "\\";
	static const ref RootNode ROOT = new RootNode(string.Empty, string.Empty, string.Empty);
	
	static ref TreeNodeStateMachine StateMachine = new TreeNodeStateMachine();
	
	static const string MISSION = "Mission";
	static const string EDITORS = "Editors";
	static const string COMMANDS = "Commands";
		
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
		
		if (StateMachine && StateMachine[m_TreeNodeState]) {
			StateMachine[m_TreeNodeState].Insert(this);
		}
					
#ifndef SERVER
#ifndef WORKBENCH
		m_Input = GetUApi().GetInputByName(GetInputName());
		if (m_Input.ID() != -1) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateInputs);
		}
#endif
#endif
	}
	
	void ~TreeNode()
	{
		if (StateMachine[m_TreeNodeState]) {
			StateMachine[m_TreeNodeState].RemoveItem(this);
		}
		
		delete View;
	}
	
	// Only runs when activated
	void Update(float dt);
			
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
		if (state.IsActive()) {
			if (total_state.IsActive()) {
				GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
			} else {
				GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
			}
		}
	}
		
	bool CreateContextMenu(inout ObservableCollection<ref MenuNode> list_items)
	{
		list_items.Insert(new MenuNode(GetEditor().GetCommand("Rename")));
		return true;
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
			
	void Add(TreeNode node)
	{
		if (node) {
			Set(node.GetUUID(), node);
		}
	}
	
	void Set(string uuid, TreeNode node)
	{	
		TreeNode context = this;
		if (uuid.Contains("\\")) {
			array<string> uuid_split = {};
			uuid.Split("\\", uuid_split);
			if (uuid_split[0] != m_UUID) {
				Error("Attempting to set sub-children with invalid root. Did not match uuid");
			}
			
			for (int i = 1; i < uuid_split.Count() - 1; i++) {				
				if (!context.Children[uuid_split[i]]) {
					context.Debug(0);
					Error(string.Format("Invalid tree %1: token=%2", uuid, uuid_split[i]));
					return;
				}
				
				context = context.Children[uuid_split[i]];
			}
		}
		
		context.Children[uuid] = node;
		node.Parent = context;
		
#ifndef SERVER
#ifndef WORKBENCH
		if (!View) {
			//Print("No view for " + m_UUID);
		} else {
			View.AddView(node.CreateView());
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
	
	// {uuid}\\{uuid}
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
	
	bool CanRename()
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
		return TreeNodeState.EXTEND; // because everyone can have kids. collapse can auto hide
	}
		
	TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.PRESS;
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
				
#ifdef DIAG_DEVELOPER
	void Debug(int depth)
	{
		string tabs;
		for (int i = 0; i < depth; i++) {
			tabs += "\t";
		}
		
		PrintFormat("%4:%3[%1] %2: State:%4", m_UUID, m_DisplayName, tabs, depth, m_TreeNodeState);
		
		foreach (string uuid, TreeNode node: Children) {
			node.Debug(depth + 1);
		}
	}
#endif
}