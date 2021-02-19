class EditorLockCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			GetEditor().LockObject(selected_object);
		}
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_L };
	}
	
	override string GetName() 
	{
		return "Lock";
	}
}