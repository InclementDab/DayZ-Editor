class EditorLockCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			GetEditor().LockObject(selected_object);
		}
		
		return true;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_L };
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_LOCK";
	}
}