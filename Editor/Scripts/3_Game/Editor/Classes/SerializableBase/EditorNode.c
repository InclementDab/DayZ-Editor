class EditorNode: SerializableBase
{
	static ref array<EditorNode> All = {};
	static ref array<EditorNode> SelectedObjects = {};
	static ref array<EditorNode> LockedObjects = {};
	
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

	// UUID of children
	protected ref map<string, ref EditorNode> m_Children = new map<string, ref EditorNode>();

	protected string m_DisplayName;
	protected bool m_IsSelected; // local
	protected bool m_IsDirty; // local

	protected ref EditorNodeView m_TreeItem;
	
	void EditorNode()
	{
		All.Insert(this);
		
		if (!GetGame().IsDedicatedServer()) {
			m_TreeItem = new EditorNodeView(m_DisplayName, this);
		}
	}
	
	void ~EditorNode()
	{
		All.RemoveItem(this);
		
		delete m_TreeItem;
	}
	
	static EditorNode Create(string display_name)
	{
		ScriptReadWriteContext ctx = new ScriptReadWriteContext();
		ctx.GetWriteContext().Write(display_name);
		ctx.GetWriteContext().Write(0);
		return new EditorNode(ctx.GetReadContext());
	}
	
	void SetSynchDirty()
	{
		m_IsDirty = true;
	}
	
	void ClearSynchDirty()
	{
		m_IsDirty = false;
	}
	
	bool IsSynchDirty()
	{
		return m_IsDirty;
	}
	
	void AddChild(string uuid, notnull EditorNode node)
	{
		m_Children[uuid] = node;
		
		// Update visual display
		if (!GetGame().IsDedicatedServer()) {
			m_TreeItem.GetTemplateController().ChildrenItems.Insert(node.GetTreeItem());
		}
	}
	
	void RemoveChild(string uuid)
	{
		delete m_Children[uuid];
		m_Children.Remove(uuid);
	}
			
	override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);
		
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
		if (!super.Read(serializer, version)) {
			return false;
		}
		
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
			}
			
			m_Children[uuid].Read(serializer, version);
		}
		
		return true;
	}
	
	void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
	}
	
	// All RPC's valid that route to OnRPC
	array<int> GetProtocols()
	{
		return {};
	}
	
	int GetVersion()
	{
		return 0;
	}
		
	EditorNodeView GetTreeItem()
	{
		return m_TreeItem;
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
	
	bool IsSelected() 
	{
		return m_IsSelected;
	}
}