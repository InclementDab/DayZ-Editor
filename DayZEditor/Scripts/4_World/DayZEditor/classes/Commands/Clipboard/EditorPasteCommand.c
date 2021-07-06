class EditorPasteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{	
		EditorClipboard.Paste(m_Editor.CurrentMousePosition);
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