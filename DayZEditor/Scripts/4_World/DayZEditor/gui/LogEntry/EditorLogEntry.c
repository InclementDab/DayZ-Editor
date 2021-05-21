class EditorLogEntry: ScriptViewTemplate<EditorLogEntryController>
{
	static const int TRACE 		= ARGB(255, 255, 255, 255);
	static const int DEBUG 		= ARGB(255, 255, 255, 255);
	static const int INFO 		= ARGB(255, 73, 128, 255);
	static const int WARNING 	= ARGB(255, 239, 239, 0);
	static const int ERROR 		= ARGB(255, 239, 0, 0);
	
	MultilineTextWidget LogEntryText;
	
	void EditorLogEntry(LogLevel level, string message)
	{
		m_TemplateController.Message = message;
		m_TemplateController.NotifyPropertyChanged("Message");
		
		switch (level) {
			case LogLevel.TRACE: {
				LogEntryText.SetColor(TRACE);
				break;
			}
			
			case LogLevel.DEBUG: {
				LogEntryText.SetColor(DEBUG);
				break;
			}
			
			case LogLevel.INFO: {
				LogEntryText.SetColor(INFO);
				break;
			}
			
			case LogLevel.WARNING: {
				LogEntryText.SetColor(WARNING);
				break;
			}
			
			case LogLevel.ERROR: {
				LogEntryText.SetColor(ERROR);
				break;
			}
		}
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/items/EditorLogEntry.layout";
	}
}