
class EditorExportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorExportDialog file_dialog(GetName(), "File", "", GetDialogButtonName());
		
		string file_name;
		ExportSettings export_settings = new ExportSettings();
		if (file_dialog.ShowDialog(file_name, export_settings) != DialogResult.OK) {
			return;
		}
				
		if (file_name == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
			return;
		}

		ExportFile(file_name, export_settings);
	}
	
	protected void ExportFile(string file_name, ExportSettings export_settings)
	{
		EditorFileType file_type = GetFileType().Spawn();
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Export");
			return;
		}
		
		file_name = "$profile:Editor/" + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(m_Editor, export_settings.ExportSelectedOnly);
		
		file_type.Export(save_data, file_name, export_settings);		
		
		string message = string.Format("Saved %1 objects!", save_data.EditorObjects.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
	}
	
	typename GetFileType();
	
	string GetDialogButtonName() {
		return "Export";
	}
}

class EditorSaveCommand: EditorExportCommandBase
{		
	override void Call(Class sender, CommandArgs args)
	{
		EditorLog.Trace("EditorSaveCommand");
		
		if (m_Editor.EditorSaveFile == string.Empty) {
			EditorFileDialog file_dialog(GetName(), "File", "", GetDialogButtonName());
			string file_name;
			if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
				return;
			}
			
			m_Editor.EditorSaveFile = file_name;
		}
		
		ExportFile(m_Editor.EditorSaveFile, new ExportSettings());
	}
	
	override string GetName() {
		return "Save";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:save";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_S };
	}
	
	override typename GetFileType() {
		return EditorDZEFile;
	}
	
	override string GetDialogButtonName() {
		return "Save";
	}
}

class EditorSaveAsCommand: EditorExportCommandBase
{		
	override string GetName() {
		return "Save As...";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_S };
	}
	
	override typename GetFileType() {
		return EditorDZEFile;
	}
	
	override string GetDialogButtonName() {
		return "Save";
	}
}



class EditorExportToInitFile: EditorExportCommandBase
{	
	override typename GetFileType() {
		return EditorCOMFile;
	}
	
	override string GetName() {
		return "Export to init.c";
	}
}

class EditorExportToExpansion: EditorExportCommandBase
{	
	override typename GetFileType() {
		return EditorExpansionFile;
	}
	
	override string GetName() {
		return "Export to .map";
	}
}

class EditorExportToTerrainBuilder: EditorExportCommandBase
{	
	override typename GetFileType() {
		return EditorTerrainBuilderFile;
	}
	
	override string GetName() {
		return "Export to Terrain Builder";
	}
}

class EditorExportToVPP: EditorExportCommandBase
{
	override typename GetFileType() {
		return EditorVPPFile;
	}
	
	override string GetName() {
		return "Export to VPP";
	}
}