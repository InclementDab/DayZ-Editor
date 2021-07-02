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
	
	static ref map<int, Object> WorldObjects = new map<int, Object>();
		
	// Current Selected PlaceableListItem
	EditorPlaceableItem CurrentSelectedItem;
	
	private bool m_IsWorldCacheLoaded;
	
	override void Init()
	{
		EditorLog.Trace("EditorObjectManager::Init");
		m_WorldObjectIndex 	= new EditorObjectMap();
		m_PlacedObjects 	= new EditorObjectMap();
		m_SelectedObjects 	= new EditorObjectMap();
		m_DeletedObjects	= new EditorDeletedObjectMap();
		m_SelectedDeletedObjects	= new EditorDeletedObjectMap();
	}
	
	EditorObject CreateObject(EditorObjectData editor_object_data)
	{		
		EditorLog.Trace("EditorObjectManager::CreateObject");

		EditorObject editor_object = new EditorObject(editor_object_data);
		if (!editor_object) return null;
		
		m_PlacedObjects.InsertEditorObject(editor_object);
		m_WorldObjectIndex.Insert(editor_object.GetWorldObject().GetID(), editor_object);
		
		EditorEvents.ObjectCreated(this, editor_object);
		return editor_object;
	}

	void DeleteObject(EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeleteObject");
		m_SelectedObjects.RemoveEditorObject(target);
		m_PlacedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeleted(this, target);		
		delete target;
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
		
		foreach (EditorDeletedObject deleted_object: m_SelectedDeletedObjects)
			DeselectHiddenObject(deleted_object);
	}
	
	// Hidden object stuff
	void HideMapObject(EditorDeletedObject target)
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
	
	void UnhideMapObject(EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::UnhideMapObject");
		m_DeletedObjects.RemoveEditorDeletedObject(target);
		delete target;
	}
	
	void SelectHiddenObject(EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::SelectHiddenObject");
		m_SelectedDeletedObjects.InsertEditorDeletedObject(target);
		EditorEvents.DeletedObjectSelected(this, target);
		target.OnSelected();
	}
	
	void DeselectHiddenObject(EditorDeletedObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeselectHiddenObject");
		m_SelectedDeletedObjects.RemoveEditorDeletedObject(target);
		EditorEvents.DeletedObjectDeselected(this, target);
		target.OnDeselected();
	}
	
	void ToggleHiddenObjectSelection(EditorDeletedObject target)
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
				
		if (GetEditor().Settings.DisableWorldCache) {
			return;
		}
		
		LoadWorldCache();
	}
	
	void LoadWorldCache()
	{
		// Loads all world objects into a map
		WorldObjects.Clear();
		
		g_Game.ReportProgress("Caching Map Objects");
		EditorLog.Info("Caching Map Objects");
		
		array<Object> objects = {};
		array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(Vector(7500, 0, 7500), 100000, objects, cargos);

		foreach (Object o: objects) {
			if (o.IsInherited(Man)) {
				continue;
			}
			
			WorldObjects.Insert(o.GetID(), o);	
		}
		
		g_Game.ReportProgress(string.Format("Cached %1 map objects", WorldObjects.Count().ToString()));
		EditorLog.Info("Cached %1 map objects", WorldObjects.Count().ToString());
		
		m_IsWorldCacheLoaded = true;
	}
	
	bool IsWorldCacheLoaded()
	{
		return m_IsWorldCacheLoaded;
	}
	
	bool IsObjectHidden(EditorDeletedObject deleted_object)
	{
		return (IsObjectHidden(deleted_object.GetID()));
	}
	
	bool IsObjectHidden(int id)
	{
		return (m_DeletedObjects[id] != null);
	}
	
	Object GetWorldObject(int id)
	{
		if (WorldObjects[id])
			return WorldObjects[id];
		
		return null;
	}
	
	map<int, Object> GetWorldObjects()
	{
		return WorldObjects;
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