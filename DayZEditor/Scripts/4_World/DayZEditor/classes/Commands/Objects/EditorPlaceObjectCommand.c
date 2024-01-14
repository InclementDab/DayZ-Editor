class EditorPlaceObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);		
		
		array<Object> placing_objects = GetEditor().GetPlacingObjects();		
		foreach (Object placing_object: placing_objects) {			
			EditorObject editor_object = GetEditor().CreateObject(EditorObjectData.Create(placing_object));
			if (!editor_object) { 
				continue;
			}
			
			if (!KeyState(KeyCode.KC_LSHIFT)) { 
				GetEditor().RemoveFromHand(placing_object); 
			}
			
			GetEditor().SelectObject(editor_object);
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