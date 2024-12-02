class EditorObjectPropertiesCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{		
		EditorObjectPropertiesDialog properties_dialog(GetName(), m_Editor.GetSelectedObjects().GetValueArray());
		properties_dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_PROPERTIES";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_T };
	}
}