
class EditorCommand: RelayCommand
{	
	protected Editor m_Editor;
	protected EditorHud m_EditorHud;
	protected EditorHudController m_EditorHudController;
	
	string Text;
	
	void EditorCommand()
	{
		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHud = m_Editor.GetEditorHud();
			if (m_EditorHud) {
				m_EditorHudController = m_EditorHud.GetEditorHudController();
			}
		}
	}

	override bool Execute(Class sender, CommandArgs args) 
	{
		EditorLog.Trace("EditorCommand::Execute");
		super.Execute(sender, args);
		if (EditorUIManager.CurrentMenu)
			delete EditorUIManager.CurrentMenu;
		
		// Needs to be since we do ShowDialog alot
		thread Call();
		return true;
	}
	
	override void CanExecuteChanged(bool state) 
	{
		Widget root = m_ViewBinding.GetLayoutRoot();
		if (state) {
			root.FindAnyWidget("EditorMenuItemLabel").SetAlpha(1);
			root.FindAnyWidget("EditorMenuItemIcon").SetAlpha(1);
		} else {
			root.FindAnyWidget("EditorMenuItemLabel").SetAlpha(0.3);
			root.FindAnyWidget("EditorMenuItemIcon").SetAlpha(0.3);
		}
		
		root.Enable(state);
	}
	
	
	protected void Call();
	
	string GetName() {
		return string.Empty;
	}
	
	string GetIcon() {
		return string.Empty;
	}
	
	string GetKeyDisplay() {
		return string.Empty;
	}
}

class EditorNewCommand: EditorCommand
{
	protected override void Call() 
	{
		EditorEditBoxDialog edit_dialog = new EditorEditBoxDialog(null, "New File...");
		string edit_data;
		DialogResult result = edit_dialog.ShowDialog(edit_data);
		if (result != DialogResult.OK) return;
	}
		
	override string GetName() {
		return "New";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + N";
	}
}

class EditorSaveCommand: EditorCommand
{
	protected override void Call() 
	{
		EditorLog.Trace("EditorSaveCommand");
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(m_Editor);
		
		if (m_Editor.GetSaveFile() == string.Empty) {
			//EditorFileSaveDialog save_dialog = new EditorFileSaveDialog();
			//save_dialog.ShowDialog();
		}
		
		DialogResult result = EditorMessageBox.Show("Save", "Are you sure?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;			
			
		FileDialogResult file_result = EditorFileManager.Save(save_data, "$profile:/Editor/SaveData.dze");
		if (file_result != FileDialogResult.SUCCESS) {
			m_EditorHud.CreateNotification(typename.EnumToString(FileDialogResult, file_result), COLOR_RED);
			return;
		} 
		
		m_EditorHud.CreateNotification("Saved!", COLOR_GREEN);
		EditorLog.Info("Saved %1 objects!", save_data.EditorObjects.Count().ToString());
	}
		
	override string GetName() {
		return "Save";
	}

	override string GetKeyDisplay() {
		return "Ctrl + S";
	}
	
	override string GetIcon() {
		return "DayZEditor/gui/icons/icon_save.edds";
	}
}

class EditorSaveAsCommand: EditorCommand
{
	protected override void Call() 
	{
		EditorFileSaveDialog save_dialog = new EditorFileSaveDialog(null);
		save_dialog.SetWorldData(EditorSaveData.CreateFromEditor(GetEditor()));
		string file = save_dialog.ShowFileDialog();
	}
		
	override string GetName() {
		return "Save As...";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + Shift + S";
	}
}

class EditorOpenCommand: EditorCommand
{
	void EditorOpenCommand() 
	{
		SetCanExecute(false);
	}
	
	protected override void Call() 
	{
		DialogResult result = EditorMessageBox.Show("Open", "Are you sure?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		
		EditorSaveData save_data;
		EditorFileManager.Open(save_data, "$profile:/Editor/SaveData.dze");
		
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Info("Loading Map %1", save_data.MapName);
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
		}
		
		m_Editor.DeleteObjects(m_Editor.GetPlacedObjects(), false);
		m_Editor.CreateObjects(save_data.EditorObjects, false);
		string msg = string.Format("Loaded %1 objects!", save_data.EditorObjects.Count().ToString());
		m_EditorHud.CreateNotification(msg, COLOR_GREEN);
		EditorLog.Info(msg);
	}
			
