class EditorFootprint: TreeNode
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
		WidgetAnimator.Animate(m_NodeView.Text, WidgetAnimatorProperty.COLOR_A, 100.0 / 255.0, 50);
		
		m_IsUndone = true;
		
		Print("Undoing!");
		int tree_depth;
		if (!Data.GetReadContext().Read(tree_depth)) {
			Error("Invalid depth");
			return;
		}

		TreeNode current = GetParent();
		for (int i = 0; i < tree_depth; i++) {
			string uuid;
			Data.GetReadContext().Read(uuid);
			
			string type;
			Data.GetReadContext().Read(type);
			
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
						
		current.Read(Data.GetReadContext(), 0);
	}
		
	void Redo()
	{
		WidgetAnimator.Animate(m_NodeView.Text, WidgetAnimatorProperty.COLOR_A, 1.0, 50);
		
		m_IsUndone = false;
		
		Print("Redoing!");
	}
}