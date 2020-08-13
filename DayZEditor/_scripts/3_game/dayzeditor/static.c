





class EditorLog
{
	
	static EditorLogLevel CurrentLogLevel = EditorLogLevel.TRACE;
	
	private static void EditorPrint(string msg, EditorLogLevel level)
	{
		if (level >= CurrentLogLevel)
			Print(string.Format("[EDITOR::%1] %2", typename.EnumToString(EditorLogLevel, level), msg));
	}
	
	static void Trace(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.TRACE);
#endif
	}
	
	static void Debug(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.DEBUG);
#endif
	}
	
	static void Info(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.INFO);
#endif
	}
	
	static void Warning(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.WARNING);
#endif
	}
	
	static void Error(string msg)
	{
#ifdef EDITORPRINT
		EditorPrint(msg, EditorLogLevel.ERROR);
#endif
	}
		
	
}
