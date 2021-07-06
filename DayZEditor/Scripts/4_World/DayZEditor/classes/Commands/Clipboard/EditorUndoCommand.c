class EditorUndoCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.Undo();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_UNDO";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:undo";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_Z };
	}
}