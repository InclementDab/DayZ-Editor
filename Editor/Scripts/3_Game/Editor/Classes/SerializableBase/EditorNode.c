class EditorNode: SerializableBase
{	
	static const int RPC_SYNC = 154365;
		
	static ref map<string, EditorNode> All = new map<string, EditorNode>();
	static ref array<EditorNode> SelectedObjects = {};	
	static ref array<EditorNode> DirtyObjects = {};
	
	static void ClearSelections()
	{
		foreach (EditorNode selected_object: SelectedObjects) {
			if (selected_object && selected_object.IsSelected()) {
				selected_object.SetSelected(false);
			}
		}
		
		SelectedObjects.Clear();
	}
	
	ref ScriptInvoker OnSelectionChanged = new ScriptInvoker();
	ref ScriptInvoker OnLockChanged = new ScriptInvoker();

	protected string m_UUID;	
	protected ref map<string, ref EditorNode> m_Children = new map<string, ref EditorNode>();

	protected string m_DisplayName;
	protected string m_Icon;
	protected bool m_IsSelected; // local
	protected EditorNode m_Parent;
	
	protected ref EditorNodeView m_NodeView;
	
	void EditorNode(string uuid, string display_name, string icon)
	{
		m_UUID = uuid;
		m_DisplayName = display_name;
		
		All[m_UUID] = this;
		
		if (!GetGame().IsDedicatedServer()) {
			m_NodeView = new EditorNodeView(m_DisplayName, this);
		}
	}
	
	void ~EditorNode()
	{
		All.Remove(m_UUID);
		
		delete m_NodeView;
	}
	
	void Synchronize()
	{	
		ScriptRPC rpc = new ScriptRPC();
		
		string uuid_string = m_UUID;
		EditorNode parent = m_Parent;
		while (parent) {
			uuid_string += string.Format("|%1", parent.GetUUID());
		}
		
		rpc.Write(uuid_string);
		rpc.Write(Type().ToString());
		Write(rpc, 0);
		rpc.Send(null, RPC_SYNC, true);		
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
	
	map<string, ref EditorNode> GetChildren()
	{
		return m_Children;
	}
	
	void SetParent(EditorNode parent)
	{
		m_Parent = parent;
		
		// Update visual display
		if (!GetGame().IsDedicatedServer()) {
			m_Parent.GetNodeView().GetTemplateController().ChildrenItems.Insert(m_NodeView);
		}
	}
	
	EditorNode GetParent()
	{
		return m_Parent;
	}
					
	override void Write(Serializer serializer, int version)
	{		
		serializer.Write(m_UUID);
		serializer.Write(m_DisplayName);
		
		serializer.Write(m_Children.Count());
		foreach (string uuid, EditorNode node: m_Children) {
			serializer.Write(uuid);
			serializer.Write(node.Type().ToString());
			node.Write(serializer, version);
		}
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(m_UUID);	
		serializer.Read(m_DisplayName);	
		int count;
		serializer.Read(count);
		for (int i = 0; i << count; i++) {
			string uuid;
			serializer.Read(uuid);
			
			string type;
			serializer.Read(type);
			
			if (!m_Children[uuid]) {				
				m_Children[uuid] = EditorNode.Cast(type.ToType().Spawn());
				
				if (!m_Children[uuid]) {
					Error("Invalid node type!");
					return false;
				}
			}
			
			m_Children[uuid].Read(serializer, version);
		}
		
		return true;
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
	
	string GetIcon()
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
		
		m_NodeView.Collapse(m_IsSelected);
		
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
}