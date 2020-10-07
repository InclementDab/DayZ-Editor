class EditorImportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorEditBoxDialog file_dialog(GetName());
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		file_name = "$profile:Editor/" + file_name;
	
		if (!FileExist(file_name)) {
			MessageBox.Show("File Not Found", "Could not find file " + file_name, MessageBoxButtons.OK);
			return;
		}
				
		EditorFileType file_type = GetFileType().Spawn();
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Import");
			return;
		}
		
		FileHandle handle = OpenFile(file_name, FileMode.READ);
		if (!handle) {
			EditorLog.Error("File in use %1", file_name);
			return;
		}
		
		EditorSaveData save_data = new EditorSaveData();
		ImportSettings settings = new ImportSettings(); // todo
		save_data = file_type.Import(handle, settings);
		
		
		EditorObjectMap result = GetEditor().CreateObjects(save_data.EditorObjects, false);
		
		string message = string.Format("Imported %1 objects!", result.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
		
		CloseFile(handle);
	}
	
	typename GetFileType();
}

class EditorImportFromExpansion: EditorImportCommandBase
{
	override string GetName() {
		return "Import from .map";
	}
	
	override typename GetFileType() {
		return EditorExpansionFile;
	}
	
}

class EditorImportFromTerrainBuilder: EditorImportCommandBase
{
	override string GetName() {
		return "Import from Terrain Builder";
	}
	
	override typename GetFileType() {
		return EditorExpansionFile;
	}
}

class EditorImportFromVPP: EditorImportCommandBase
{
	override string GetName() {
		return "Import from .VPP";
	}
	
	override typename GetFileType() {
		return EditorExpansionFile;
	}
}