

class OnObjectCreateEventArgs
{
	EditorObject target;
	vector CreatePosition;
	int mouse_x, mouse_y;
}


// ObjectSelectedEventArgs
// 0 EditorObject:  Target
// 1 bool: Selection State
typedef Param2<EditorObject, bool> ObjectSelectedEventArgs;


class EditorEvents 
{
	static ref ScriptInvoker OnObjectCreated = new ScriptInvoker();
	static ref ScriptInvoker OnObjectSelectionChanged = new ScriptInvoker();
	static ref ScriptInvoker OnObjectDrag = new ScriptInvoker();
	static ref ScriptInvoker OnObjectDrop = new ScriptInvoker();
	
	void EditorEvents()
	{
		Print("EditorEvents");
		OnObjectCreated = new ScriptInvoker();
		OnObjectSelectionChanged = new ScriptInvoker();
		OnObjectDrag = new ScriptInvoker();
		OnObjectDrop = new ScriptInvoker();
	}
	
	static void ObjectCreateInvoke(Class context, EditorObject obj) 
	{
		Print("EditorEvents::ObjectCreate");
		OnObjectCreated.Invoke(context, obj);	
	}
	
	static void ObjectSelectedInvoke(Class context, ObjectSelectedEventArgs args) 
	{
		//Print("EditorEvents::ObjectSelected");
		OnObjectSelectionChanged.Invoke(context, args);
	}
	
	static void DragInvoke(Class context, EditorObject obj)
	{
		Print("EditorEvents::Drag");
		OnObjectDrag.Invoke(context, obj);
	}
	
	static void DropInvoke(Class context, EditorObject obj)
	{
		Print("EditorEvents::Drop");
		OnObjectDrop.Invoke(context, obj);
	}
}

