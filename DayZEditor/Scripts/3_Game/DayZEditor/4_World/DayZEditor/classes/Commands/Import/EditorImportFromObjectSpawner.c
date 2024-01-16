class EditorImportFromObjectSpawner: EditorImportCommandBase
{
	override typename GetFileType() 
	{
		return EditorObjectSpawnerFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_IMPORT_OBJECTSPAWNER (*.json)";
	}
}