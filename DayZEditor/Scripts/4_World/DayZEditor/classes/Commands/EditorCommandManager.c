class EditorCommandManager
{
	protected ref map<typename, ref EditorCommand> m_Commands = new map<typename, ref EditorCommand>();
	protected ref map<int, EditorCommand> m_CommandShortcutMap = new map<int, EditorCommand>();

	// This is done specifically for the ViewBindings that bind to these on the toolbar
	// if you adapt ViewBinding to call a function, to acquire a delegate. ill give you $30
	EditorCommand NewCommand;
	EditorCommand OpenCommand;
	EditorCommand SaveCommand;
	EditorCommand SaveAsCommand;
	EditorCommand CloseCommand;
	EditorCommand DeleteCommand;
	EditorCommand ExitCommand;
	EditorCommand EscapeCommand;
	EditorCommand UndoCommand;
	EditorCommand RedoCommand;
	EditorCommand SelectAllCommand;
	EditorCommand CutCommand;
	EditorCommand CopyCommand;
	EditorCommand PasteCommand;
	EditorCommand MagnetCommand;
	EditorCommand GroundCommand;
	EditorCommand SnapCommand;
	EditorCommand CollisionCommand;
	EditorCommand BrushToggleCommand;
	EditorCommand BrushDensityCommand;
	EditorCommand BrushRadiusCommand;
	EditorCommand CameraTrackAddNode;
	EditorCommand CameraTrackRun;
	
	void ~EditorCommandManager()
	{		
		delete m_Commands;
		delete m_CommandShortcutMap;
	}
	
	void Init()
	{
		NewCommand = RegisterCommand(EditorNewCommand);
		OpenCommand = RegisterCommand(EditorOpenCommand);
		RegisterCommand(EditorOpenRecentCommand);
		SaveCommand = RegisterCommand(EditorSaveCommand);
		SaveAsCommand = RegisterCommand(EditorSaveAsCommand);
		CloseCommand = RegisterCommand(EditorCloseCommand);
		ExitCommand = RegisterCommand(EditorExitCommand);
		EscapeCommand = RegisterCommand(EditorEscapeCommand);
		
		RegisterCommand(EditorLoadMapCommand);
	
		UndoCommand = RegisterCommand(EditorUndoCommand);
		RedoCommand = RegisterCommand(EditorRedoCommand);
		SelectAllCommand = RegisterCommand(EditorSelectAllCommand);
		DeleteCommand = RegisterCommand(EditorDeleteCommand);
		RegisterCommand(EditorDuplicateCommand);
		RegisterCommand(EditorFindAndReplaceCommand);
		RegisterCommand(EditorCopyPositionCommand);

		CutCommand = RegisterCommand(EditorCutCommand);
		CopyCommand = RegisterCommand(EditorCopyCommand);
		PasteCommand = RegisterCommand(EditorPasteCommand);
	
		MagnetCommand = RegisterCommand(EditorMagnetCommand);
		GroundCommand = RegisterCommand(EditorGroundCommand);
		SnapCommand = RegisterCommand(EditorSnapCommand);
		CollisionCommand = RegisterCommand(EditorCollisionCommand);
	
		RegisterCommand(EditorDumpSceneCommand);
		RegisterCommand(EditorEnvironmentControlCommand);
		RegisterCommand(EditorCameraControlsCommand);
		
		// All preferences
		RegisterCommand(EditorPreferencesCommand);
		
		RegisterCommand(EditorReloadHudCommand);
		RegisterCommand(EditorReloadBrushesCommand);
		RegisterCommand(EditorReloadCacheCommand);
	
		RegisterCommand(EditorLootEditorCommand);
		RegisterCommand(EditorAddToFavoritesCommand);
		RegisterCommand(EditorPlaceObjectCommand);
	
		RegisterCommand(EditorBrushPropertiesCommand);
		BrushToggleCommand = RegisterCommand(EditorBrushToggleCommand);
		BrushDensityCommand = RegisterCommand(EditorBrushDensityCommand);
		BrushRadiusCommand = RegisterCommand(EditorBrushRadiusCommand);
	
		RegisterCommand(EditorObjectPropertiesCommand);
		RegisterCommand(EditorShowCommand);
		RegisterCommand(EditorHideCommand);
		RegisterCommand(EditorLockCommand);
		RegisterCommand(EditorUnlockCommand);
	
		RegisterCommand(EditorExportToInitFile);
		RegisterCommand(EditorExportToExpansion);
		RegisterCommand(EditorExportToTerrainBuilder);
		RegisterCommand(EditorExportToVPP);
		RegisterCommand(EditorExportToCOM);
		RegisterCommand(EditorExportToEvents);
		RegisterCommand(EditorExportToMapGroupPos);
		RegisterCommand(EditorExportLoaderCache);
		
		RegisterCommand(EditorCheckDuplicatesCommand);
	
		RegisterCommand(EditorImportFromInit);
		RegisterCommand(EditorImportFromExpansion);
		RegisterCommand(EditorImportFromTerrainBuilder);
		RegisterCommand(EditorImportFromVPP);
		RegisterCommand(EditorImportFromCOM);
	
		RegisterCommand(EditorScriptEditorCommand);
	
		RegisterCommand(EditorJoinDiscordCommand);
		RegisterCommand(EditorOpenWikiCommand);
	
		RegisterCommand(EditorDonateCommand);
		RegisterCommand(EditorHelpCommand);
		
		// Camera Track
		CameraTrackAddNode = RegisterCommand(EditorCameraTrackAddNode);
		CameraTrackRun = RegisterCommand(EditorCameraTrackRun);
	}
	
	EditorCommand RegisterCommand(typename command_type)
	{
		if (!command_type.IsInherited(EditorCommand)) {
			EditorLog.Error("Command must inherit from EditorCommand");
			return null;
		}
		
		EditorCommand command = EditorCommand.Cast(command_type.Spawn());
		if (!command) {
			EditorLog.Error("Invalid command");
			return null;
		}
		
		this[command_type] = command;
		
		if (command.GetShortcut()) {
			m_CommandShortcutMap.Insert(command.GetShortcut().GetMask(), command);
		}	
		
		return command;	
	}
	
	void Set(typename command_type, EditorCommand command)
	{
		m_Commands.Insert(command_type, command);
	}
	
	EditorCommand Get(typename command_type)
	{
		return m_Commands[command_type];
	}
	
	array<EditorCommand> GetCommands()
	{
		return m_Commands.GetValueArray();
	}
	
	EditorCommand GetCommandFromShortcut(int shortcut)
	{
		return m_CommandShortcutMap[shortcut];
	}
}