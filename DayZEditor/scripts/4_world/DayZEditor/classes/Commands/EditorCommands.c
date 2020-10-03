
class EditorCommand: RelayCommand
{	
	protected Editor m_Editor;
	
	string Text;

	override bool Execute(Class sender, CommandArgs args) 
	{
		EditorLog.Trace("EditorCommand::Execute");
		super.Execute(sender, args);
		if (EditorUIManager.CurrentMenu)
			delete EditorUIManager.CurrentMenu;		
		
		if (!m_Editor) {
			m_Editor = GetEditor();
		}
		
		// Needs to be since we do ShowDialog alot
		if (!m_Editor) {
			EditorLog.Error("EditorCommand::Editor was null!");
			return true;
		} 
		
		thread Call(sender, args);
		
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
	
	
	protected void Call(Class sender, CommandArgs args);
	
	string GetName() {
		return string.Empty;
	}
	
	string GetIcon() {
		return string.Empty;
	}
	
	string GetShortcut() {
		return string.Empty;
	}
}

class EditorNewCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorEditBoxDialog edit_dialog = new EditorEditBoxDialog("New File...");
		string edit_data;
		DialogResult result = edit_dialog.ShowDialog(edit_data);
		if (result != DialogResult.OK) return;
	}
		
	override string GetName() {
		return "New";
	}
	
	override string GetShortcut() {
		return "Ctrl + N";
	}
	
	override string GetIcon() {
		return "DayZEditor/gui/icons/icon_new2.edds";
	}
}

class EditorSaveCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
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
			m_Editor.GetEditorHud().CreateNotification(typename.EnumToString(FileDialogResult, file_result), COLOR_RED);
			return;
		} 
		
		m_Editor.GetEditorHud().CreateNotification("Saved!", COLOR_GREEN);
		EditorLog.Info("Saved %1 objects!", save_data.EditorObjects.Count().ToString());
	}
		
	override string GetName() {
		return "Save";
	}

	override string GetShortcut() {
		return "Ctrl + S";
	}
	
	override string GetIcon() {
		return "DayZEditor/gui/icons/icon_save.edds";
	}
	
}

class EditorSaveAsCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		
	}
		
	override string GetName() {
		return "Save As...";
	}
	
	override string GetShortcut() {
		return "Ctrl + Shift + S";
	}
}

class EditorOpenCommand: EditorCommand
{
	void EditorOpenCommand() 
	{
		SetCanExecute(false);
	}
	
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show("Open", "Are you sure?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		
		EditorSaveData save_data;
		EditorFileManager.Open(save_data, "$profile:/Editor/SaveData.dze");
		
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Info("Loading Map %1", save_data.MapName);
			//delete m_EditorInstance;
			
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
		
			// Scuffed way of waiting for new Editor to be created. It doesnt work (obv)
			/*while (!m_EditorInstance) {
				Sleep(1);
			}*/
		}
		
		m_Editor.DeleteObjects(m_Editor.GetPlacedObjects(), false);
		m_Editor.CreateObjects(save_data.EditorObjects, false);
		
		m_Editor.GetCamera().SetTransform(save_data.CameraPosition);
		string msg = string.Format("Loaded %1 objects!", save_data.EditorObjects.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(msg, COLOR_GREEN);
		EditorLog.Info(msg);
	}

	override string GetName() {
		return "Open";
	}

	override string GetIcon() {
		return "DayZEditor/gui/icons/icon_folder.edds";
	}
	
	override string GetShortcut() {
		return "Ctrl + O";
	}
}

class EditorCloseCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{

	}
			
	override string GetName() {
		return "Close";
	}
	
	override string GetShortcut() {
		return "Ctrl + W";
	}
}

class EditorExitCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		GetGame().LogoutRequestTime();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission().CreateLogoutMenu, GetGame().GetUIManager().GetMenu());
	}
	
	override string GetName() {
		return "Exit";
	}
	
	override string GetShortcut() {
		return "Alt + F4";
	}
	
}

class EditorUndoCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.GetObjectManager().Undo();
	}
	
	override string GetName() {
		return "Undo";
	}
	
	override string GetShortcut() {
		return "Ctrl + Z";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:arrow_round_icon";
	}
}

class EditorRedoCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) {
		m_Editor.GetObjectManager().Redo();
	}
		
	override string GetName() {
		return "Redo";
	}
	
	override string GetShortcut() {
		return "Ctrl + Y";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:arrow_round_icon";
	}
}

class EditorSelectAllCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorObjectSet placed_objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject eo: placed_objects)
			m_Editor.SelectObject(eo);
	}
		
	override string GetName() {
		return "Select All...";
	}
	
	override string GetShortcut() {
		return "Ctrl + A";
	}
}

class EditorDeleteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.DeleteObjects(m_Editor.GetSelectedObjects());		
	}
		
	override string GetName() {
		return "Delete";
	}
	
	override string GetShortcut() {
		return "Delete";
	}
}

class EditorExportCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show("Export", "Export file expansion_export.map?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		
		EditorSaveData save_data = EditorSaveData.CreateFromEditor(m_Editor);
		
		ExportSettings settings = new ExportSettings();
		settings.ExportFileMode = ExportMode.EXPANSION;
		FileDialogResult file_result = EditorFileManager.Export(save_data, "$profile:/Editor/expansion_export.map", settings);
		if (file_result != FileDialogResult.SUCCESS) {
			m_Editor.GetEditorHud().CreateNotification(typename.EnumToString(FileDialogResult, file_result), COLOR_RED);
			return;
		}
		
		string message = string.Format("Exported %1 objects!", save_data.EditorObjects.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
	}

	override string GetName() {
		return "Export";
	}
	
	override string GetShortcut() {
		return "Ctrl + E";
	}
}

class EditorImportCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show("Import", "Import file expansion_import.map?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		

		EditorSaveData save_data();
		EditorFileManager.Import(save_data, "$profile:/Editor/expansion_import.map", ImportMode.EXPANSION);
		string message = string.Format("Imported %1 objects!", save_data.EditorObjects.Count().ToString());
		m_Editor.GetEditorHud().CreateNotification(message, COLOR_GREEN);
		EditorLog.Info(message);
		
		m_Editor.CreateObjects(save_data.EditorObjects);
	}

	override string GetName() {
		return "Import";
	}
	
	override string GetShortcut() {
		return "Ctrl + I";
	}
}

class EditorCutCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorClipboard.Cut(m_Editor.GetSelectedObjects());
	}

	override string GetName() {
		return "Cut";
	} 
	
	override string GetShortcut() {
		return "Ctrl + X";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:cut_icon";
	}
}
	
class EditorCopyCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorClipboard.Copy(m_Editor.GetSelectedObjects());
	}

	override string GetName() {
		return "Copy";
	}
	
	override string GetShortcut() {
		return "Ctrl + C";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:copy_icon";
	}
}

class EditorPasteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{		
		EditorClipboard.Paste(m_Editor.CurrentMousePosition);
	}

	override string GetName() {
		return "Paste";
	}
	
	override string GetShortcut() {
		return "Ctrl + V";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:paste_icon";
	}
}

class EditorPreferencesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
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
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorEnvironmentDialog dialog = new EditorEnvironmentDialog("Weather Controller");
		DialogResult result = dialog.ShowDialog();
	}
	
	override string GetName() {
		return "Environment";
	}
	
	override string GetShortcut() {
		return "Ctrl + Shift + I";
	}
	
	override string GetIcon() {
		return "set:dayz_gui image:icon_sun";
	}
}

class EditorCameraControlsCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{

	}

	override string GetName() {
		return "Camera";
	}
	
	override string GetShortcut() {
		return "Ctrl + Shift + T";
	}
}

class EditorReloadHudCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.ReloadHud();
	}

	override string GetName() {
		return "Reload UI";
	}
	
	override string GetIcon() {
		return "set:dayz_gui image:icon_refresh";
	}
	
	override string GetShortcut() {
		return "Ctrl + Shift + U";
	}
}

class EditorLootEditorCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.EditLootSpawns(m_Editor.GetSelectedObjects().GetElement(0).GetType());
	}
	
	override string GetName() {
		return "Edit Loot Spawns";
	}
}

class EditorObjectPropertiesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorObjectPropertiesDialog properties_dialog("Edit Properties", m_Editor.GetSelectedObjects().GetElement(0));
		properties_dialog.ShowDialog();
	}
	
	override string GetName() {
		return "Properties";
	}
	
	override string GetShortcut() {
		return "Ctrl + T";
	}
}

class EditorLoadMapCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorMapSelectDialog select_dialog = new EditorMapSelectDialog(GetName());
		string selected_map;
		DialogResult result = select_dialog.ShowDialog(selected_map);
		
		if (selected_map != string.Empty && result == DialogResult.OK) {
			EditorLog.Info("Loading Map %1", selected_map);
			m_Editor.GetObjectManager().Clear();
			GetGame().PlayMission(CreateEditorMission(selected_map));
		}
	}
	
	override string GetName() {
		return "Select Map...";
	}
}

class EditorReloadBrushesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().BrushTypeBoxData.Clear();
		m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().ReloadBrushes(m_Editor.EditorBrushFile);
	}
	
	override string GetName() {
		return "Reload Brushes";
	}
}

class EditorBrushPropertiesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{

	}
	
	override string GetName() {
		return "Edit Brush Properties";
	}
}

class EditorMagnetCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		// These 3 commands are really ugly plz fix tyler
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (m_ViewBinding && button_args) {
			if (button_args.GetButtonState()) {
				m_ViewBinding.GetLayoutRoot().GetChildren().SetColor(COLOR_CANDY);
			} else {
				m_ViewBinding.GetLayoutRoot().GetChildren().SetColor(COLOR_WHITE);
			}
		}
	}
	
	override string GetName() {
		return "Toggle Magnet Mode";
	}
}

class EditorGroundCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (m_ViewBinding && button_args) {
			if (button_args.GetButtonState()) {
				m_ViewBinding.GetLayoutRoot().GetChildren().SetColor(COLOR_APPLE);
			} else {
				m_ViewBinding.GetLayoutRoot().GetChildren().SetColor(COLOR_WHITE);
			}
		}
	}
	
	override string GetName() {
		return "Toggle Ground Mode";
	}
}

class EditorSnapCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (m_ViewBinding && button_args) {
			if (button_args.GetButtonState()) {
				m_ViewBinding.GetLayoutRoot().GetChildren().SetColor(COLOR_JELLY);
			} else {
				m_ViewBinding.GetLayoutRoot().GetChildren().SetColor(COLOR_WHITE);
			}
		}
	}
	
	override string GetName() {
		return "Toggle Snapping";
	}
}

class EditorBrushToggleCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		
		switch (button_args.GetMouseButton()) {
			
			case 0: {
				EditorHudToolbarController toolbar_controller = m_Editor.GetEditorHud().GetTemplateController().GetToolbarController();
				bool button_state = button_args.GetButtonState();
				button_args.Source.FindAnyWidget("BrushToggleButtonText").SetPos(button_state * 1, button_state * 1);
				
				
				if (button_state) {
					m_Editor.SetBrush(EditorBrush.Create(toolbar_controller.BrushTypeBoxData[toolbar_controller.BrushTypeSelection]));
				} else {
					m_Editor.SetBrush(null);
				}
				
				m_Editor.CommandManager.BrushRadiusCommand.SetCanExecute(button_state);
				m_Editor.CommandManager.BrushDensityCommand.SetCanExecute(button_state);
				
				break;
			}
			
			case 1: {
				m_Editor.CommandManager.BrushPropertiesCommand.Execute(this, args);
				break;
			}
		}
	}
	
	override string GetName() {
		return "Toggle Brush";
	}
}

class EditorPlaceObjectCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		if (!m_Editor.ObjectInHand) return;

		EditorObject editor_object = m_Editor.CreateFromObject(m_Editor.ObjectInHand.GetProjectionEntity());
		EditorEvents.ObjectPlaced(this, editor_object);
		m_Editor.SelectObject(editor_object);
		
		string type = m_Editor.ObjectInHand.GetProjectionEntity().GetType();
		delete m_Editor.ObjectInHand;
		
		if (!KeyState(KeyCode.KC_LSHIFT)) { 
			EditorEvents.StopPlacing(this);
		} else {
			m_Editor.ObjectInHand = new EditorHologram(type, m_Editor.CurrentMousePosition);
		}
	}
}

class EditorBrushDensityCommand: EditorCommand
{
	override void CanExecuteChanged(bool state)
	{
		if (m_ViewBinding) {
			Widget root = m_ViewBinding.GetLayoutRoot().GetParent();
			root.Show(state);
			root.Enable(state);			
		}
	}
	
	override string GetName() {
		return "Brush Density";
	}
}

class EditorBrushRadiusCommand: EditorCommand
{
	override void CanExecuteChanged(bool state)
	{
		Print(m_Controller);
		if (m_ViewBinding) {
			Widget root = m_ViewBinding.GetLayoutRoot().GetParent();
			Print(root.GetName());
			root.Show(state);
			root.Enable(state);	
		}
	}
	
	override string GetName() {
		return "Brush Radius";
	}
}