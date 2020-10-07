class EditorImportCommandBase: EditorCommand
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
			EditorLog.Error("Invalid FileType in Import");
			return;
		}
		
		file_name = "$profile:Editor/" + file_name;
		EditorFileManager.GetSafeFileName(file_name, file_type.GetExtension());
		if (!FileExist(file_name)) {
			MessageBox.Show("File Not Found", "Could not find file " + file_name, MessageBoxButtons.OK);
			return;
		}
		
		EditorSaveData save_data = new EditorSaveData();
		ImportSettings settings = new ImportSettings(); // todo
		save_data = file_type.Import(file_name, settings);
		
		EditorObjectMap result = GetEditor().CreateObjects(save_data.EditorObjects, false);
		
		string message = string.Format("Loaded %1 objects!", result.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
	}
	
	typename GetFileType();
}


class EditorOpenCommand: EditorImportCommandBase
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		super.Call(sender, args);		

		/*
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Info("Loading Map %1", save_data.MapName);
			//delete m_EditorInstance;
			
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
		
			// Scuffed way of waiting for new Editor to be created. It doesnt work (obv)
			/*while (!m_EditorInstance) {
				Sleep(1);
			}
		}*/
		/*
		m_Editor.DeleteObjects(m_Editor.GetPlacedObjects(), false);
		foreach (int id, ref EditorObjectData data: save_data.EditorObjects) {
			m_Editor.CreateObject(data, false);
		}*/
		
		/*
		m_Editor.GetCamera().SetTransform(save_data.CameraPosition);
		
		string msg = string.Format("Loaded %1 objects!", save_data.EditorObjects.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(msg, COLOR_GREEN);
		EditorLog.Info(msg);*/
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