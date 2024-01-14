// more or less a dummy to enable / disable the Open Recent button
class EditorOpenRecentCommand: EditorCommand
{
	override bool CanExecute()
	{
		return GetEditor().GetRecentFiles().Count() > 0;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_OPEN_RECENT";
	}
}