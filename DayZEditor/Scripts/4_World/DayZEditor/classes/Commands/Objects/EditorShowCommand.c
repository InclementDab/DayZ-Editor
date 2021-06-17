class EditorShowCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Show(true);
		}
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_H };
	}
	
	override string GetName() 
	{
		return "Show";
	}
}