class EditorImportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorEditBoxDialog file_dialog(GetName());
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) == DialogResult.OK) {
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
			
			EditorSaveData save_data = new EditorSaveData();
			ImportSettings settings = new ImportSettings(); // todo
			file_type.Import(save_data, file_name, settings);
			
			
			string message = string.Format("Imported %1 objects!", save_data.EditorObjects.Count().ToString());
			m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
			EditorLog.Info(message);
			
			foreach (EditorObjectData eo: save_data.EditorObjects) {
				GetEditor().CreateObject(eo, false);
			}
			
		}
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