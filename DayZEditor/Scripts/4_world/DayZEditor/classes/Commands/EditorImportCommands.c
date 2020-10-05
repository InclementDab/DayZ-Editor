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
			
			EditorSaveData save_data = new EditorSaveData();
			EditorFileManager.Import(save_data, file_name, GetImportMode());
			
			string message = string.Format("Imported %1 objects!", save_data.EditorObjects.Count().ToString());
			m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
			EditorLog.Info(message);
			
			Print(save_data.EditorObjects.Count());
			foreach (EditorObjectData eo: save_data.EditorObjects) {
				GetEditor().CreateObject(eo, false);
			}
			
		}
	}
	
	ImportMode GetImportMode();
}

class EditorImportFromExpansion: EditorImportCommandBase
{
	override string GetName() {
		return "Import from .map";
	}
	
	override ImportMode GetImportMode() {
		return ImportMode.EXPANSION;
	}
}

class EditorImportFromTerrainBuilder: EditorImportCommandBase
{
	override string GetName() {
		return "Import from Terrain Builder";
	}
	
	override ImportMode GetImportMode() {
		return ImportMode.TERRAINBUILDER;
	}
}

class EditorImportFromVPP: EditorImportCommandBase
{
	override string GetName() {
		return "Import from .VPP";
	}
	
	override ImportMode GetImportMode() {
		return ImportMode.VPP;
	}
}