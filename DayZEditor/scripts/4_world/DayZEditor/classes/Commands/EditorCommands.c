
class EditorCommand: RelayCommand
{	
	protected Editor m_Editor = GetEditor();
	protected EditorHudController m_EditorHudController;
	
	string Text;
	
	void EditorCommand()
	{
		if (m_Editor) {
			m_EditorHudController = m_Editor.GetEditorHud().GetEditorHudController();
		}
	}

	override bool Execute(Class sender, CommandArgs args) 
	{
		EditorLog.Trace("EditorCommand::Execute");
		super.Execute(sender, args);
		if (EditorUIManager.CurrentMenu)
			delete EditorUIManager.CurrentMenu;
		
		Call();
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
	
	
	void Call();
	
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
	override void Call() {
		GetEditor().New();
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
	override void Call() {
		GetEditor().Save();
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
	override void Call() {
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
	void EditorOpenCommand(ref array<KeyCode> input_gestures = null) {
		SetCanExecute(false);
	}
	
	override void Call() {
		GetEditor().Open();
	}
			
	override string GetName() {
		return "Open";
	}
		
	override string GetIcon() {
		return "DayZEditor/gui/icons/icon_folder_128x128.edds";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + O";
	}
}

class EditorCloseCommand: EditorCommand
{
	override void Call() {
		GetEditor().Close();
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
	override void Call() {
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
	
	override void Call() {
		GetEditor().GetObjectManager().Undo();
	}
	
	override string GetName() {
		return "Undo";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + Z";
	}
}

class EditorRedoCommand: EditorCommand
{
	void EditorRedoCommand(ref array<KeyCode> input_gestures = null) {
		SetCanExecute(GetEditor().GetObjectManager().CanRedo());
	}
	
	override void Call() {
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
	override void Call() {
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
	override void Call() {
		if (GetEditor())
			GetEditor().GetObjectManager().DeleteObjects(GetEditor().GetSelectedObjects());
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
	override void Call() {
		GetEditor().Export();
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
	override void Call() {
		GetEditor().Import();
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
	
	override void Call() {
		GetEditor().Cut(GetEditor().GetSelectedObjects());
	}

	override string GetName() {
		return "Cut";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + X";
	}
}

class EditorCopyCommand: EditorCommand
{
	void EditorCopyCommand(ref array<KeyCode> input_gestures = null) {
		SetCanExecute(GetEditor().GetSelectedObjects().Count() > 0);
	}
	
	override void Call() {
		GetEditor().Copy(GetEditor().GetSelectedObjects());
	}

	override string GetName() {
		return "Copy";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + C";
	}
}

class EditorPasteCommand: EditorCommand
{
	void EditorPasteCommand(ref array<KeyCode> input_gestures = null) {
		string clipboard_text;
		GetGame().CopyFromClipboard(clipboard_text);
		SetCanExecute(clipboard_text != string.Empty);
	}
	
	override void Call() {
		GetEditor().Paste(GetEditor().CurrentMousePosition);
	}

	override string GetName() {
		return "Paste";
	}
	
	override string GetKeyDisplay() {
		return "Ctrl + V";
	}
}

class EditorPreferencesCommand: EditorCommand
{
	override void Call() {
		
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
	override void Call() {
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
	override void Call() {
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
	override void Call() {
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
	override void Call() {
		GetEditor().EditLootSpawns(GetEditor().GetSelectedObjects().GetElement(0).GetType());
	}
	
	override string GetName() {
		return "Edit Loot Spawns";
	}
}

class EditorObjectPropertiesCommand: EditorCommand
{
	override void Call() {
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