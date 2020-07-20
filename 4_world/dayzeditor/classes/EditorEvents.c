

class OnObjectCreateEventArgs
{
	EditorObject target;
	vector CreatePosition;
	int mouse_x, mouse_y;
}

class ObjectSelectedEventArgs
{
	EditorObject Target;
	bool SelectionStatus;
	void ObjectSelectedEventArgs(EditorObject target, bool selectionstatus)
	{
		Target = target; 
		SelectionStatus = selectionstatus;
	}
}




class EditorEvents 
{
	ref ScriptInvoker OnObjectCreated;
	ref ScriptInvoker OnObjectSelectionChanged;
	ref ScriptInvoker OnObjectDrag;
	ref ScriptInvoker OnObjectDrop;
	
	void EditorEvents()
	{
		OnObjectCreated = new ScriptInvoker();
		OnObjectSelectionChanged = new ScriptInvoker();
		OnObjectDrag = new ScriptInvoker();
		OnObjectDrop = new ScriptInvoker();
	}
	
	void ObjectCreateInvoke(Class context, EditorObject obj) 
	{
		Print("EditorEvents::ObjectCreate");
		OnObjectCreated.Invoke(context, obj);	
	}
	
	void ObjectSelectedInvoke(Class context, ObjectSelectedEventArgs args) 
	{
		//Print("EditorEvents::ObjectSelected");
		OnObjectSelectionChanged.Invoke(context, args);
	}
	
	void DragInvoke(Class context, EditorObject obj)
	{
		Print("EditorEvents::Drag");
		OnObjectDrag.Invoke(context, obj);
	}
	
	void DropInvoke(Class context, EditorObject obj)
	{
		Print("EditorEvents::Drop");
		OnObjectDrop.Invoke(context, obj);
	}
}

