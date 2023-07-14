class EditorSearchCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		m_Editor.GetEditorHud().ShowCursor(true);
		SetFocus(m_Editor.GetEditorHud().LeftbarSearchBar);
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_SEARCH";
	}
}