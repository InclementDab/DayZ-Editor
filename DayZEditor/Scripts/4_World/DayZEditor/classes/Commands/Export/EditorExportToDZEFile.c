#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorExportToDZEFile)]
#endif
class EditorExportToDZEFile: EditorExportCommandBase
{	
	override typename GetFileType() 
	{
		return EditorDZEFile;
	}
	
	override string GetName() 
	{
		return "Export to DZE (*.dze)";
	}
}