class EditorPlaceObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);		
		
		if (GetWidgetUnderCursor()) {
			return false;
		}
		
		array<Object> placing_objects = GetEditor().GetPlacingObjects();		
		foreach (Object placing_object: placing_objects) {			
			EditorObject editor_object = GetEditor().CreateObject(placing_object, EFE_DEFAULT);
			if (!editor_object) { 
				continue;
			}
			
			if (!KeyState(KeyCode.KC_LSHIFT)) { 
				GetEditor().RemoveFromHand(placing_object); 
			}
			
			editor_object.SetSelected(true);
		}		
		
		/*
		if (m_Editor.IsPromptedForObjectSelection()) {
			m_Editor.SatisfyObjectSelectionPrompt(Editor.ObjectUnderCursor);
			return true;
		}*/
		
		return false;
	}
	
	override bool CanExecute()
	{
		return GetEditor().IsPlacing();
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_PLACE_OBJECT";
	}
}