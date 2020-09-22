
class EditorCommand: RelayCommand
{	
	protected Editor m_Editor;
	protected EditorHudController m_EditorHudController;
	
	void EditorCommand()
	{
		m_Editor = GetEditor();
		if (m_Editor) {
			m_EditorHudController = m_Editor.GetEditorHud().GetEditorHudController();
		}
	}
	
	override void Execute(RelayCommandArgs args) 
	{
		EditorLog.Trace("EditorCommand::Execute");
		super.Execute(args);
		if (EditorUIManager.CurrentMenu)
			delete EditorUIManager.CurrentMenu;
		
		Call();
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
	
	string GetKeys() {
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
	
	override string GetKeys() {
		return "Ctrl + N";
	}
}

class EditorSaveCommand: EditorCommand
{
	override void Call() {
		EditorWorldData world_data(GetEditor());
		GetEditor().Save(GetEditor().GetSaveFile(), world_data);
	}
		
	override string GetName() {
		return "Save";
	}

	override string GetKeys() {
		return "Ctrl + S";
	}
}

class EditorSaveAsCommand: EditorCommand
{
	override void Call() {
		EditorFileSaveDialog save_dialog = new EditorFileSaveDialog(null, GetEditor().GetEditorHud().GetEditorHudController());
		save_dialog.SetWorldData(new EditorWorldData(GetEditor()));
		string file = save_dialog.ShowFileDialog();
	}
		
	override string GetName() {
		return "Save As...";
	}
	
	override string GetKeys() {
		return "Ctrl + Shift + S";
	}
}

class EditorOpenCommand: EditorCommand
{
	void EditorOpenCommand() {
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
	
	override string GetKeys() {
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
	
	override string GetKeys() {
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
	
	override string GetKeys() {
		return "Alt + F4";
	}
	
}

class EditorUndoCommand: EditorCommand
{
	void EditorUndoCommand() {
		SetCanExecute(GetEditor().GetObjectManager().CanUndo());
	}
	
	override void Call() {
		GetEditor().GetObjectManager().Undo();
	}
	
	override string GetName() {
		return "Undo";
	}
	
	override string GetKeys() {
		return "Ctrl + Z";
	}
}

class EditorRedoCommand: EditorCommand
{
	void EditorRedoCommand() {
		SetCanExecute(GetEditor().GetObjectManager().CanRedo());
	}
	
	override void Call() {
		GetEditor().GetObjectManager().Redo();
	}
		
	override string GetName() {
		return "Redo";
	}
	
	override string GetKeys() {
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
	
	override string GetKeys() {
		return "Ctrl + A";
	}
}

class EditorDeleteCommand: EditorCommand
{
	override void Call() {
		GetEditor().GetObjectManager().DeleteObjects(GetEditor().GetSelectedObjects());
	}
		
	override string GetName() {
		return "Delete";
	}
	
	override string GetKeys() {
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
	
	override string GetKeys() {
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
	
	override string GetKeys() {
		return "Ctrl + I";
	}
}

class EditorCutCommand: EditorCommand
{
	void EditorCutCommand() {
		SetCanExecute(GetEditor().GetSelectedObjects().Count() > 0);
	}
	
	override void Call() {
		GetEditor().Cut(GetEditor().GetSelectedObjects());
	}

	override string GetName() {
		return "Cut";
	}
	
	override string GetKeys() {
		return "Ctrl + X";
	}
}

class EditorCopyCommand: EditorCommand
{
	void EditorCopyCommand() {
		SetCanExecute(GetEditor().GetSelectedObjects().Count() > 0);
	}
	
	override void Call() {
		GetEditor().Copy(GetEditor().GetSelectedObjects());
	}

	override string GetName() {
		return "Copy";
	}
	
	override string GetKeys() {
		return "Ctrl + C";
	}
}

class EditorPasteCommand: EditorCommand
{
	void EditorPasteCommand() {
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
	
	override string GetKeys() {
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
		EditorEnvironmentDialog environment_dialog = new EditorEnvironmentDialog(null, m_EditorHudController);
		environment_dialog.ShowDialog();
	}
	
	override string GetName() {
		return "Environment";
	}
	
	override string GetKeys() {
		return "Ctrl + Shift + I";
	}
	
	override string GetIcon() {
		return "set:dayz_gui image:icon_sun";
	}
}

class EditorCameraControlsCommand: EditorCommand
{
	override void Call() {
		EditorCameraDialog cam_dialog(null, m_EditorHudController);
		cam_dialog.SetEditorCamera(m_Editor.GetCamera());
		cam_dialog.ShowDialog();
	}

	override string GetName() {
		return "Camera";
	}
	
	override string GetKeys() {
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
	
	override string GetKeys() {
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
		EditorObjectPropertiesDialog properties_dialog(null, m_EditorHudController);
		properties_dialog.SetEditorObject(m_Editor.GetSelectedObjects().GetElement(0));
		properties_dialog.ShowDialog();
	}
	
	override string GetName() {
		return "Properties";
	}
	
	override string GetKeys() {
		return "Ctrl + T";
	}
}