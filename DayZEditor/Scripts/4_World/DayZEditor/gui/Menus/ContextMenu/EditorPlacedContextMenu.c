class EditorPlacedContextMenu: EditorContextMenu
{
	void EditorPlacedContextMenu(float x, float y) 
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		AddMenuButton(m_Editor.CommandManager.CutCommand);
		AddMenuButton(m_Editor.CommandManager.CopyCommand);
		AddMenuButton(m_Editor.CommandManager.PasteCommand);
		AddMenuButton(m_Editor.CommandManager.DeleteCommand);
		AddMenuButton(m_Editor.CommandManager.ShowCommand);
		AddMenuButton(m_Editor.CommandManager.HideCommand);
		AddMenuButton(m_Editor.CommandManager.LockCommand);
		AddMenuButton(m_Editor.CommandManager.UnlockCommand);		
		AddMenuButton(m_Editor.CommandManager.ObjectPropertiesCommand);
	}
}