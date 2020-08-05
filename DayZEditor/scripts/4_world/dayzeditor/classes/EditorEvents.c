



// remove the static invokes and let the Editor class manage it
class EditorEvents 
{
	static ref ScriptInvoker OnObjectCreated = new ScriptInvoker();
	static ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	static ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	static ref ScriptInvoker OnObjectDrag = new ScriptInvoker();
	static ref ScriptInvoker OnObjectDrop = new ScriptInvoker();
	
	void EditorEvents()
	{
		Print("EditorEvents");
		OnObjectCreated = new ScriptInvoker();
		OnObjectSelected = new ScriptInvoker();
		OnObjectDeselected = new ScriptInvoker();
		OnObjectDrag = new ScriptInvoker();
		OnObjectDrop = new ScriptInvoker();
	}
	
	static void ObjectCreateInvoke(Class context, EditorObject obj) 
	{
		Print("EditorEvents::ObjectCreate");
		OnObjectCreated.Invoke(context, obj);	
	}
	
	static void ObjectSelectedInvoke(Class context, EditorObject obj) 
	{
		//Print("EditorEvents::ObjectSelected");
		OnObjectSelected.Invoke(context, obj);
	}
	
	static void ObjectDeselectedInvoke(Class context, EditorObject obj) 
	{
		//Print("EditorEvents::OnObjectDeselected");
		OnObjectDeselected.Invoke(context, obj);
	}
	
	static void DragInvoke(Class context, EditorObject obj, ref RaycastRVResult raycast_result = null)
	{
		Print("EditorEvents::Drag");
		OnObjectDrag.Invoke(context, obj, raycast_result);
	}
	
	static void DropInvoke(Class context, EditorObject obj)
	{
		Print("EditorEvents::Drop");
		OnObjectDrop.Invoke(context, obj);
	}
}

