class EditorLogEntry: ScriptView
{
	static const int TRACE 		= ARGB(255, 255, 255, 255);
	static const int DEBUG 		= ARGB(255, 255, 255, 255);
	static const int INFO 		= ARGB(255, 73, 170, 255);
	static const int WARNING 	= ARGB(255, 239, 239, 0);
	static const int ERROR 		= ARGB(255, 239, 0, 0);
	
	static const int FADE_OUT_TIME = 15000;
	
	MultilineTextWidget LogEntryText;
	
	void EditorLogEntry(LogLevel level, string message)
	{
		string log_level_string = typename.EnumToString(LogLevel, level);
		LogEntryText.SetText(string.Format("[%1]: %2", log_level_string, message));
		
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
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(FadeOut, FADE_OUT_TIME);
	}
	
	protected void FadeOut()
	{
		WidgetAnimator.Animate(GetLayoutRoot(), WidgetAnimatorProperty.COLOR_A, 0.0, 100);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(Delete, 100);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/items/EditorLogEntry.layout";
	}
}