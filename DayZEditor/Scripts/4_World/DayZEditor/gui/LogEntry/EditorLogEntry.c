class EditorLogEntry: ScriptViewTemplate<EditorLogEntryController>
{
	static const int TRACE 		= ARGB(255, 255, 255, 255);
	static const int DEBUG 		= ARGB(255, 255, 255, 255);
	static const int INFO 		= ARGB(255, 73, 128, 255);
	static const int WARNING 	= ARGB(255, 239, 239, 0);
	static const int ERROR 		= ARGB(255, 239, 0, 0);
		
	void EditorLogEntry(LogLevel level, string message)
	{
		m_TemplateController.Message = message;
		m_TemplateController.NotifyPropertyChanged("Message");
		
		switch (level) {
			case LogLevel.TRACE: {
				GetLayoutRoot().SetColor(TRACE);
				break;
			}
			
			case LogLevel.DEBUG: {
				GetLayoutRoot().SetColor(DEBUG);
				break;
			}
			
			case LogLevel.INFO: {
				GetLayoutRoot().SetColor(INFO);
				break;
			}
			
			case LogLevel.WARNING: {
				GetLayoutRoot().SetColor(WARNING);
				break;
			}
			
			case LogLevel.ERROR: {
				GetLayoutRoot().SetColor(ERROR);
				break;
			}
		}
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/items/EditorLogEntry.layout";
	}
}