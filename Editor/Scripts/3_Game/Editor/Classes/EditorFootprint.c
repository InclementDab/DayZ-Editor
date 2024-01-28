class EditorFootprint: TreeNode
{
	protected bool m_IsUndone;
	
	// Either ONE is null, OR THE OTHER, NEVER BOTH... 
	TreeNode Pointer;
	ref ScriptReadWriteContext Copy;
	
	void EditorFootprint(string uuid, string display_name, Symbols icon, TreeNode pointer, ScriptReadWriteContext copy)
	{
		// Can be null
		Pointer = pointer;
		
		// Can be null
		Copy = copy;
	}
	
	bool IsUndone()
	{
		return m_IsUndone;
	}
	
	void Undo()
	{
		WidgetAnimator.Animate(m_NodeView.Text, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		
		m_IsUndone = true;
		Print("Undoing!");
		
		// If its real (unreal it)
		if (!Copy) {
			Copy = Pointer.CreateCopy();
			delete Pointer;
		}
		
		// If its not real (real it)
		if (!Pointer) {
			int tree_depth;
			if (!Copy.GetReadContext().Read(tree_depth)) {
				Error("Invalid depth");
				return;
			}
	
			TreeNode current = Pointer;
			for (int i = 0; i < tree_depth; i++) {
				string uuid;
				Copy.GetReadContext().Read(uuid);
				
				string type;
				Copy.GetReadContext().Read(type);
				
				TreeNode node = current[uuid];
				if (!node) {
					node = TreeNode.Cast(type.ToType().Spawn());
					if (!node) {
						Error("Invalid node type " + type);
						continue;
					}
					
					current[uuid] = node;
					node.SetParent(current[uuid]);
				}
				
				current = current[uuid];
			}
							
			current.Read(Copy.GetReadContext(), 0);			
			delete Copy;
		}
	}
		
	void Redo()
	{
		WidgetAnimator.Animate(m_NodeView.Text, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		
		m_IsUndone = false;
		
		
	}
}