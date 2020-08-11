enum LogSeverity {
	TRACE, 		// Logs at beginning of each method
	DEBUG,		// Debug info within conditionals etc...
	INFO,		// Helpful info within conditionals etc... (likely more user friendly)
	WARNING,	// Warnings back to the user. unlikely to be mission critical
	ERROR		// oof
};


static LogSeverity LOG_SEVERITY = LogSeverity.TRACE;

static void EditorPrint(string s, LogSeverity severity = LogSeverity.TRACE)
{
	#ifdef EDITORPRINT
	if (severity >= LOG_SEVERITY)
		Print(string.Format("[EDITOR::%1] %2", typename.EnumToString(LogSeverity, severity), s));
	#endif
}