	override string GetName() {
		return "Open";
	}
		
	override string GetIcon() {
		return "DayZEditor/gui/icons/icon_folder.edds";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + O";
	}
}

class EditorCloseCommand: EditorCommand
{
	protected override void Call() 
	{
		EditorEnvironmentDialog dialog = new EditorEnvironmentDialog(null, "Weather Controller");
		DialogResult result = dialog.ShowDialog();
	}
			
	override string GetName() {
		return "Close";
	}
		
	override string GetIcon() {
		return "dayz_editor_gui:building_icon";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + W";
	}
}

class EditorExitCommand: EditorCommand
{	
	protected override void Call() {
		GetGame().LogoutRequestTime();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().CreateLogoutMenu, GetGame().GetUIManager().GetMenu());
	}
	
	override string GetName() {
		return "Exit";
	}
		
	override string GetIcon() {
		return "dayz_editor_gui:building_icon";
	}
	
	override string GetKeyDisplay() {
		return "Alt + F4";
	}
	
}

class EditorUndoCommand: EditorCommand
{
	void EditorUndoCommand(ref array<KeyCode> input_gestures = null) {
		SetCanExecute(GetEditor().GetObjectManager().CanUndo());
	}
	
	protected override void Call() {
		GetEditor().GetObjectManager().Undo();
	}
	
	override string GetName() {
		return "Undo";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + Z";
	}
	
	override string GetIcon() {
		
	}
}

class EditorRedoCommand: EditorCommand
{
	void EditorRedoCommand(ref array<KeyCode> input_gestures = null) {
		SetCanExecute(GetEditor().GetObjectManager().CanRedo());
	}
	
	protected override void Call() {
		GetEditor().GetObjectManager().Redo();
	}
		
	override string GetName() {
		return "Redo";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + Y";
	}
}

class EditorSelectAllCommand: EditorCommand
{
	protected override void Call() 
	{
		EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
		foreach (EditorObject eo: placed_objects)
			GetEditor().SelectObject(eo);
	}
		
	override string GetName() {
		return "Select All...";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + A";
	}
}

class EditorDeleteCommand: EditorCommand
{
	protected override void Call() {
		m_Editor.DeleteObjects(m_Editor.GetSelectedObjects());		
	}
		
	override string GetName() {
		return "Delete";
	}
	
	override string GetKeyDisplay() {
		return "Delete";
	}
}

class EditorExportCommand: EditorCommand
{
	protected override void Call() 
	{
		DialogResult result = EditorMessageBox.Show("Export", "Export file expansion_export.map?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(m_Editor);
		
		ExportSettings settings = new ExportSettings();
		settings.ExportFileMode = ExportMode.EXPANSION;
		FileDialogResult file_result = EditorFileManager.Export(save_data, "$profile:/Editor/expansion_export.map", settings);
		if (file_result != FileDialogResult.SUCCESS) {
			m_EditorHud.CreateNotification(typename.EnumToString(FileDialogResult, file_result), COLOR_RED);
			return;
		}
		
		string message = string.Format("Exported %1 objects!", save_data.EditorObjects.Count().ToString());
		m_EditorHud.CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
	}

	override string GetName() {
		return "Export";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + E";
	}
}

class EditorImportCommand: EditorCommand
{
	protected override void Call() 
	{
		DialogResult result = EditorMessageBox.Show("Import", "Import file expansion_import.map?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		

		EditorSaveData save_data();
		EditorFileManager.Import(save_data, "$profile:/Editor/expansion_import.map", ImportMode.EXPANSION);
		string message = string.Format("Imported %1 objects!", save_data.EditorObjects.Count().ToString());
		m_EditorHud.CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
		
		m_Editor.CreateObjects(save_data.EditorObjects);
	}

	override string GetName() {
		return "Import";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + I";
	}
}

class EditorCutCommand: EditorCommand
{
	void EditorCutCommand(ref array<KeyCode> input_gestures = null) {
		SetCanExecute(GetEditor().GetSelectedObjects().Count() > 0);
	}
	
