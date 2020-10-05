class EditorCommandManager
{
	ref map<int, EditorCommand> CommandShortcutMap = new map<int, EditorCommand>();
	
	// Editor Commands
	ref EditorNewCommand NewCommand;
	ref EditorOpenCommand OpenCommand;
	ref EditorSaveCommand SaveCommand;
	ref EditorSaveAsCommand SaveAsCommand;
	ref EditorCloseCommand CloseCommand;
	ref EditorExportCommand ExportCommand;
	ref EditorImportCommand ImportCommand;
	ref EditorExitCommand ExitCommand;
	
	ref EditorUndoCommand UndoCommand;
	ref EditorRedoCommand RedoCommand;
	ref EditorSelectAllCommand SelectAllCommand;
	ref EditorDeleteCommand DeleteCommand;
	
	ref EditorCutCommand CutCommand;
	ref EditorCopyCommand CopyCommand;
	ref EditorPasteCommand PasteCommand;
	
	ref EditorMagnetCommand MagnetCommand;
	ref EditorGroundCommand GroundCommand;
	ref EditorSnapCommand SnapCommand;
	ref EditorCollisionCommand CollisionCommand;
	
	ref EditorEnvironmentControlCommand EnvironmentControlCommand;
	ref EditorPreferencesCommand PreferencesCommand;
	ref EditorCameraControlsCommand CameraControlsCommand;
	ref EditorReloadHudCommand ReloadHudCommand;
	ref EditorReloadBrushesCommand ReloadBrushesCommand;
	ref EditorLootEditorCommand LootEditorCommand;
	
	ref EditorPlaceObjectCommand PlaceObjectCommand;
	
	ref EditorBrushPropertiesCommand BrushPropertiesCommand;
	ref EditorBrushToggleCommand BrushToggleCommand;	
	ref EditorBrushDensityCommand BrushDensityCommand;
	ref EditorBrushRadiusCommand BrushRadiusCommand;
	
	ref EditorObjectPropertiesCommand ObjectPropertiesCommand;
	
	void EditorCommandManager()
	{			
		for (int i = 0; i < Type().GetVariableCount(); i++) {
			string variable_name = Type().GetVariableName(i);
			typename variable_type = Type().GetVariableType(i);
			if (variable_type.IsInherited(EditorCommand)) {
				EditorCommand command = variable_type.Spawn();
				EnScript.SetClassVar(this, variable_name, 0, command);
				if (command.GetKeys()) {
					CommandShortcutMap.Insert(command.GetKeys().GetMask(), command);
				}
			}
		}
	}
	
	EditorCommand GetCommandFromHotkeys(int key)
	{
		//EditorLog.Trace("Editor::GetCommandFromHotkeys");
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			
			// Ctrl + Shift keybinds
			if (KeyState(KeyCode.KC_LSHIFT)) {
				
				switch (key) {
					
					case KeyCode.KC_S: {
						return SaveAsCommand;
					}
					
					case KeyCode.KC_I: {
						return EnvironmentControlCommand;
					}
					
					case KeyCode.KC_T: {
						return CameraControlsCommand;
					}
					
					case KeyCode.KC_U: {
						return ReloadHudCommand;						
					}
				}
			}
			
			
			switch (key) {
				
				case KeyCode.KC_Z: {
					return UndoCommand;
				}
				
				case KeyCode.KC_Y: {
					return RedoCommand;
				}
				
				case KeyCode.KC_A: {
					return SelectAllCommand;
				}
				
				case KeyCode.KC_N: {
					return NewCommand;
				}
				
				case KeyCode.KC_S: {
					//if (m_EditorSaveFile == string.Empty)
					//	return SaveAsCommand;
					
					return SaveCommand;
				}
				
				case KeyCode.KC_O: {
					return OpenCommand;
				}
				
				case KeyCode.KC_E: {
					return ExportCommand;
				}
				
				case KeyCode.KC_I: {					
					return ImportCommand;
				}
				
				case KeyCode.KC_X: {
					return CutCommand;
				}

				case KeyCode.KC_C: {
					return CopyCommand;
				}
				
				case KeyCode.KC_V: {
					return PasteCommand;
				}
				
				case KeyCode.KC_W: {
					return CloseCommand;
				}
			}
		}

		return null;
	}
}