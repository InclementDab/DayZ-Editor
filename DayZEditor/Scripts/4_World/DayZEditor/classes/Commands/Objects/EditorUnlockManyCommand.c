#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorUnlockManyCommand)]
#endif
class EditorUnlockManyCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
				
		if (m_Editor.GetEditorHud().GetBrushState() && m_Editor.Brush.IsInherited(EditorUnlockBrush)) {
			m_Editor.GetEditorHud().SetBrushState(0);
		} else {
			m_Editor.GetEditorHud().SetBrushByTypename(EditorUnlockBrush);
			m_Editor.GetEditorHud().SetBrushState(1);
		}
		
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
		return GetEditor().Brush && GetEditor().Brush.IsInherited(EditorUnlockBrush);
	}
}