	protected override void Call() {
		GetEditor().Cut(GetEditor().GetSelectedObjects());
	}

	override string GetName() {
		return "Cut";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + X";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:cut_icon";
	}
}

class EditorCopyCommand: EditorCommand
{
	void EditorCopyCommand(ref array<KeyCode> input_gestures = null) {
		SetCanExecute(GetEditor().GetSelectedObjects().Count() > 0);
	}
	
	protected override void Call() {
		GetEditor().Copy(GetEditor().GetSelectedObjects());
	}

	override string GetName() {
		return "Copy";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + C";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:copy_icon";
	}
}

class EditorPasteCommand: EditorCommand
{
	void EditorPasteCommand(ref array<KeyCode> input_gestures = null) {
		string clipboard_text;
		GetGame().CopyFromClipboard(clipboard_text);
		SetCanExecute(clipboard_text != string.Empty);
	}
	
	protected override void Call() {
		GetEditor().Paste(GetEditor().CurrentMousePosition);
	}

	override string GetName() {
		return "Paste";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + V";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:paste_icon";
	}
}

class EditorPreferencesCommand: EditorCommand
{
	protected override void Call() {
		
	}

	override string GetName() {
		return "Preferences...";
	}
	
	override string GetIcon() {
		return "set:dayz_gui image:settings";
	}
}

class EditorEnvironmentControlCommand: EditorCommand
{
	protected override void Call() {
		EditorEnvironmentDialog environment_dialog = new EditorEnvironmentDialog(null);
		environment_dialog.ShowDialog();
	}
	
	override string GetName() {
		return "Environment";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + Shift + I";
	}
	
	override string GetIcon() {
		return "set:dayz_gui image:icon_sun";
	}
}

class EditorCameraControlsCommand: EditorCommand
{
	protected override void Call() {
		EditorCameraDialog cam_dialog(null);
		cam_dialog.SetEditorCamera(m_Editor.GetCamera());
		cam_dialog.ShowDialog();
	}

	override string GetName() {
		return "Camera";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + Shift + T";
	}
}

class EditorReloadHudCommand: EditorCommand
{
	protected override void Call() {
		GetEditor().ReloadHud();
	}

	override string GetName() {
		return "Reload UI";
	}
	
	override string GetIcon() {
		return "set:dayz_gui image:icon_refresh";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + Shift + U";
	}
}

class EditorLootEditorCommand: EditorCommand
{
	protected override void Call() {
		GetEditor().EditLootSpawns(GetEditor().GetSelectedObjects().GetElement(0).GetType());
	}
	
	override string GetName() {
		return "Edit Loot Spawns";
	}
}

class EditorObjectPropertiesCommand: EditorCommand
{
	protected override void Call() {
		EditorObjectPropertiesDialog properties_dialog(null);
		properties_dialog.SetEditorObject(m_Editor.GetSelectedObjects().GetElement(0));
		properties_dialog.ShowDialog();
	}
	
	override string GetName() {
		return "Properties";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + T";
	}
}

class EditorLoadMapCommand: EditorCommand
{	
	protected override void Call()
	{
		EditorMapSelectDialog select_dialog = new EditorMapSelectDialog(null, GetName());
		string selected_map;
		DialogResult result = select_dialog.ShowDialog(selected_map);
		
		if (selected_map != string.Empty && result == DialogResult.OK) {
			EditorLog.Info("Loading Map %1", selected_map);
			GetGame().PlayMission(CreateEditorMission(selected_map));
		}
	}
	
	override string GetName() {
		return "Select Map...";
	}
}

class EditorReloadBrushesCommand: EditorCommand
{
	protected override void Call()
	{
		m_EditorHudController.GetToolbarController().BrushTypeBoxData.Clear();
		m_EditorHudController.GetToolbarController().ReloadBrushes(m_Editor.EditorBrushFile);
	}
	
	override string GetName() {
		return "Reload Brushes";
	}
}