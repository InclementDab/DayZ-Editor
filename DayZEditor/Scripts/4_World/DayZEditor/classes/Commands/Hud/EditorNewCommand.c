class EditorNewCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorFileDialog_DEPRECATED edit_dialog = new EditorFileDialog_DEPRECATED(GetName(), "*.dze", GetEditor().GetSaveFile(), "#STR_EDITOR_NEW");
		string file_name;
		DialogResult result = edit_dialog.ShowDialog(file_name);
		if (result != DialogResult.OK) { 
			return;
		}
		
		EditorFileManager.GetSafeFileName(file_name, ".dze");
		// a terrible hack. the file dialogs need to return a fille path to the file
		if (!SystemPath.IsPathRooted(file_name)) {
			file_name = SystemPath.Combine(Editor.ROOT_DIRECTORY, file_name);
		}
		
		// Only supporting new in root dir atm
		if (FileExist(file_name)) {
			if (MessageBox.Show("Are you sure?", "File " + file_name + " already exists. Overwrite?", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
				return;
			}
		}
		
		m_Editor.Clear();
		m_Editor.SetSaveFile(file_name);
		m_Editor.CreateSaveData(); // spaghetti but it creates a new timestamped file data internally
		
		GetEditor().GetEditorHud().CreateNotification("New File Created");
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_NEW";
	}

	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:new_alt";
	}

	override Symbols GetSymbol()
	{
		return Symbols.FILE;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_N };
	}
}