class EditorRedoCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.Redo();
	}
		
	override string GetName() 
	{
		return "Redo";
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