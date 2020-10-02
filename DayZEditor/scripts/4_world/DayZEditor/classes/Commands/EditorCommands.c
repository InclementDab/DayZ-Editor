
class EditorCommand: RelayCommand
{	
	protected Editor editor;
	
	string Text;

	override bool Execute(Class sender, CommandArgs args) 
	{
		EditorLog.Trace("EditorCommand::Execute");
		super.Execute(sender, args);
		if (EditorUIManager.CurrentMenu)
			delete EditorUIManager.CurrentMenu;		
		// Needs to be since we do ShowDialog alot
		if (GetEditor()) {
			thread Call(GetEditor());
		} else {
			EditorLog.Error("EditorCommand::Editor was null!");
		}
		return true;
	}
	
	override void CanExecuteChanged(bool state) 
	{
		//EditorLog.Trace("CanExecuteChanged: %1 - %2", state.ToString(), m_ViewBinding.GetLayoutRoot().GetName());
		
		if (m_ViewBinding) {
			
			Widget root = m_ViewBinding.GetLayoutRoot();
			if (state) {
				root.SetAlpha(1);
			} else {
				root.SetAlpha(0.25);
			}
			
			root.Enable(state);			
		}		
	}
	
	
	protected void Call(Editor editor);
	
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
	protected override void Call(Editor editor) 
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
	protected override void Call(Editor editor) 
	{
		EditorLog.Trace("EditorSaveCommand");
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(editor);
		
		if (editor.GetSaveFile() == string.Empty) {
			//EditorFileSaveDialog save_dialog = new EditorFileSaveDialog();
			//save_dialog.ShowDialog();
		}
		
		DialogResult result = EditorMessageBox.Show("Save", "Are you sure?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;			
			
		FileDialogResult file_result = EditorFileManager.Save(save_data, "$profile:/Editor/SaveData.dze");
		if (file_result != FileDialogResult.SUCCESS) {
			editor.GetEditorHud().CreateNotification(typename.EnumToString(FileDialogResult, file_result), COLOR_RED);
			return;
		} 
		
		editor.GetEditorHud().CreateNotification("Saved!", COLOR_GREEN);
		EditorLog.Info("Saved %1 objects!", save_data.EditorObjects.Count().ToString());
	}
		
	override string GetName() {
		return "Save";
	}

	override string GetKeyDisplay() {
		return "Ctrl + S";
	}
}

class EditorSaveAsCommand: EditorCommand
{
	protected override void Call(Editor editor) 
	{
		
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
	
	protected override void Call(Editor editor) 
	{
		DialogResult result = EditorMessageBox.Show("Open", "Are you sure?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		
		EditorSaveData save_data;
		EditorFileManager.Open(save_data, "$profile:/Editor/SaveData.dze");
		
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Info("Loading Map %1", save_data.MapName);
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
			while (!editor) {
				Sleep(1);
			}
		}
		
		editor.DeleteObjects(editor.GetPlacedObjects(), false);
		editor.CreateObjects(save_data.EditorObjects, false);
		
		editor.GetCamera().SetTransform(save_data.CameraPosition);
		string msg = string.Format("Loaded %1 objects!", save_data.EditorObjects.Count().ToString());
		editor.GetEditorHud().CreateNotification(msg, COLOR_GREEN);
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
	protected override void Call(Editor editor) 
	{

	}
			
	override string GetName() {
		return "Close";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + W";
	}
}

class EditorExitCommand: EditorCommand
{	
	protected override void Call(Editor editor) 
	{
		GetGame().LogoutRequestTime();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().CreateLogoutMenu, GetGame().GetUIManager().GetMenu());
	}
	
	override string GetName() {
		return "Exit";
	}
	
	override string GetKeyDisplay() {
		return "Alt + F4";
	}
	
}

class EditorUndoCommand: EditorCommand
{	
	protected override void Call(Editor editor) 
	{
		editor.GetObjectManager().Undo();
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
	protected override void Call(Editor editor) {
		editor.GetObjectManager().Redo();
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
	protected override void Call(Editor editor) 
	{
		EditorObjectSet placed_objects = editor.GetPlacedObjects();
		foreach (EditorObject eo: placed_objects)
			editor.SelectObject(eo);
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
	protected override void Call(Editor editor) {
		editor.DeleteObjects(editor.GetSelectedObjects());		
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
	protected override void Call(Editor editor) 
	{
		DialogResult result = EditorMessageBox.Show("Export", "Export file expansion_export.map?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(editor);
		
		ExportSettings settings = new ExportSettings();
		settings.ExportFileMode = ExportMode.EXPANSION;
		FileDialogResult file_result = EditorFileManager.Export(save_data, "$profile:/Editor/expansion_export.map", settings);
		if (file_result != FileDialogResult.SUCCESS) {
			editor.GetEditorHud().CreateNotification(typename.EnumToString(FileDialogResult, file_result), COLOR_RED);
			return;
		}
		
		string message = string.Format("Exported %1 objects!", save_data.EditorObjects.Count().ToString());
		editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
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
	protected override void Call(Editor editor) 
	{
		DialogResult result = EditorMessageBox.Show("Import", "Import file expansion_import.map?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		

		EditorSaveData save_data();
		EditorFileManager.Import(save_data, "$profile:/Editor/expansion_import.map", ImportMode.EXPANSION);
		string message = string.Format("Imported %1 objects!", save_data.EditorObjects.Count().ToString());
		editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
		
		editor.CreateObjects(save_data.EditorObjects);
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
	protected override void Call(Editor editor) 
	{
		EditorClipboard.Cut(editor.GetSelectedObjects());
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
	protected override void Call(Editor editor) 
	{
		EditorClipboard.Copy(editor.GetSelectedObjects());
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
	protected override void Call(Editor editor) 
	{		
		EditorClipboard.Paste(Editor.CurrentMousePosition);
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
	protected override void Call(Editor editor) 
	{
		
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
	protected override void Call(Editor editor) 
	{
		EditorEnvironmentDialog dialog = new EditorEnvironmentDialog(null, "Weather Controller");
		DialogResult result = dialog.ShowDialog();
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
	protected override void Call(Editor editor) 
	{

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
	protected override void Call(Editor editor) {
		editor.ReloadHud();
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
	protected override void Call(Editor editor) 
	{
		editor.EditLootSpawns(editor.GetSelectedObjects().GetElement(0).GetType());
	}
	
	override string GetName() {
		return "Edit Loot Spawns";
	}
}

class EditorObjectPropertiesCommand: EditorCommand
{
	protected override void Call(Editor editor) 
	{
		EditorObjectPropertiesDialog properties_dialog(null, "Edit Properties", editor.GetSelectedObjects().GetElement(0));
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
	protected override void Call(Editor editor)
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
	protected override void Call(Editor editor)
	{
		editor.GetEditorHud().GetEditorHudController().GetToolbarController().BrushTypeBoxData.Clear();
		editor.GetEditorHud().GetEditorHudController().GetToolbarController().ReloadBrushes(editor.EditorBrushFile);
	}
	
	override string GetName() {
		return "Reload Brushes";
	}
}

class EditorBrushPropertiesCommand: EditorCommand
{
	protected override void Call(Editor editor)
	{

	}
	
	override string GetName() {
		return "Edit Brush Properties";
	}
}

class EditorMagnetCommand: EditorCommand
{
	protected override void Call(Editor editor)
	{
		
	}
}

class EditorGroundCommand: EditorCommand
{
}

class EditorSnapCommand: EditorCommand
{
}