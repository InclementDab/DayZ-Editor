class EditorPlaceObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (m_Editor.IsPlacing()) {
			m_Editor.PlaceObject();
			return true;
		}
		
		if (m_Editor.IsPromptedForObjectSelection()) {
			m_Editor.SatisfyObjectSelectionPrompt(Editor.ObjectUnderCursor);
			return true;
		}
		
		m_Editor.ClearSelection();
		return false;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_PLACE_OBJECT";
	}
}