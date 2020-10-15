
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
	
	// Called AFTER Object is selected
	static ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	static void ObjectSelected(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectSelected");
		OnObjectSelected.Invoke(context, target);
	}
	
	// Called AFTER Object is deselected
	static ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	static void ObjectDeselected(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectDeselected");
		OnObjectDeselected.Invoke(context, target);
	}
	
	// Called AFTER Editor starts placing
	static ref ScriptInvoker OnStartPlacing = new ScriptInvoker();
	static void StartPlacing(Class context, EditorPlaceableItem target)
	{
		EditorLog.Trace("EditorEvents::StartPlacing");
		OnStartPlacing.Invoke(context, target);
	}
	
	// Called AFTER Editor places object
	static ref ScriptInvoker OnObjectPlaced = new ScriptInvoker();
	static void ObjectPlaced(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectPlaced");
		OnObjectPlaced.Invoke(context, target);
	}
	
	// Called AFTER Editor stops
	static ref ScriptInvoker OnStopPlacing = new ScriptInvoker();
	static void StopPlacing(Class context)
	{
		EditorLog.Trace("EditorEvents::StopPlacing");
		OnStopPlacing.Invoke(context);
	}
	
	// Called AFTER Map is Toggled!
	static ref ScriptInvoker OnMapToggled = new ScriptInvoker();
	static void MapToggled(Class context, MapWidget editor_map, bool state)
	{
		EditorLog.Trace("EditorEvents::MapToggled");
		OnMapToggled.Invoke(context, editor_map, state);
	}
}
