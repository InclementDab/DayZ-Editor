class EditorSelectAllCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (GetEditor().GetEditorHud().GetTemplateController().CategoryPlacements) {
			EditorObjectMap placed_objects = m_Editor.GetPlacedObjects();
			foreach (EditorObject eo: placed_objects) {
				m_Editor.SelectObject(eo);
			}
		}
		
		if (GetEditor().GetEditorHud().GetTemplateController().CategoryDeletions) {
			EditorDeletedObjectMap deleted_object = m_Editor.GetDeletedObjects();
			foreach (auto edo: deleted_object) {
				m_Editor.SelectHiddenObject(edo);
			}
		}
		
		return true;
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_SELECT_ALL";
	}
		
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_A };
	}
}