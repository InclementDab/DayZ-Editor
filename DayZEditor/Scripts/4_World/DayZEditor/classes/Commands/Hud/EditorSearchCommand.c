class EditorSearchCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		m_Editor.GetEditorHud().ShowCursor(true);
		SetFocus(m_Editor.GetEditorHud().LeftbarSearchBar);
		return true;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_F };
	}
}