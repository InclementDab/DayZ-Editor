



// remove the static invokes and let the Editor class manage it
class EditorEvents 
{
	static ref ScriptInvoker OnObjectCreated 	= new ScriptInvoker();
	static ref ScriptInvoker OnObjectDeleted 	= new ScriptInvoker();
	
	static ref ScriptInvoker OnObjectSelected 	= new ScriptInvoker();
	static ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	static ref ScriptInvoker OnSelectionCleared = new ScriptInvoker();
	static ref ScriptInvoker OnObjectDrag		= new ScriptInvoker();
	static ref ScriptInvoker OnObjectDrop		= new ScriptInvoker();
	static ref ScriptInvoker OnBrushChanged		= new ScriptInvoker();
	static ref ScriptInvoker OnSettingsChanged 	= new ScriptInvoker();
	static ref ScriptInvoker OnPlaceableCategoryChanged 	= new ScriptInvoker();
	
	

	
	static void ObjectCreateInvoke(Class context, EditorObject obj) 
	{
		//Print("EditorEvents::ObjectCreate");
		OnObjectCreated.Invoke(context, obj);	
	}	
	
	static void ObjectDeleteInvoke(Class context, EditorObject obj) 
	{
		Print("EditorEvents::DeleteObject");
		OnObjectDeleted.Invoke(context, obj);	
	}
	

	static void SelectObject(Class context, EditorObject obj) 
	{
		//Print("EditorEvents::ObjectSelected");
		OnObjectSelected.Invoke(context, obj);
	}
	
	
	
	
	static void DeselectObject(Class context, EditorObject obj) 
	{
		//Print("EditorEvents::OnObjectDeselected");
		OnObjectDeselected.Invoke(context, obj);
	}
	
	static void DragInvoke(Class context, EditorObject obj, ref RaycastRVResult raycast_result = null)
	{
		//Print("EditorEvents::DragInvoke");
		OnObjectDrag.Invoke(context, obj, raycast_result);
	}
	
	static void DropInvoke(Class context, EditorObject obj)
	{
		//Print("EditorEvents::DropInvoke");
		OnObjectDrop.Invoke(context, obj);
	}
	
	static void SettingsChangedInvoke(Class context, string changed, EditorSettings settings)
	{
		Print("EditorEvents::SettingsChangedInvoke");
		OnSettingsChanged.Invoke(context, changed, settings);
	}		
	
	static void PlaceableCategoryChangedInvoke(Class context, PlaceableObjectCategory category)
	{
		Print("EditorEvents::PlaceableCategoryChangedInvoke");
		OnPlaceableCategoryChanged.Invoke(context, category);
	}	
	
	
	// Maybe a new way of doing stuff
	static void ChangeBrush(Class context, EditorBrush brush)
	{
		Print("EditorEvents::ChangeBrush");
		OnBrushChanged.Invoke(context, brush);
	}
	
	static void ClearSelection(Class context)
	{
		Print("EditorEvents::ClearSelection");
		OnSelectionCleared.Invoke(context);
	}


}

