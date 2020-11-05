
class EditorObjectManagerModule: JMModuleBase
{
	// strong reference of Placed Objects
	private ref EditorObjectMap					m_PlacedObjects;
	
	// Stored list of all selected Objects
	private ref EditorObjectMap					m_SelectedObjects;
	
	// Stored list of all Placed Objects, indexed by their WorldObject ID
	private ref EditorObjectMap					m_WorldObjectIndex;
	
	private ref map<int, Object> 				m_MapObjects;
	
	EditorObjectMap GetSelectedObjects() 
		return m_SelectedObjects; 
	
	EditorObjectMap GetPlacedObjects()
		return m_PlacedObjects; 
		
	EditorObject GetPlacedObjectById(int id)
		return m_PlacedObjects.Get(id); 
	
	EditorObject GetEditorObject(int id)
		return m_PlacedObjects.Get(id); 
	
	EditorObject GetEditorObject(notnull Object world_object) 
		return m_WorldObjectIndex.Get(world_object.GetID()); 
	
	// Current Selected PlaceableListItem
	EditorPlaceableItem CurrentSelectedItem;
	
	override void Init()
	{
		EditorLog.Trace("EditorObjectManager::CreateObjects");
		m_WorldObjectIndex 	= new EditorObjectMap();
		m_PlacedObjects 	= new EditorObjectMap();
		m_SelectedObjects 	= new EditorObjectMap();
		m_MapObjects 		= new map<int, Object>();
	}
	
	
	EditorObject CreateObject(ref EditorObjectData editor_object_data)
	{		
		EditorLog.Trace("EditorObjectManager::CreateObject");

		EditorObject editor_object = new EditorObject(editor_object_data);
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
	}
	
	void Clear()
	{
		m_WorldObjectIndex.Clear();
		m_PlacedObjects.Clear();
		m_SelectedObjects.Clear();
	}
	
	array<int> GetDeletedObjects()
	{		
		array<int> deleted_objects = {};
		array<Object> registered = CF__ObjectManager.GetRegisteredObjects();
		foreach (Object obj: registered) {
			if (obj) {
				deleted_objects.Insert(obj.GetID());
			}
		}
		
		return deleted_objects;
	}
			
	override bool IsClient() 
	{	
		return true;
	}
	
	override bool IsServer() 
	{
		return true;
	}
	
	override void OnMissionStart()
	{
		// Loads all world objects into a map
		ref array<Object> objects = {};
		ref array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(Vector(7500, 0, 7500), 20000, objects, cargos);

		foreach (Object o: objects) {
			m_MapObjects.Insert(o.GetID(), o);
		}
		
		EditorLog.Info("Loaded %1 map objects", m_MapObjects.Count().ToString());
	}
	
	Object GetWorldObject(int id)
	{
		return m_MapObjects[id];
	}
	
	ref map<int, Object> GetWorldObjects()
	{
		return m_MapObjects;
	}
}

