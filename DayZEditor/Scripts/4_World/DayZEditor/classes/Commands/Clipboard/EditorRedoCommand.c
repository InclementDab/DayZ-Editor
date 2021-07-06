class EditorRedoCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.Redo();
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_REDO";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:redo";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_Y };
	}
}