enum ShortcutKeyType
{
	NONE, // darn :(
	PRESS, // 100ms
	DOUBLE, // woah
	HOLD, // EEEEEK like a poop?
	TOGGLE // ying and yang
};

class TreeNode: SerializableBase
{
	static const string PATH_SEPERATOR = "\\";
	
	static ref array<TreeNode> SelectedObjects = {};	
	static ref array<TreeNode> DirtyObjects = {};
	
	static void ClearSelections()
	{
		foreach (TreeNode selected_object: SelectedObjects) {
			if (selected_object) {
				selected_object.SetSelected(false);
			}
		}
	}
	
	ref ScriptInvoker OnSelectionChanged = new ScriptInvoker();

	protected string m_UUID;	
	ref map<string, ref TreeNode> Children = new map<string, ref TreeNode>();

	protected string m_Icon, m_DisplayName;
	protected bool m_IsSelected; // local
	protected TreeNode m_Parent;
	
	protected ref TreeView m_NodeView;
	
	void TreeNode(string uuid, string display_name, Symbols icon)
	{
		m_UUID = uuid;
		m_DisplayName = display_name;
		m_Icon = icon;
	}
	
	void ~TreeNode()
	{
		delete m_NodeView;
	}
			
	void Synchronize(PlayerIdentity identity = null)
	{	
		ScriptRPC rpc = new ScriptRPC();
		int tree_depth = GetParentDepth();
		rpc.Write(tree_depth);

		for (int i = tree_depth; i >= 0; i--) {
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
	
	void Remove(string uuid)
	{
		delete Children[uuid];
		Children.Remove(uuid);
	}
	
	TreeNode Get(string uuid)
	{
		if (!Children.Contains(uuid)) {
			Error(string.Format("[%1:%2] did not contain child: %3", m_UUID, m_DisplayName, uuid));
		}
	
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
		int parent_depth = GetParentDepth();
		for (int i = parent_depth - 1; i >= 0; i++) {
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
		if (m_Parent && m_Parent.GetNodeView()) {
			m_Parent.GetNodeView().GetTemplateController().ChildrenItems.Insert(GetNodeView());
		}
	}
	
	TreeNode GetParent()
	{
		return m_Parent;
	}
		
	void OnSynchronized()
	{
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
	
	void SetSelected(bool selected)
	{
		m_IsSelected = selected;
				
		if (m_IsSelected) {
			SelectedObjects.Insert(this);
		} else {
			SelectedObjects.RemoveItem(this);
		}
				
		OnSelectionChanged.Invoke(this);
	}
	
	bool GetDefaultState()
	{
		return false;
	}
	
	bool CanSelect()
	{
		return true;
	}
		
	bool IsSelected() 
	{
		return m_IsSelected;
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