
class EditorCommand: RelayCommand
{	
	protected Editor m_Editor = GetEditor();
	
	void Call();
	
	override void Execute(RelayCommandArgs args) {
		EditorLog.Trace("EditorCommand::Execute");
		super.Execute(args);
		m_Editor.GetEditorHud().CloseMenu();
		Call();
	}
	
	override void CanExecuteChanged(bool state) {
		
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
		m_Editor.New();
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
		EditorWorldData world_data(m_Editor);
		m_Editor.Save(m_Editor.GetSaveFile(), world_data);
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
		EditorFileSaveDialog save_dialog = new EditorFileSaveDialog(new EditorWorldData(m_Editor));
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
		m_Editor.Open();
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
		m_Editor.Close();
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
		SetCanExecute(m_Editor.GetObjectManager().CanUndo());
	}
	
	override void Call() {
		m_Editor.GetObjectManager().Undo();
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
		SetCanExecute(m_Editor.GetObjectManager().CanRedo());
	}
	
	override void Call() {
		m_Editor.GetObjectManager().Redo();
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
		EditorObjectSet placed_objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject eo: placed_objects)
			m_Editor.SelectObject(eo);
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
		m_Editor.GetObjectManager().DeleteObjects(m_Editor.GetSelectedObjects());
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
		m_Editor.Export();
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
		m_Editor.Import();
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
		SetCanExecute(m_Editor.GetSelectedObjects().Count() > 0);
	}
	
	override void Call() {
		m_Editor.Cut(m_Editor.GetSelectedObjects());
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
		SetCanExecute(m_Editor.GetSelectedObjects().Count() > 0);
	}
	
	override void Call() {
		m_Editor.Copy(m_Editor.GetSelectedObjects());
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
		m_Editor.Paste(m_Editor.CurrentMousePosition);
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
		EditorEnvironmentDialog environment_dialog = new EditorEnvironmentDialog();
		environment_dialog.Show();
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
		EditorCameraDialog cam_dialog(m_Editor.GetCamera());
		cam_dialog.Show();
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
		m_Editor.ReloadHud();
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