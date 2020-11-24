class EditorImportCommandBase: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorFileDialog file_dialog(GetName(), "*" + EditorFileType.Cast(GetFileType().Spawn()).GetExtension(), "", GetDialogButtonName());
		
		string file_name;
		if (file_dialog.ShowDialog(file_name) != DialogResult.OK) {
			return;
		}
		
		if (file_name == string.Empty) {
			MessageBox.Show("Error", "No file name specified!", MessageBoxButtons.OK);
			return;
		}
					
		ImportFile(file_name);
	}
	
	protected EditorSaveData ImportFile(string file_name, bool clear_before = false)
	{
		EditorFileType file_type = GetFileType().Spawn();
		if (!file_type) {
			EditorLog.Error("Invalid FileType in Import");
			return null;
		}
		
		file_name = "$profile:Editor/" + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		if (!FileExist(file_name)) {
			MessageBox.Show("File Not Found", "Could not find file " + file_name, MessageBoxButtons.OK);
			return null;
		}
		
		EditorSaveData save_data = new EditorSaveData();
		ImportSettings settings = new ImportSettings(); // todo
		save_data = file_type.Import(file_name, settings);
		
		if (clear_before) {
			m_Editor.Clear();
		}
		
		CF_ObjectManager.UnhideAllMapObjects();
				
		EditorLog.Info("Deleting %1 Objects", save_data.DeletedObjects.Count().ToString());
		foreach (int id: save_data.DeletedObjects) {
			if (!GetEditor().HideMapObject(id)) {
				EditorLog.Warning("Failed to delete building: %1", id.ToString());
				EditorMessageBox.ShowSynchronous("Error", string.Format("There was an error deleting object %1\nTry restarting the game and reloading the file", id), MessageBoxButtons.OK);
			}
		}
		
		EditorLog.Info("Creating %1 Objects", save_data.EditorObjects.Count().ToString());
		foreach (EditorObjectData data: save_data.EditorObjects) {
			GetEditor().CreateObject(data, false);
		}

		
		m_Editor.GetEditorHud().CreateNotification(string.Format("Loaded %1 objects!", save_data.EditorObjects.Count()), COLOR_GREEN);
		return save_data;
	}
	
	typename GetFileType();
	
	string GetDialogButtonName() {
		return "Import";
	}
}


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
			m_Editor.EditorSaveFile = file_name;
			m_Editor.GetCamera().SetPosition(save_data.CameraPosition);
		}
	}


	override string GetName() {
		return "Open";
	}

	override string GetIcon() {
		return "set:dayz_editor_gui image:open";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_O };
	}
	
	override typename GetFileType() {
		return EditorDZEFile;
	}
	
	override string GetDialogButtonName() {
		return "Open";
	}
}

class EditorImportFromInit: EditorImportCommandBase
{
	override string GetName() {
		return "Import from Init.c";
	}
	
	override typename GetFileType() {
		return EditorInitFile;
	}
}

class EditorImportFromCOM: EditorImportCommandBase
{
	override string GetName() {
		return "Import from COM";
	}
	
	override typename GetFileType() {
		return EditorCOMFile;
	}
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
		return EditorTerrainBuilderFile;
	}
}

class EditorImportFromVPP: EditorImportCommandBase
{
	override string GetName() {
		return "Import from .VPP";
	}
	
	override typename GetFileType() {
		return EditorVPPFile;
	}
}