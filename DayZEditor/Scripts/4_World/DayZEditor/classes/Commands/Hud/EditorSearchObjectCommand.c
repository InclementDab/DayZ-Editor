class EditorSearchObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		Object object = Editor.GetObjectUnderCursor();
		if (!object) {
			return true;
		}
		
		m_Editor.GetEditorHud().ShowCursor(true);
		m_Editor.GetEditorHud().GetTemplateController().SearchBarData = object.GetType();
		m_Editor.GetEditorHud().GetTemplateController().NotifyPropertyChanged("SearchBarData");
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_SEARCH_OBJECT";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_F };
	}
}