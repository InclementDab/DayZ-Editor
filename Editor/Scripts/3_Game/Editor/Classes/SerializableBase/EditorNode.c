class EditorNode: SerializableBase
{
	static ref array<EditorNode> SelectedObjects = {};	
	static ref array<EditorNode> DirtyObjects = {};
	
	static void ClearSelections()
	{
		foreach (EditorNode selected_object: SelectedObjects) {
			if (selected_object) {
				selected_object.SetSelected(false);
			}
		}
	}
	
	ref ScriptInvoker OnSelectionChanged = new ScriptInvoker();
	ref ScriptInvoker OnLockChanged = new ScriptInvoker();

	protected string m_UUID;	
	protected ref map<string, ref EditorNode> m_Children = new map<string, ref EditorNode>();

	protected string m_Icon, m_DisplayName;
	protected bool m_IsSelected; // local
	protected EditorNode m_Parent;
	
	protected ref EditorNodeView m_NodeView;
	
	void EditorNode(string uuid, string display_name, Symbols icon)
	{
		m_UUID = uuid;
		m_DisplayName = display_name;
		m_Icon = icon;
		
#ifndef SERVER
		m_NodeView = new EditorNodeView(m_DisplayName, this, m_Icon);
#endif
	}
	
	void ~EditorNode()
	{
		delete m_NodeView;
	}
	
	void Synchronize(PlayerIdentity identity = null)
	{	
		ScriptRPC rpc = new ScriptRPC();
		int tree_depth = GetParentDepth();
		rpc.Write(tree_depth);

		for (int i = tree_depth - 1; i >= 0; i--) {
			EditorNode parent = GetParentAtDepth(i);
			Print(parent.GetUUID());
			rpc.Write(parent.GetUUID());
			rpc.Write(parent.Type().ToString());
		}
			
		Write(rpc, 0);
		rpc.Send(null, DayZGame.RPC_NODE_SYNC, true, identity);
	}
							
	void Add(notnull EditorNode node)
	{
		Set(node.GetUUID(), node);
	}
	
	void Set(string uuid, notnull EditorNode node)
	{
		m_Children[uuid] = node;
		
		node.SetParent(this);
	}
	
	void Remove(string uuid)
	{
		delete m_Children[uuid];
		m_Children.Remove(uuid);
	}
	
	EditorNode Get(string uuid)
	{		
		return m_Children[uuid];
	}
	
	EditorNode GetNode(string uuid)
	{
		return this[uuid];
	}
	
	map<string, ref EditorNode> GetChildren()
	{
		return m_Children;
	}
	
	void SetParent(EditorNode parent)
	{
		m_Parent = parent;
		
		// Update visual display
		if (m_Parent && m_Parent.GetNodeView()) {
			m_Parent.GetNodeView().GetTemplateController().ChildrenItems.Insert(m_NodeView);
		}
	}
	
	EditorNode GetParent()
	{
		return m_Parent;
	}
	
	void OnSynchronized()
	{
	}
	
	EditorNode GetParentAtDepth(int depth)
	{
		EditorNode parent = this;
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
		EditorNode parent = GetParent();
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
		
		serializer.Write(m_Children.Count());
		foreach (string uuid, EditorNode node: m_Children) {
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
			EditorNode node = m_Children[uuid];
			if (!node) {				
				node = EditorNode.Cast(type.ToType().Spawn());
				if (!node) {
					Error("Invalid node type " + type);
					return false;
				}
				
				node.SetParent(this);
				m_Children[uuid] = node;
			}
			
			node.Read(serializer, version);
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
		
	EditorNodeView GetNodeView()
	{
		return m_NodeView;
	}
	
	void SetSelected(bool selected)
	{
		m_IsSelected = selected;
		
		m_NodeView.ShowChildren(m_IsSelected);
		
		if (m_IsSelected) {
			SelectedObjects.Insert(this);
		} else {
			SelectedObjects.RemoveItem(this);
		}
				
		OnSelectionChanged.Invoke(this);
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
		
		PrintFormat("%3[%1] %2:", m_UUID, m_DisplayName, tabs);
		
		foreach (string uuid, EditorNode node: m_Children) {
			node.Debug(depth + 1);
		}
	}
#endif
}