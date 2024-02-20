class EditorSelectAllCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap placed_objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject eo: placed_objects) {
			m_Editor.SelectObject(eo);
		}
		
		return true;
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_SELECT_ALL";
	}
		
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_A };
	}
}