class EditorObjectManagerModule: JMModuleBase
{
	// strong reference to objects, insert and remove expectedly
	protected ref map<int, ref EditorObject> 		m_EditorObjectRefs;
	protected ref map<int, ref EditorDeletedObject> m_EditorDeletedObjectRefs;
	
	// Stored list of all Placed Objects
	protected ref EditorObjectMap					m_PlacedObjects;
	
	// Stored list of all selected Objects
	protected ref EditorObjectMap					m_SelectedObjects;
	
	// Stored list of all Placed Objects, indexed by their WorldObject ID
	protected ref EditorObjectMap					m_WorldObjectIndex;
		
	// Stored list of all Hidden Objects, indexed by their WorldObject ID
	protected ref EditorDeletedObjectMap 			m_DeletedObjects;
	
	protected ref EditorDeletedObjectMap			m_SelectedDeletedObjects;

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

		m_EditorObjectRefs = new map<int, ref EditorObject>();
		m_EditorDeletedObjectRefs = new map<int, ref EditorDeletedObject>();
	}
	
	EditorObject CreateObject(notnull EditorObjectData editor_object_data)
	{		
		EditorLog.Trace("EditorObjectManager::CreateObject");

		EditorObject editor_object = new EditorObject(editor_object_data);
		if (!editor_object || !editor_object.GetWorldObject()) { 
			return null;
		}
				
		// strong ref
		m_EditorObjectRefs[editor_object.GetID()] = editor_object;
		
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
		
		// remove strong ref
		m_EditorObjectRefs.Remove(target.GetID());
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
		
		// strong ref
		m_EditorDeletedObjectRefs[target.GetID()] = target;
	
		m_DeletedObjects.InsertEditorDeletedObject(target);
	}
	
	void UnhideMapObject(int target)
	{
		EditorLog.Trace("EditorObjectManager::UnhideMapObject");
		m_DeletedObjects.Remove(target);
				
		// remove strong ref		
		delete m_EditorDeletedObjectRefs[target];
		m_EditorDeletedObjectRefs.Remove(target);
	}
	
	void UnhideMapObject(notnull EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::UnhideMapObject");
		m_DeletedObjects.RemoveEditorDeletedObject(target);

		// remove strong ref
		delete m_EditorDeletedObjectRefs[target.GetID()];
		m_EditorDeletedObjectRefs.Remove(target.GetID());
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
		
		m_EditorObjectRefs.Clear();
		m_EditorDeletedObjectRefs.Clear();
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
	
	bool IsObjectHidden(Object object)
	{
		return (CF.ObjectManager.IsMapObjectHidden(object));
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
		return m_PlacedObjects[id]; 
	}
	
	EditorObject GetEditorObject(int id)
	{
		return m_PlacedObjects[id]; 
	}
	
	EditorObject GetEditorObject(notnull Object world_object) 
	{
		return m_WorldObjectIndex.Get(world_object.GetID()); 
	}
	
	void Debug()
	{
		EditorLog.Debug("EditorObjectManager Debug Info");
		EditorLog.Debug(m_EditorObjectRefs.Count().ToString());
		EditorLog.Debug(m_EditorDeletedObjectRefs.Count().ToString());
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