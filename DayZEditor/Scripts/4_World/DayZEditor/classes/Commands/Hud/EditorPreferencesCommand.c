class EditorPreferencesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorPreferencesDialog dialog = new EditorPreferencesDialog("Editor Preferences", GetName());
		DialogResult result = dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "General"; // Just a catch-all default
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:settings";
	}
}

class EditorPreferencesGeneralCommand: EditorPreferencesCommand
{
	override string GetName()
	{
		return "General";
	}
	
	override string GetIcon() 
	{
		return "";
	}
}

class EditorPreferencesBrushesCommand: EditorPreferencesCommand
{
	override string GetName()
	{
		return "Brushes";
	}
	
	override string GetIcon() 
	{
		return "";
	}
}

class EditorPreferencesThemesCommand: EditorPreferencesCommand
{
	override string GetName()
	{
		return "Themes";
	}
	
	override string GetIcon() 
	{
		return "";
	}
}

class EditorPreferencesAdvancedCommand: EditorPreferencesCommand
{
	override string GetName()
	{
		return "Advanced";
	}
	
	override string GetIcon() 
	{
		return "";
	}
}