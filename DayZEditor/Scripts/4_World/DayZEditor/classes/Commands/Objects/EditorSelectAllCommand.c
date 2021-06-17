class EditorSelectAllCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorObjectMap placed_objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject eo: placed_objects)
			m_Editor.SelectObject(eo);
	}
		
	override string GetName() 
	{
		return "Select All...";
	}
		
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_A };
	}
}