class EditorObjectManagerModule: JMModuleBase
{
	// strong reference of Placed Objects
	private ref EditorObjectMap					m_PlacedObjects;
	
	// Stored list of all selected Objects
	private ref EditorObjectMap					m_SelectedObjects;
	
	// Stored list of all Placed Objects, indexed by their WorldObject ID
	private ref EditorObjectMap					m_WorldObjectIndex;
	
	// Stored list of all Hidden Objects, indexed by their WorldObject ID
	private ref EditorDeletedObjectMap 			m_DeletedObjects;
	
	private ref EditorDeletedObjectMap			m_SelectedDeletedObjects;

	// Current Selected PlaceableListItem
	EditorPlaceableItem CurrentSelectedItem;
		
	override void Init()
	{
		EditorLog.Trace("EditorObjectManager::Init");
		m_WorldObjectIndex 	= new EditorObjectMap();
		m_PlacedObjects 	= new EditorObjectMap();
		m_SelectedObjects 	= new EditorObjectMap();
		m_DeletedObjects	= new EditorDeletedObjectMap();
		m_SelectedDeletedObjects	= new EditorDeletedObjectMap();
	}
	
	EditorObject CreateObject(notnull EditorObjectData editor_object_data)
	{		
		EditorLog.Trace("EditorObjectManager::CreateObject");

		EditorObject editor_object = new EditorObject(editor_object_data);
		if (!editor_object) return null;
		
		m_PlacedObjects.InsertEditorObject(editor_object);
		m_WorldObjectIndex.Insert(editor_object.GetWorldObject().GetID(), editor_object);
		
		EditorEvents.ObjectCreated(this, editor_object);
		return editor_object;
	}

	void DeleteObject(notnull EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeleteObject");
		m_SelectedObjects.RemoveEditorObject(target);
		m_PlacedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeleted(this, target);		
		delete target;
	}
	
	// Call to select an object
	void SelectObject(notnull EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::SelectObject");
		m_SelectedObjects.InsertEditorObject(target);
		EditorEvents.ObjectSelected(this, target);
		target.OnSelected();
		
		// todo perhaps propagate selections to the children of the object?
	}
	
	// Call to deselect an object
	void DeselectObject(notnull EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeselectObject");
		m_SelectedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeselected(this, target);
		target.OnDeselected();
	}	
		
	// Call to toggle selection
	void ToggleSelection(notnull EditorObject target)
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
		
		foreach (EditorDeletedObject deleted_object: m_SelectedDeletedObjects)
			DeselectHiddenObject(deleted_object);
	}
	
	// Hidden object stuff
	void HideMapObject(notnull EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::HideMapObject");
		m_DeletedObjects.InsertEditorDeletedObject(target);
	}
	
	void UnhideMapObject(int target)
	{
		EditorLog.Trace("EditorObjectManager::UnhideMapObject");
		delete m_DeletedObjects[target];
		m_DeletedObjects.Remove(target);
	}
	
	void UnhideMapObject(notnull EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::UnhideMapObject");
		m_DeletedObjects.RemoveEditorDeletedObject(target);
		delete target;
	}
	
	void SelectHiddenObject(notnull EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::SelectHiddenObject");
		m_SelectedDeletedObjects.InsertEditorDeletedObject(target);
		EditorEvents.DeletedObjectSelected(this, target);
		target.OnSelected();
	}
	
	void DeselectHiddenObject(notnull EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeselectHiddenObject");
		m_SelectedDeletedObjects.RemoveEditorDeletedObject(target);
		EditorEvents.DeletedObjectDeselected(this, target);
		target.OnDeselected();
	}
	
	void ToggleHiddenObjectSelection(notnull EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::ToggleHiddenObjectSelection");
		if (target.IsSelected())
			DeselectHiddenObject(target);
		else
			SelectHiddenObject(target);
	}
		
	void Clear()
	{
		m_WorldObjectIndex.Clear();
		m_PlacedObjects.Clear();
		m_SelectedObjects.Clear();		
		m_DeletedObjects.ClearSafe();
		m_SelectedDeletedObjects.ClearSafe();		
	}
				
	override void OnMissionStart()
	{
		// On Load unhide em all
		CF.ObjectManager.UnhideAllMapObjects();
	}
		
	bool IsObjectHidden(EditorDeletedObject deleted_object)
	{
		return (IsObjectHidden(deleted_object.GetID()));
	}
	
	bool IsObjectHidden(EditorDeletedObjectData deleted_object_data)
	{
		return (IsObjectHidden(deleted_object_data.ID));
	}
	
	bool IsObjectHidden(int id)
	{
		return (m_DeletedObjects[id] != null);
	}
			
	EditorObjectMap GetSelectedObjects() 
	{
		return m_SelectedObjects; 
	}
	
	EditorObjectMap GetPlacedObjects()
	{
		return m_PlacedObjects; 
	}
	
	EditorDeletedObjectMap GetDeletedObjects()
	{
		return m_DeletedObjects;
	}
	
	EditorDeletedObjectMap GetSelectedHiddenObjects()
	{
		return m_SelectedDeletedObjects;
	}
		
	EditorObject GetPlacedObjectById(int id)
	{
		return m_PlacedObjects.Get(id); 
	}
	
	EditorObject GetEditorObject(int id)
	{
		return m_PlacedObjects.Get(id); 
	}
	
	EditorObject GetEditorObject(notnull Object world_object) 
	{
		return m_WorldObjectIndex.Get(world_object.GetID()); 
	}
	
	override bool IsClient() 
	{	
		return true;
	}
	
	override bool IsServer() 
	{
		return true;
	}
}