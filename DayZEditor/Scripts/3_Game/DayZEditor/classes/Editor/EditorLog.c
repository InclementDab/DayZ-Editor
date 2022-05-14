[RegisterLogger(EditorLog)]
class EditorLog: LoggerBase
{
	static ref ScriptInvoker OnLog;
	
	
	static string ExclusiveLogMode;
	
	private static string m_LastCalledType;	
	protected static LogLevel m_CurrentLogLevel = LogLevel.INFO;
	
	static void EditorPrint(string msg, LogLevel level)
	{	
		if (level < m_CurrentLogLevel) {
			return;
		}
			
		if (!OnLog) {
			OnLog = new ScriptInvoker();
		}
		
		OnLog.Invoke(level, msg);
		
		if (level == LogLevel.ERROR) {
			Error2("Editor Error", msg);
			return;
		}
		
		string loglevel = typename.EnumToString(LogLevel, level);
		
		if (msg.Contains("::")) {
			TStringArray msg_split();
			msg.Split(":", msg_split);
			
			if (ExclusiveLogMode != string.Empty && ExclusiveLogMode != msg_split[0]) {
				return;
			}
			
			if (m_LastCalledType != msg_split[0]) {
				m_LastCalledType = msg_split[0];
				DoLog(EditorLog, string.Format("[%1::%2]:", loglevel, m_LastCalledType));
			}			

			DoLog(EditorLog, string.Format("	%1", msg.Substring(msg_split[0].Length() + 2, msg.Length() - msg_split[0].Length() - 2)));
		} 
		else if (ExclusiveLogMode == string.Empty) {
			DoLog(EditorLog, string.Format("[%1] %2", loglevel, msg));
		}
	}
	
	static void SetLevel(LogLevel log_level)
	{
		m_CurrentLogLevel = log_level;
	}
	
	static LogLevel GetLevel()
	{
		return m_CurrentLogLevel;
	}
	
	static void Trace(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.TRACE);
	}
	
	static void Debug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.DEBUG);
	}	
	
	static void Info(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.INFO);
	}	
	
	static void Warning(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.WARNING);
	}	
	
	static void Error(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		msg = string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		EditorPrint(msg, LogLevel.ERROR);
		
#ifdef COMPONENT_SYSTEM
		Workbench.Dialog("Editor Log Error", msg);
#else
		MessageBox.ShowSynchronous("Error", msg, MessageBoxButtons.OK);
#endif
	}
}