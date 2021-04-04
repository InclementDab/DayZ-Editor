class EditorCommandManager
{
	protected ref map<typename, ref EditorCommand> m_Commands = new map<typename, ref EditorCommand>();
	protected ref map<int, EditorCommand> m_CommandShortcutMap = new map<int, EditorCommand>();

	void ~EditorCommandManager()
	{		
		delete m_Commands;
		delete m_CommandShortcutMap;
	}
	
	void Init()
	{
		RegisterCommand(EditorNewCommand);
		RegisterCommand(EditorOpenCommand);
		RegisterCommand(EditorSaveCommand);
		RegisterCommand(EditorSaveAsCommand);
		RegisterCommand(EditorCloseCommand);
		RegisterCommand(EditorExitCommand);
		RegisterCommand(EditorEscapeCommand);
	
		RegisterCommand(EditorUndoCommand);
		RegisterCommand(EditorRedoCommand);
		RegisterCommand(EditorSelectAllCommand);
		RegisterCommand(EditorDeleteCommand);
	
		RegisterCommand(EditorCutCommand);
		RegisterCommand(EditorCopyCommand);
		RegisterCommand(EditorPasteCommand);
	
		RegisterCommand(EditorMagnetCommand);
		RegisterCommand(EditorGroundCommand);
		RegisterCommand(EditorSnapCommand);
		RegisterCommand(EditorCollisionCommand);
	
		RegisterCommand(EditorDumpSceneCommand);
		RegisterCommand(EditorEnvironmentControlCommand);
		RegisterCommand(EditorPreferencesCommand);
		RegisterCommand(EditorCameraControlsCommand);
		RegisterCommand(EditorReloadHudCommand);
		RegisterCommand(EditorReloadBrushesCommand);
		RegisterCommand(EditorLootEditorCommand);
		RegisterCommand(EditorAddToFavoritesCommand);
	
		RegisterCommand(EditorPlaceObjectCommand);
	
		RegisterCommand(EditorBrushPropertiesCommand);
		RegisterCommand(EditorBrushToggleCommand);
		RegisterCommand(EditorBrushDensityCommand);
		RegisterCommand(EditorBrushRadiusCommand);
	
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
	}
	
	void RegisterCommand(typename command_type)
	{
		if (!command_type.IsInherited(EditorCommand)) {
			EditorLog.Error("Command must inherit from EditorCommand");
			return;
		}
		
		EditorCommand command = EditorCommand.Cast(command_type.Spawn());
		if (!command) {
			EditorLog.Error("Invalid command");
			return;
		}
		
		this[command_type] = command;
		
		if (command.GetShortcut()) {
			m_CommandShortcutMap.Insert(command.GetShortcut().GetMask(), command);
		}		
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