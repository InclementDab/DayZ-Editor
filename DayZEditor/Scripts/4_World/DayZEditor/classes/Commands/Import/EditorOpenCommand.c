class EditorOpenCommand: EditorImportCommandBase
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		//super.Execute(sender, args);
		GetEditor().GetEditorHud().ShowFileDialog(GetName(), GetFileType(), ScriptCaller.Create(OnOpenFileSelected), eDialogMode.OPEN, eDialogFlags.ALLOW_DOUBLE_CLICK, GetEditor().GetSaveFile());
		
		return true;
	}
	
	protected void OnOpenFileSelected(string file_name, eDialogExtraSetting extra_settings)
	{
		GetEditor().LoadSaveData(ImportFile(file_name), true);
		GetEditor().SetSaveFile(file_name);
	}
			
	override string GetName() 
	{
		return "#STR_EDITOR_OPEN";
	}

	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:open";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_O };
	}
	
	override typename GetFileType() 
	{
		return EditorDZEFile;
	}
	
	override string GetDialogButtonName() 
	{
		return "#STR_EDITOR_OPEN";
	}
}