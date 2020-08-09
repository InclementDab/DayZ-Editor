static void EditorPrint( string s )
{
	#ifdef EDITORPRINT
	Print( "[EDITOR DEBUG]: " + s );
	#endif
}
