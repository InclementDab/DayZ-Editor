class EditorShowCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		EditorAction hide_action = new EditorAction("Unshow", "Show");
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Show(true);
			
			hide_action.InsertUndoParameter(new Param1<int>(selected_object.GetID()));
			hide_action.InsertRedoParameter(new Param1<int>(selected_object.GetID()));
		}
		
		m_Editor.InsertAction(hide_action);
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