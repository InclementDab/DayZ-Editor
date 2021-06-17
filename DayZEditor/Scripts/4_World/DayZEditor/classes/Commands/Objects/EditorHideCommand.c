class EditorHideCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		if (m_Editor.IsPlacing()) {
			return;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			selected_object.Show(false);
		}
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_H };
	}
	
	override string GetName() 
	{
		return "Hide";
	}
}