class EditorShowCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorAction hide_action = new EditorAction("Unshow", "Show");
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		GetEditor().CreateCheckpoint(selected_objects);
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Show(true);
		}
		
		return true;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_H };
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_SHOW";
	}
}