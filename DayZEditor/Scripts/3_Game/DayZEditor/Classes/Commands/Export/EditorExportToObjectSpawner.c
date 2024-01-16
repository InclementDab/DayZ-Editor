class EditorExportToObjectSpawner: EditorExportCommandBase
{
	override typename GetFileType() 
	{
		return EditorObjectSpawnerFile;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_EXPORT_OBJECTSPAWNER (*.json)";
	}
}