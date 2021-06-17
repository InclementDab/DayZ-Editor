class EditorNewCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorEditBoxDialog edit_dialog = new EditorEditBoxDialog("New File...");
		string file_name;
		DialogResult result = edit_dialog.ShowDialog(file_name);
		if (result != DialogResult.OK) { 
			return;
		}
		
		EditorFileManager.GetSafeFileName(file_name, ".dze");
		
		// Only supporting new in root dir atm
		if (FileExist("$profile:Editor/" + file_name)) {
			if (MessageBox.Show("Are you sure?", "File " + file_name + " already exists. Overwrite?", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
				return;
			}
		}
		
		m_Editor.Clear();
		m_Editor.SetSaveFile(file_name);
		
		GetEditor().GetEditorHud().CreateNotification("New File Created", COLOR_GREEN);
	}
		
	override string GetName() 
	{
		return "New";
	}

	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:new_alt";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_N };
	}
}