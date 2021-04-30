class EditorOpenCommand: EditorImportCommandBase
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorFileDialog file_dialog(GetName(), "*.dze", "", GetDialogButtonName());
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		EditorSaveData save_data = ImportFile(file_name, true);
		/*
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Info("Loading Map %1", save_data.MapName);
			//delete m_EditorInstance;
			
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
		
			// Scuffed way of waiting for new Editor to be created. It doesnt work (obv)
			/*while (!m_EditorInstance) {
				Sleep(1);
			}
		}
*/		
		if (save_data) {
			m_Editor.SetSaveFile(file_name);
			m_Editor.GetCamera().SetPosition(save_data.CameraPosition);
		}
	}

	override bool UnhideMapObjects()
	{
		return true;
	}

	override string GetName() 
	{
		return "Open";
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
		return "Open";
	}
}