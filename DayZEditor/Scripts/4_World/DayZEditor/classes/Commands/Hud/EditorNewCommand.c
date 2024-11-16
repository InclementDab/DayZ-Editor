class EditorNewCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorHud.CurrentDialog = new EditorFileDialog(GetName(), ScriptCaller.Create(OnNewDialog), "*.dze", GetEditor().GetSaveFile(), "#STR_EDITOR_NEW");		
		EditorHud.CurrentDialog.GetLayoutRoot().Show(true);
		return true;
	}
	
	protected void OnNewDialog(string file)
	{
		EditorFileManager.GetSafeFileName(file, ".dze");
		
		// Only supporting new in root dir atm
		if (FileExist(file)) {
			//GetEditorHud().ShowMessageBox()
			// if (MessageBox.Show("Are you sure?", "File " + file + " already exists. Overwrite?", MessageBoxButtons.OKCancel) == DialogResult.Cancel) {
			// 	return;
			// }
		}
		
		m_Editor.Clear();
		m_Editor.SetSaveFile(file);
		
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