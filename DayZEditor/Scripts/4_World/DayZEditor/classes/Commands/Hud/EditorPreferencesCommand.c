class EditorPreferencesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorPreferencesDialog dialog = new EditorPreferencesDialog("Editor Preferences");
		DialogResult result = dialog.ShowDialog();
		
		if (result != DialogResult.OK) {
			return;
		}
		
		// cf test
		//EditorSettings.Save(m_Editor.Settings, m_Editor.EditorSettingsFile);
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