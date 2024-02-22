class EditorFootprint: Node
{
	protected bool m_IsUndone;
	
	// Either ONE is null, OR THE OTHER, NEVER BOTH... 
	Node Pointer;
	ref ScriptReadWriteContext Copy;
	protected Node m_PointerParent;
	protected string m_Path;
	
	void EditorFootprint(UUID uuid, Node pointer, ScriptReadWriteContext copy)
	{
		// Can be null
		Pointer = pointer;
		
		if (Pointer) {
			m_Path = Pointer.GetFullPath();
			m_PointerParent = Pointer.GetParent();
		}
		
		// Can be null
		Copy = copy;
	}
	
	bool IsUndone()
	{
		return m_IsUndone;
	}
	
	void Undo()
	{
		//WidgetAnimator.Animate(View.Text, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		
		m_IsUndone = true;
		Print("Undoing!");
		
		// If its real (unreal it)
		if (!Copy) {
			Print(Pointer);
			Copy = Pointer.CreateCopy();
			delete Pointer;
			return;
		}
			
		// If its not real (real it)
		int tree_depth;
		if (!Copy.GetReadContext().Read(tree_depth)) {
			Error("Invalid depth");
			return;
		}

		Node current = Pointer;
		for (int i = 0; i < tree_depth; i++) {
			string uuid;
			Copy.GetReadContext().Read(uuid);
			
			string type;
			Copy.GetReadContext().Read(type);
			
			Node node = current[uuid];
			if (!node) {
				node = Node.Cast(type.ToType().Spawn());
				if (!node) {
					Error("Invalid node type " + type);
					continue;
				}
				
				current[uuid] = node;
				node.GetParent() = current[uuid];
			}
			
			current = current[uuid];
		}
						
		current.Read(Copy.GetReadContext(), 0);			
	}
		
	void Redo()
	{
		//WidgetAnimator.Animate(View.Text, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		
		m_IsUndone = false;
		
		// If its real (unreal it)
		if (!Copy) {
			Print(Pointer);
			Copy = Pointer.CreateCopy();
			delete Pointer;
		}
		
		// If its not real (real it)
		Node current = m_PointerParent;
		//for (int i = 0; i < tree_depth; i++) {
		string uuid_tree;
		Copy.GetReadContext().Read(uuid_tree);
		
		Node node = m_PointerParent.Get(uuid_tree);
		node.Read(Copy.GetReadContext(), 0);
		delete Copy;
	}
}