#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorLockManyCommand)]
#endif
class EditorLockManyCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (m_Editor.GetBrush() && m_Editor.GetBrush().IsInherited(EditorLockBrush)) {
			m_Editor.SetBrush(null);
		} else {
			m_Editor.GetEditorHud().GetTemplateController().BrushToggleButtonState = false;
			m_Editor.GetEditorHud().GetTemplateController().NotifyPropertyChanged("BrushToggleButtonState");
			m_Editor.SetBrush(new EditorLockBrush());
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
		return GetEditor().GetBrush() && GetEditor().GetBrush().IsInherited(EditorLockBrush);
	}
}