
class EditorExportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorEditBoxDialog file_dialog(GetName());
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
						
		EditorFileType file_type = GetFileType().Spawn();
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Export");
			return;
		}		
		
		file_name = "$profile:Editor/" + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(m_Editor);
		ExportSettings settings = new ExportSettings(); // todo
		file_type.Export(save_data, file_name, settings);		
		
		string message = string.Format("Exported %1 objects!", save_data.EditorObjects.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
	}
	
	typename GetFileType();
}


class EditorExportToInitFile: EditorExportCommandBase
{	
	override typename GetFileType() {
		return EditorExpansionFile;
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
		return EditorExpansionFile;
	}
	
	override string GetName() {
		return "Export to Terrain Builder";
	}
}

class EditorExportToVPP: EditorExportCommandBase
{
	override typename GetFileType() {
		return EditorExpansionFile;
	}
	
	override string GetName() {
		return "Export to VPP";
	}
}