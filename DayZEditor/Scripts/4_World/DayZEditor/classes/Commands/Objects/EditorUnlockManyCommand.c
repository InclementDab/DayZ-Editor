#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorUnlockManyCommand)]
#endif
class EditorUnlockManyCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
				
		m_Editor.UnlockMode = !m_Editor.UnlockMode;
		m_Editor.LockMode = false;
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_UNLOCK";
	}

	override Symbols GetSymbol()
	{
		return Symbols.LOCK_OPEN;
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.PALE_GREEN;
	}
	
	override bool IsToggled()
	{
		return GetEditor().UnlockMode;
	}
}