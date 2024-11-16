// more or less a dummy to enable / disable the Open Recent button
class EditorOpenRecentCommand: EditorCommand
{
	override string GetName()
	{
		return "#STR_EDITOR_OPEN_RECENT";
	}
	
	override bool CanExecute()
	{
		return GetEditor().GetRecentFiles().Count() > 0;
	}
}