class EditorPreferencesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		OpenPreferences();
	}
	
	void OpenPreferences(string name = "General")
	{
		thread _OpenPreferences(name);
	}
	
	private void _OpenPreferences(string name)
	{
		EditorPreferencesDialog dialog = new EditorPreferencesDialog("Editor Preferences", name);
		DialogResult result = dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "Preferences...";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:settings";
	}
}