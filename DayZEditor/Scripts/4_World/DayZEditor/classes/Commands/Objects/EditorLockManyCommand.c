#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorLockManyCommand)]
#endif
class EditorLockManyCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (m_Editor.GetEditorHud().GetBrushState() && m_Editor.Brush.IsInherited(EditorLockBrush)) {
			m_Editor.GetEditorHud().SetBrushState(0);
		} else {
			m_Editor.GetEditorHud().SetBrushByTypename(EditorLockBrush);
			m_Editor.GetEditorHud().SetBrushState(1);
		}
		
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
		return GetEditor().Brush && GetEditor().Brush.IsInherited(EditorLockBrush);
	}
}