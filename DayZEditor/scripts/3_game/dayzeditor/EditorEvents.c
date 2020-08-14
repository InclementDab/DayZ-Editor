
class EditorEvents
{
	
	static ref ScriptInvoker OnEditorSetActive = new ScriptInvoker();
	
	static void EditorSetActive(Class context, bool state)
	{
		EditorLog.Trace("EditorEvents::EditorSetActive");
		OnEditorSetActive.Invoke(context, state);
	}
}
