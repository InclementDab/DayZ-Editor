class EditorUnlockCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			GetEditor().UnlockObject(selected_object);
		}
	}
		
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_L };
	}
	
	override string GetName() 
	{
		return "Unlock";
	}
}