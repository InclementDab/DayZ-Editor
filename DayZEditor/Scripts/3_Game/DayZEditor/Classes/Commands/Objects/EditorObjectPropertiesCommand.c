class EditorObjectPropertiesCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{		
		EditorObjectPropertiesDialog properties_dialog(GetName(), EditorObject.SelectedObjects);
		properties_dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_PROPERTIES";
	}
}