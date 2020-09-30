// Enum for LogLevel. Utility for plugins and loggers
enum LogLevel {
	TRACE, 		// Logs at beginning of each method
	DEBUG,		// Debug info within conditionals etc...
	INFO,		// Helpful info within conditionals etc... (likely more user friendly)
	WARNING,	// Warnings back to the user. unlikely to be mission critical
	ERROR,		// Fatal errors. Mission critical
	NONE
};
