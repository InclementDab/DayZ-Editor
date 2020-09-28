
class EditorObjectManagerModule: JMModuleBase
{
	private ref EditorObjectSet 				m_PlacedObjects;
	private ref EditorObjectSet					m_SelectedObjects;
	private ref EditorActionStack 				m_ActionStack;
	private ref map<int, int> 					m_PlacedObjectIndex;
	
	EditorObjectSet GetSelectedObjects() 
		return m_SelectedObjects; 
	
	EditorObjectSet GetPlacedObjects()
		return m_PlacedObjects; 
		
	EditorObject GetPlacedObjectById(int id)
		return m_PlacedObjects.Get(id); 
	
	EditorObject GetEditorObject(int id)
		return m_PlacedObjects.Get(id); 
	
	EditorObject GetEditorObject(notnull Object world_object)
		return GetEditorObject(m_PlacedObjectIndex.Get(world_object.GetID())); 
	

	override void Init()
	{
		EditorLog.Trace("EditorObjectManager::CreateObjects");
		
		m_PlacedObjectIndex = new map<int, int>();
		m_PlacedObjects 	= new EditorObjectSet();
		m_SelectedObjects 	= new EditorObjectSet();
		m_ActionStack 		= new EditorActionStack();
	}
	
	ref EditorObjectSet CreateObjects(ref EditorObjectDataSet data_list, bool create_undo = true)
	{
		EditorLog.Trace("EditorObjectManager::CreateObjects");
		EditorObjectSet object_set = new EditorObjectSet();
		EditorAction action = new EditorAction("Delete", "Create");
		foreach (EditorObjectData editor_object_data: data_list) {
			
			EditorObject editor_object = EditorObject.Create(editor_object_data);
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));		
			
			// Register object trigger events
			object_set.InsertEditorObject(editor_object);	
			m_PlacedObjects.InsertEditorObject(editor_object);
			m_PlacedObjectIndex.Insert(editor_object.GetWorldObject().GetID(), editor_object.GetID());
			EditorEvents.ObjectCreated(this, editor_object);
		}
		
		if (create_undo) {
			m_ActionStack.InsertAction(action);
			m_ActionStack.UpdateDebugReadout(GetEditor().GetEditorHud().GetEditorHudController().DebugActionStackListbox);
		}
		
		return object_set;
	}
	
	
	EditorObject CreateObject(ref EditorObjectData editor_object_data, bool create_undo = true)
	{		
		EditorLog.Trace("EditorObjectManager::CreateObject");
		
		EditorObject editor_object = EditorObject.Create(editor_object_data);
		
		
		EditorAction action = new EditorAction("Delete", "Create");;
		action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		
		m_PlacedObjects.InsertEditorObject(editor_object);
		m_PlacedObjectIndex.Insert(editor_object.GetWorldObject().GetID(), editor_object.GetID());
		EditorEvents.ObjectCreated(this, editor_object);
		
		if (create_undo) {
			m_ActionStack.InsertAction(action);
			// this crashes smile :)
			//m_ActionStack.UpdateDebugReadout(GetEditor().GetEditorHud().GetEditorHudController().DebugActionStackListbox);
		}
			
	
		return editor_object;
	}
	
	EditorObject CreateFromObject(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		EditorLog.Trace("EditorObjectManager::CreateFromObject");
		// If object already exists
		if (m_PlacedObjects.Get(target.GetID())) 
			return m_PlacedObjects.Get(target.GetID());
		
		EditorObject editor_object = EditorObject.Create(target, flags);
		m_PlacedObjects.InsertEditorObject(editor_object);
		m_PlacedObjectIndex.Insert(editor_object.GetWorldObject().GetID(), editor_object.GetID());
		EditorEvents.ObjectCreated(this, editor_object);
		
		return editor_object;
	}

	void DeleteObject(EditorObject target, bool create_undo = true)
	{
		EditorLog.Trace("EditorObjectManager::DeleteObject");
		m_SelectedObjects.RemoveEditorObject(target);
		m_PlacedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeleted(this, target);
		
		if (create_undo) {
			EditorAction action = new EditorAction("Create", "Delete");
			action.InsertUndoParameter(target, new Param1<int>(target.GetID()));
			action.InsertRedoParameter(target, new Param1<int>(target.GetID()));
			m_ActionStack.InsertAction(action);
			m_ActionStack.UpdateDebugReadout(GetEditor().GetEditorHud().GetEditorHudController().DebugActionStackListbox);
		}
		
		delete target;
	}
	
	void DeleteObjects(EditorObjectSet target, bool create_undo = true)
	{
		EditorLog.Trace("EditorObjectManager::DeleteObjects");
		
		EditorAction action = new EditorAction("Create", "Delete");
		
		foreach (EditorObject editor_object: target) {
			m_SelectedObjects.RemoveEditorObject(editor_object);
			m_PlacedObjects.RemoveEditorObject(editor_object);
			EditorEvents.ObjectDeleted(this, editor_object);
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			delete editor_object;
		}	
			
		if (create_undo) {
			m_ActionStack.InsertAction(action);
			m_ActionStack.UpdateDebugReadout(GetEditor().GetEditorHud().GetEditorHudController().DebugActionStackListbox);
		}
	}
	
	// Call to select an object
	void SelectObject(EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::SelectObject");
		m_SelectedObjects.InsertEditorObject(target);
		EditorEvents.ObjectSelected(this, target);
		target.OnSelected();
	}
	
	// Call to deselect an object
	void DeselectObject(EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeselectObject");
		m_SelectedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeselected(this, target);
		target.OnDeselected();
	}	
	
	// Call to toggle selection
	void ToggleSelection(EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::ToggleSelection");
		if (target.IsSelected())
			DeselectObject(target);
		else
			SelectObject(target);
		
	}
	
	// Call to clear selection
	void ClearSelection()
	{
		EditorLog.Trace("EditorObjectManager::ClearSelection");		
		foreach (EditorObject editor_object: m_SelectedObjects)
			DeselectObject(editor_object);
	}
	
	void Undo()
	{
		EditorLog.Trace("EditorObjectManager::Undo");
		foreach (EditorAction action: m_ActionStack) {
			if (!action.IsUndone()) {
				action.CallUndo();
				return;
			}
		}
	}
	
	void Redo()
	{
		EditorLog.Trace("EditorObjectManager::Redo");
		for (int i = m_ActionStack.Count() - 1; i >= 0; i--) {
			if (m_ActionStack[i] != null && m_ActionStack[i].IsUndone()) {
				m_ActionStack[i].CallRedo();
				return;
			}
		}
	}
	
	bool CanUndo() 
	{
		foreach (EditorAction action: m_ActionStack) {
			if (!action.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool CanRedo() 
	{
		for (int i = m_ActionStack.Count() - 1; i >= 0; i--) {
			if (m_ActionStack[i] != null && m_ActionStack[i].IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	
	override bool IsClient() 
		return true;
	
	override bool IsServer() 
		return true;
	
}

