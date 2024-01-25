class EditorHistory: set<ref EditorFootprint>
{
	static const int MAX_SIZE = 512;
	
	int InsertAction(EditorFootprint value)
	{	
		int count = Count();
		for (int i = 0; i < count; i++) {
			if (!this[i].IsUndone()) {
				break;
			}
			
			Remove(i);
			i--; count--;
		}
		
		if (count >= MAX_SIZE) {
			Remove(count - 1);
		}
		
		// Adds to bottom of stack
		return InsertAt(value, 0);
	}
}

class EditorFootprint: Managed
{
	protected bool m_IsUndone;
	
	string ID = UUID.Empty;
	string Type;
	ref ScriptReadWriteContext Data = new ScriptReadWriteContext();
	
	bool IsUndone()
	{
		return m_IsUndone;
	}
	
	void Undo()
	{
		m_IsUndone = true;
		
		int tree_depth;
		if (!Data.GetReadContext().Read(tree_depth)) {
			Error("Invalid depth");
			return;
		}

		EditorNode current = GetDayZGame().GetEditor();
		for (int i = 0; i < tree_depth; i++) {
			string uuid;
			Data.GetReadContext().Read(uuid);
			
			string type;
			Data.GetReadContext().Read(type);
			
			EditorNode node = current[uuid];
			if (!node) {
				node = EditorNode.Cast(type.ToType().Spawn());
				if (!node) {
					Error("Invalid node type " + type);
					continue;
				}
				
				current[uuid] = node;
				node.SetParent(current[uuid]);
			}
			
			current = current[uuid];
		}
						
		current.Read(Data.GetReadContext(), 0);
	}
		
	void Redo()
	{
		m_IsUndone = false;
	}
}