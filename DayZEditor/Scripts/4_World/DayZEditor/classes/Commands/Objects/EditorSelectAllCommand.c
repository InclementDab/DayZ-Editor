class EditorSelectAllCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		string right_search_bar_text = GetEditor().GetEditorHud().RightSearchBar.GetText();
		if (GetEditor().GetEditorHud().GetTemplateController().CategoryPlacements) {
			EditorObjectMap placed_objects = m_Editor.GetPlacedObjects();
			foreach (EditorObject eo: placed_objects) {
				if (!eo.GetListItem() || eo.GetListItem().FilterType(right_search_bar_text)) {
					m_Editor.SelectObject(eo);
				}
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