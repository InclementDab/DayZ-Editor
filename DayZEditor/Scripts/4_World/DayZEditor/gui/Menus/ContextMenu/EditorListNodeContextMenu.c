class EditorListNodeContextMenu: EditorContextMenu
{
	void EditorListNodeContextMenu(float x, float y, notnull EditorListNode context)
	{
		m_Editor.CommandManager[EditorExpandAllCommand].SetData(new Param1<EditorListNode>(context));
		m_Editor.CommandManager[EditorCollapseAllCommand].SetData(new Param1<EditorListNode>(context));
		
		AddMenuButton(m_Editor.CommandManager[EditorExpandAllCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCollapseAllCommand]);
	}
}