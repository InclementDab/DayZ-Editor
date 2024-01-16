class EditorPreferencesCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		OpenPreferences("#STR_EDITOR_GENERAL");
	}
	
	void OpenPreferences(string name)
	{
		thread _OpenPreferences(name);
	}
	
	private void _OpenPreferences(string name)
	{
		EditorPreferencesDialog dialog = new EditorPreferencesDialog(GetName(), name);
		DialogResult result = dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_PREFERENCES";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:settings";
	}
}