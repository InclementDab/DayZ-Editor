[RegisterLogger(EditorLog)]
class EditorLog: LoggerBase
{
	static ref ScriptInvoker OnLog;
	protected static LogLevel m_CurrentLogLevel = LogLevel.INFO;
	
	static void EditorPrint(string msg, LogLevel level)
	{	
		if (level < m_CurrentLogLevel) {
			return;
		}
			
		if (!OnLog) {
			OnLog = new ScriptInvoker();
		}
		
		int hour, minute, second;
		GetHourMinuteSecond(hour, minute, second);
		int time = GetGame().GetTime();
		string formatted_time = string.Format("%1:%2:%3 %4", hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2), time);
		
		OnLog.Invoke(level, msg);
		
		if (level == LogLevel.ERROR) {
			Error2("Editor Error", msg);
			return;
		}
	
		DoLog(EditorLog, string.Format("[%1][%3]		: %2", typename.EnumToString(LogLevel, level), msg, formatted_time));
		
#ifdef DIAG_DDEVELOPER
		Print(String(msg)); // maybe add some extra logic to avoid clogging
#endif
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
	
	override bool GetLogMask()
	{
		return LoggerInstanceMask.CLIENT | LoggerInstanceMask.SERVER;
	}
}