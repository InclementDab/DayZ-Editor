class EditorNewCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);	
		
		GetEditor().GetEditorHud().ShowFileDialog(GetName(), EditorDZEFile, ScriptCaller.Create(OnFileCreated), eDialogMode.NEW, eDialogFlags.WARN_ON_OVERWRITE, GetEditor().GetSaveFile());	
		
		return true;
	}
		
	protected void OnFileCreated(string file_name, eDialogExtraSetting extra_settings)
	{
		EditorFileManager.GetSafeFileName(file_name, ".dze");
		// a terrible hack. the file dialogs need to return a fille path to the file
		if (!SystemPath.IsPathRooted(file_name)) {
			file_name = SystemPath.Combine(Editor.ROOT_DIRECTORY, file_name);
		}
				
		m_Editor.Clear();
		m_Editor.SetSaveFile(file_name);
		m_Editor.CreateSaveData(); // spaghetti but it creates a new timestamped file data internally
		
		GetEditor().GetEditorHud().CreateNotification(string.Format("File %1 Created", File.GetName(file_name)));
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