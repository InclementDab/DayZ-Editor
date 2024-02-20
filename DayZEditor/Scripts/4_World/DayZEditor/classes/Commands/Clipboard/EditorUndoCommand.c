class EditorUndoCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.Undo();
		return true;
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