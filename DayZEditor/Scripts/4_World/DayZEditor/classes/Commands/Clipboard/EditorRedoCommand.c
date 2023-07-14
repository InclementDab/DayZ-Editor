class EditorRedoCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.Redo();
		return true;
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_REDO";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:redo";
	}
}