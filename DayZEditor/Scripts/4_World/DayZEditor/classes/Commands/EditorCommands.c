
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
	
	// Abstracts
	string GetName() {
		return string.Empty;
	}
	
	string GetIcon() {
		return string.Empty;
	}
		
	string GetShortcutString() 
	{
		if (GetShortcut()) {
			return GetShortcut().GetString();
		}
		
		return string.Empty;
	}
	
	ShortcutKeys GetShortcut();
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

	override string GetIcon() {
		return "set:dayz_editor_gui image:new_alt";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_N };
	}
}




class EditorCloseCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = MessageBox.Show("Close", "Are you sure?", MessageBoxButtons.OKCancel);
		
		if (result != DialogResult.OK) {
			return;
		}
		
		m_Editor.Clear();
	}
			
	override string GetName() {
		return "Close";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_W };
	}
}

class EditorExitCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show("Exit to main menu", "Do you want to close the DayZ Editor?", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) return;
		
		GetGame().AbortMission();
	}
	
	override string GetName() {
		return "Exit";
	}
}

class EditorUndoCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.Undo();
	}
	
	override string GetName() {
		return "Undo";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:undo";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_Z };
	}
}

class EditorRedoCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		m_Editor.Redo();
	}
		
	override string GetName() {
		return "Redo";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:redo";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_Y };
	}
}

class EditorSelectAllCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorObjectMap placed_objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject eo: placed_objects)
			m_Editor.SelectObject(eo);
	}
		
	override string GetName() {
		return "Select All...";
	}
		
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_A };
	}
}

class EditorDeleteCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorObjectMap objects = m_Editor.GetSelectedObjects();
		m_Editor.DeleteObjects(objects);		
	}
		
	override string GetName() {
		return "Delete";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_DELETE };
	}
}

class EditorCutCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorClipboard.Cut();
	}

	override string GetName() {
		return "Cut";
	} 
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:cut";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_X };
	}
}
	
class EditorCopyCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorClipboard.Copy();
	}

	override string GetName() {
		return "Copy";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:copy";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_C };
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
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:paste";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_V };
	}
}

class EditorPreferencesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorPreferencesDialog dialog = new EditorPreferencesDialog("Editor Preferences");
		DialogResult result = dialog.ShowDialog();
		
		if (result != DialogResult.OK) {
			return;
		}
		
		// cf test
		//EditorSettings.Save(m_Editor.Settings, m_Editor.EditorSettingsFile);
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
	
	override string GetIcon() {
		return "set:dayz_gui image:icon_sun";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_I };
	}
}

class EditorCameraControlsCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorCameraDialog camera_dialog("Camera Properties");
		camera_dialog.ShowDialog();
	}

	override string GetName() {
		return "Camera";
	}
	
	override string GetIcon() {
		return "set:dayz_editor_gui image:camera_alt";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_T };
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
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_U };
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
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_T };
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
		m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().ReloadBrushes(m_Editor.Settings.EditorBrushFile);
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
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "MagnetButton") {
			m_Editor.MagnetMode = !m_Editor.MagnetMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.MagnetMode");
		}
	}
	
	override string GetName() {
		return "Toggle Magnet Mode";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_1 };
	}
}

class EditorGroundCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "GroundButton") {
			m_Editor.GroundMode = !m_Editor.GroundMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.GroundMode");
		}
	}
	
	override string GetName() {
		return "Toggle Ground Mode";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_2 };
	}
}

class EditorSnapCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "SnapButton") {
			m_Editor.SnappingMode = !m_Editor.SnappingMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.SnappingMode");
		}
	}
	
	override string GetName() {
		return "Toggle Snapping";
	}

	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_3 };
	}
}

class EditorCollisionCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "CollisionButton") {
			m_Editor.CollisionMode = !m_Editor.CollisionMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.CollisionMode");
		}
	}
	
	override string GetName() {
		return "Toggle Collision";
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_4 };
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
		m_Editor.PlaceObject();
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

class EditorEscapeCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		if (EditorUIManager.CurrentDialog) {	
			EditorUIManager.CurrentDialog.CloseDialog();
			return;
		}
		
		if (EditorUIManager.CurrentMenu) {
			delete EditorUIManager.CurrentMenu;
			return;
		} 
		
		if (m_Editor.IsPlacing()) {
			delete m_Editor.ObjectInHand;
			return;
		}
		
		if (m_Editor.GetSelectedObjects().Count() > 0) {
			m_Editor.ClearSelection();
			return;
		}
		
		if (m_Editor.GetEditorHud().IsMapVisible()) {
			m_Editor.GetEditorHud().EditorMapWidget.Show(false);
			return;
		}
		
		if (m_Editor.IsLootEditActive()) {
			m_Editor.FinishEditLootSpawns();
			return;
		} 

		if (g_Game.GetMission().IsPaused()) {
			g_Game.GetMission().Continue();
			return;
		} 
		
		g_Game.GetMission().Pause();
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_ESCAPE };
	}
	
	override string GetName() {
		return "Escape";
	}
}

class EditorLockCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Lock(true);
		}
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_L };
	}
	
	override string GetName() {
		return "Lock";
	}
}

class EditorUnlockCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Lock(false);
		}
	}
		
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_L };
	}
	
	override string GetName() {
		return "Unlock";
	}
}


class EditorShowCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			selected_object.Show(true);
		}
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_LCONTROL, KeyCode.KC_H };
	}
	
	override string GetName() {
		return "Show";
	}
}

class EditorHideCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			selected_object.Show(false);
		}
	}
	
	override ShortcutKeys GetShortcut() {
		return { KeyCode.KC_H };
	}
	
	override string GetName() {
		return "Hide";
	}
}

class EditorScriptEditorCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{
		EditorMultilineEditBoxDialog edit_dialog = new EditorMultilineEditBoxDialog("Script Console");
		
		string script_test;
		edit_dialog.ShowDialog(script_test);
		
		string file_name = "$profile:Editor/test.c";
		FileHandle handle = OpenFile(file_name, FileMode.WRITE);
		FPrintln(handle, "static void main() {");
		FPrintln(handle, script_test);
		FPrintln(handle, "}");
		
		if (handle) {
			CloseFile(handle);
		}
		
		ScriptModule script_module = ScriptModule.LoadScript(GetGame().GetMission().MissionScript, file_name, true);
		if (!script_module) {
			MessageBox.Show("Error", "Invalid Syntax in Script Editor", MessageBoxButtons.OK);
			return;
		}
		
		script_module.Call(null, "main", null);
	}
	
	override string GetName() {
		return "Script Editor";
	}
}

class EditorDonateCommand: EditorCommand
{
	override void Call(Class sender, CommandArgs args)
	{	
		EditorDonateDialog donate_dialog("Donate");
		donate_dialog.ShowDialog();
	}
	
	override string GetName() {
		return "Donate";
	}
}
