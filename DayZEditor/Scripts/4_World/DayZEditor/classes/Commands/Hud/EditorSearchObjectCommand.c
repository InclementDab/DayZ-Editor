class EditorSearchObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		Object object = GetObjectUnderCursor();
		if (!object) {
			return true;
		}
		
		m_Editor.GetEditorHud().GetTemplateController().SearchBarData = object.GetType();
		m_Editor.GetEditorHud().GetTemplateController().NotifyPropertyChanged("SearchBarData");
		SetFocus(GetEditor().GetEditorHud().LeftbarSearchBar);
		return true;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_F };
	}
}