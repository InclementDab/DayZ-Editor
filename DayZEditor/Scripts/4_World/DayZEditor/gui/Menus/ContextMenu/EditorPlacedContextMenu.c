class EditorPlacedContextMenu: EditorContextMenu
{
	void EditorPlacedContextMenu(float x, float y, EditorObject context) 
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		AddMenuButton(m_Editor.CommandManager[EditorCutCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCopyCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPasteCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorDeleteCommand]);
		if (context.GetWorldObject().IsMan()) {
			// Just assigning the package *cool*
			m_Editor.CommandManager[EditorControlPlayerCommand].SetData(new Param1<EditorObject>(context));
			
			AddMenuDivider();
			AddMenuButton(m_Editor.CommandManager[EditorControlPlayerCommand]);
		}
		
		EntityAI entity;
		if (Class.CastTo(entity, context.GetWorldObject()) && entity.GetInventory()) {
			m_Editor.CommandManager[EditorEditInventoryCommand].SetData(new Param1<EntityAI>(entity));
			AddMenuButton(m_Editor.CommandManager[EditorEditInventoryCommand]);
		}
		
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorShowCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorHideCommand]);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager[EditorLockCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorUnlockCommand]);
		AddMenuDivider();		
		AddMenuButton(m_Editor.CommandManager[EditorDuplicateCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorObjectPropertiesCommand]);
	}
}