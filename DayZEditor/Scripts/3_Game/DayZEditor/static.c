#define EDITOR_PRINT



class EditorLog
{
	static LogLevel CurrentLogLevel = LogLevel.TRACE;
	static string ExclusiveLogMode;
	
	private static string m_LastCalledType;	
	
	static void EditorPrint(string msg, LogLevel level)
	{
		if (level >= EditorLog.CurrentLogLevel) {
			
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
					PrintFormat("[%1::%2]:", loglevel, m_LastCalledType);
				}
				

				PrintFormat("	%1", msg.Substring(msg_split[0].Length() + 2, msg.Length() - msg_split[0].Length() - 2));
			} 
			else if (ExclusiveLogMode == string.Empty) {
				PrintFormat("[%1] %2", loglevel, msg);
			}
		}
	}
	
	
	static void Trace(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITOR_PRINT		
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.TRACE);
#endif
	}
	
	static void Debug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITOR_PRINT
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.DEBUG);
#endif
	}	
	
	static void Info(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITOR_PRINT
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.INFO);
#endif
	}	
	
	static void Warning(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EDITOR_PRINT
		EditorPrint(string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9), LogLevel.WARNING);
#endif
	}	
	
	static void Error(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		msg = string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		EditorPrint(msg, LogLevel.ERROR);
		
		ErrorDialog(msg);
	}
	
	private static void ErrorDialog(string message)
	{
#ifdef COMPONENT_SYSTEM
		Workbench.Dialog("Editor Log Error", message);
#else
		MessageBox.ShowSynchronous("Error", message, MessageBoxButtons.OK);
#endif
	}
}


static bool RecursiveGetParent(out Widget w, string name)
{
	if (!w) 
		return false;
	
	if (w.GetName() == name) 
		return true;
	
	w = w.GetParent();
	
	if (w) {
		return RecursiveGetParent(w, name);
	}
	
	return false;
}

static Widget GetWidgetRoot(Widget w)
{
	Widget parent = w;
	_GetWidgetRoot(parent);
	return parent;
}

static void _GetWidgetRoot(out Widget w)
{
	if (w.GetParent() == null) {
		return;	
	}
	
	w = w.GetParent();
	_GetWidgetRoot(w);
}


// remove this once BI adds set into string
typedef string BetterString;
class BetterString: string
{
	static string value;
	void Set(int n, string _value)
	{
		string pre = value.Substring(0, n);
		n += 1;
		int length = value.Length() - n;
		string post = value.Substring(n, length);
		value = pre + _value + post;
	}	
}

static const int COLOR_SALMON	= ARGB(255, 192, 94, 83);
static const int COLOR_SALMON_A	= ARGB(128, 50, 100, 100);
static const int COLOR_CANDY 	= -1618884;
static const int COLOR_APPLE 	= -9785268;
static const int COLOR_JELLY 	= -1010901;
static const int COLOR_PALE_B   = ARGB(255, 50, 100, 200);
static const int COLOR_BLACK	= -16777216;
static const int COLOR_WHITE 	= -1;
static const int COLOR_WHITE_A	= 855638015;
static const int COLOR_EMPTY 	= 0;