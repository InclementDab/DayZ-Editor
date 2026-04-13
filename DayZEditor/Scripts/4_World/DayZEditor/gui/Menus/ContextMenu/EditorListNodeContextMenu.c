class EditorNodeViewContextMenu: EditorContextMenu
{
	void EditorNodeViewContextMenu(float x, float y, notnull EditorNodeView context)
	{
		m_Editor.CommandManager[EditorExpandAllCommand].SetData(new Param1<EditorNodeView>(context));
		m_Editor.CommandManager[EditorCollapseAllCommand].SetData(new Param1<EditorNodeView>(context));
		
		AddMenuButton(m_Editor.CommandManager[EditorExpandAllCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCollapseAllCommand]);
	}
}