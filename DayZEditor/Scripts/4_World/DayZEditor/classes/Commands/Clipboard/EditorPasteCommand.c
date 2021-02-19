class EditorPasteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{	
		EditorClipboard.Paste(m_Editor.CurrentMousePosition);
	}

	override string GetName() 
	{
		return "Paste";
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