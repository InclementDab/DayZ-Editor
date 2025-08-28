class EditorHideCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		if (m_Editor.IsPlacing()) {
			return false;
		}
		
		EditorAction hide_action = new EditorAction("Show", "Unshow");
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			selected_object.Show(false);
			hide_action.InsertUndoParameter(new Param1<int>(selected_object.GetID()));
			hide_action.InsertRedoParameter(new Param1<int>(selected_object.GetID()));
		}
		
		m_Editor.InsertAction(hide_action);
		
		return true;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_H };
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_HIDE";
	}

	override Symbols GetSymbol()
	{
		return Symbols.EYE_SLASH;
	}
}