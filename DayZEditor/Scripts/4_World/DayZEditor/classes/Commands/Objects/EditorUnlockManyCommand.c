#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorUnlockManyCommand)]
#endif
class EditorUnlockManyCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (m_Editor.GetBrush() && m_Editor.GetBrush().IsInherited(EditorUnlockBrush)) {
			m_Editor.SetBrush(null);
		} else {
			m_Editor.GetEditorHud().GetTemplateController().BrushToggleButtonState = false;
			m_Editor.GetEditorHud().GetTemplateController().NotifyPropertyChanged("BrushToggleButtonState");
			m_Editor.SetBrush(new EditorUnlockBrush());
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
		return GetEditor().GetBrush() && GetEditor().GetBrush().IsInherited(EditorUnlockBrush);
	}
}