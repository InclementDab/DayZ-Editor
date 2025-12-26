#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorLockManyCommand)]
#endif
class EditorLockManyCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		m_Editor.LockMode = !m_Editor.LockMode;
		m_Editor.UnlockMode = false;
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_LOCK";
	}

	override Symbols GetSymbol()
	{
		return Symbols.LOCK;
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.ORANGE_RED;
	}
	
	override bool IsToggled()
	{
		return GetEditor().LockMode;
	}
}