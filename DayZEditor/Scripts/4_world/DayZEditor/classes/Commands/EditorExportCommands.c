
class EditorExportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorEditBoxDialog file_dialog(GetName());
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) == DialogResult.OK) {
			file_name = "$profile:Editor/" + file_name;
			EditorFileManager.Export(EditorSaveData.CreateFromEditor(m_Editor), file_name, GetExportSettings());
		}
	}
	
	ExportSettings GetExportSettings();
}


class EditorExportToInitFile: EditorExportCommandBase
{	
	override ExportSettings GetExportSettings() {
		ExportSettings export_settings();
		export_settings.ExportFileMode = ExportMode.COMFILE;
		return export_settings;
	}
	
	override string GetName() {
		return "Export to init.c";
	}
}

class EditorExportToExpansion: EditorExportCommandBase
{	
	override ExportSettings GetExportSettings() {
		ExportSettings export_settings();
		export_settings.ExportFileMode = ExportMode.TERRAINBUILDER;
		return export_settings;
	}
	
	override string GetName() {
		return "Export to .map";
	}
}

class EditorExportToTerrainBuilder: EditorExportCommandBase
{	
	override ExportSettings GetExportSettings() {
		ExportSettings export_settings();
		export_settings.ExportFileMode = ExportMode.TERRAINBUILDER;
		return export_settings;
	}
	
	override string GetName() {
		return "Export to Terrain Builder";
	}
}

class EditorExportToVPP: EditorExportCommandBase
{
	override ExportSettings GetExportSettings() {
		ExportSettings export_settings();
		export_settings.ExportFileMode = ExportMode.VPP;
		return export_settings;
	}
	
	override string GetName() {
		return "Export to VPP";
	}
}