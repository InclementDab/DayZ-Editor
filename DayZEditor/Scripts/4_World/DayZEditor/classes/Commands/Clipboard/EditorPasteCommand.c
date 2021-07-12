class EditorPasteCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorClipboard.Paste(m_Editor.CurrentMousePosition);
		return true;
	}

	override string GetName() 
	{
		return "#STR_EDITOR_PASTE";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:paste";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_V };
	}
}