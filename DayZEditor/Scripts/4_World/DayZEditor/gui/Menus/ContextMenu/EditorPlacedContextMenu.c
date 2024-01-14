class EditorPlacedContextMenu: EditorContextMenu
{
	void EditorPlacedContextMenu(float x, float y, EditorObject context) 
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		AddMenuButton(m_Editor.CommandManager[EditorCutCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCopyCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorPasteCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorDeleteCommand]);
		EntityAI entity;
		if (Class.CastTo(entity, context.GetWorldObject()) && entity.GetInventory() && entity.GetInventory().GetAttachmentSlotsCount() > 0) {
			AddMenuDivider();
			m_Editor.CommandManager[EditorEditInventoryCommand].SetData(new Param1<EntityAI>(entity));
			AddMenuButton(m_Editor.CommandManager[EditorEditInventoryCommand]);
		}
		
		if (context.GetWorldObject().IsMan()) {
			// Just assigning the package *cool*
			m_Editor.CommandManager[EditorControlPlayerCommand].SetData(new Param1<EditorObject>(context));
			AddMenuButton(m_Editor.CommandManager[EditorControlPlayerCommand]);
		}
		
		AddMenuButton(m_Editor.CommandManager[EditorAlignToSurfaceCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorSnapToSurfaceCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorResetAlignmentCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorExportRelativeToObject]);
		
		AddMenuDivider();
		/*if (context.Show) {
			AddMenuButton(m_Editor.CommandManager[EditorHideCommand]);
		} else {
			AddMenuButton(m_Editor.CommandManager[EditorShowCommand]);
		}
		
		if (context.Locked) {
			AddMenuButton(m_Editor.CommandManager[EditorUnlockCommand]);
		} else {
			AddMenuButton(m_Editor.CommandManager[EditorLockCommand]);
		}*/
		
		AddMenuDivider();		
		AddMenuButton(m_Editor.CommandManager[EditorDuplicateCommand]);
		
		m_Editor.CommandManager[EditorExportEventPos].SetData(new Param1<EntityAI>(EntityAI.Cast(context.GetWorldObject())));
		AddMenuButton(m_Editor.CommandManager[EditorExportEventPos]);
		AddMenuButton(m_Editor.CommandManager[EditorObjectPropertiesCommand]);
	}
}