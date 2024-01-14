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
	
	static ref ScriptInvoker OnDeletedObjectSelected = new ScriptInvoker();
	static void DeletedObjectSelected(Class context, EditorDeletedObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectDeselected");
		OnDeletedObjectSelected.Invoke(context, target);
	}
	
	static ref ScriptInvoker OnDeletedObjectDeselected = new ScriptInvoker();
	static void DeletedObjectDeselected(Class context, EditorDeletedObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectDeselected");
		OnDeletedObjectDeselected.Invoke(context, target);
	}
		
	static ref ScriptInvoker OnAddInHand = new ScriptInvoker();
	static void AddInHand(Class context, Object world_object, EditorHandData hand_data)
	{
		EditorLog.Trace("EditorEvents::StartPlacing");
		OnAddInHand.Invoke(context, world_object, hand_data);
	}
	
	static ref ScriptInvoker OnRemoveFromHand = new ScriptInvoker();
	static void RemoveFromHand(Class context, Object world_object, EditorHandData hand_data)
	{
		EditorLog.Trace("EditorEvents::RemoveFromHand");
		OnRemoveFromHand.Invoke(context, world_object, hand_data);
	}
	
	// Called AFTER Editor places object
	static ref ScriptInvoker OnObjectPlaced = new ScriptInvoker();
	static void ObjectPlaced(Class context, EditorObject target)
	{
		EditorLog.Trace("EditorEvents::ObjectPlaced");
		OnObjectPlaced.Invoke(context, target);
	}
	
	// Called AFTER Map is Toggled!
	static ref ScriptInvoker OnMapToggled = new ScriptInvoker();
	static void MapToggled(Class context, MapWidget editor_map, bool state)
	{
		EditorLog.Trace("EditorEvents::MapToggled");
		OnMapToggled.Invoke(context, editor_map, state);
	}
}