
class EditorEvents
{
	// Called AFTER Object is created
	static ref ScriptInvoker OnObjectCreated = new ScriptInvoker();
	static void ObjectCreated(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectCreated");
		OnObjectCreated.Invoke(context, target);
	}
	
	// Called BEFORE Object is deleted
	static ref ScriptInvoker OnObjectDeleted = new ScriptInvoker();
	static void ObjectDeleted(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectDeleted");
		OnObjectDeleted.Invoke(context, target);
	}
	
	// Call to select an Editor Object
	static ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	static void SelectObject(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::SelectObject");
		OnObjectSelected.Invoke(context, target);
	}
	
	// Call to deselect an Editor Object
	static ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	static void DeselectObject(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::DeselectObject");
		OnObjectDeselected.Invoke(context, target);
	}
}
