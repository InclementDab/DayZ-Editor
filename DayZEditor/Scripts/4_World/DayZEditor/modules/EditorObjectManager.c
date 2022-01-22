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
	
	protected ref array<ref EditorPlaceableItem>	m_PlaceableObjects;
	
	protected ref map<string, EditorPlaceableItem>	m_PlaceableObjectsByType;
	
	// lookup table by p3d
	protected ref map<string, ref array<EditorPlaceableItem>>	m_PlaceableObjectsByP3d;
	
	protected EditorStatistics m_EditorStatistics;

	// Current Selected PlaceableListItem
	EditorPlaceableItem CurrentSelectedItem;
	
	override void Init()
	{
		EditorLog.Trace("EditorObjectManager::Init");
		m_EditorStatistics  = EditorStatistics.GetInstance();
		m_WorldObjectIndex 	= new EditorObjectMap();
		m_PlacedObjects 	= new EditorObjectMap();
		m_SelectedObjects 	= new EditorObjectMap();
		m_DeletedObjects	= new EditorDeletedObjectMap();
		m_SelectedDeletedObjects	= new EditorDeletedObjectMap();

		m_EditorObjectRefs = new map<int, ref EditorObject>();
		m_EditorDeletedObjectRefs = new map<int, ref EditorDeletedObject>();

		// Loads placeable objects	
		g_Game.ReportProgress("Loading Placeable Objects");
		
		m_PlaceableObjects = {};
		m_PlaceableObjectsByType = new map<string, EditorPlaceableItem>;
		m_PlaceableObjectsByP3d = new map<string, ref array<EditorPlaceableItem>>();
		TStringArray config_paths = {};
		config_paths.Insert(CFG_VEHICLESPATH);
		config_paths.Insert(CFG_WEAPONSPATH);
		config_paths.Insert(CFG_MAGAZINESPATH);
		
		foreach (string path: config_paths) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type;
		        GetGame().ConfigGetChildName(path, i, type);
				if (GetGame().ConfigGetInt(path + " " + type + " scope") < 1 && !GetEditor().Settings.ShowScopeZeroObjects) {
					continue;
				}
				
				EditorPlaceableItem placeable_item = EditorPlaceableItem.Create(path, type);
				if (!placeable_item) {
					continue;
				}
				
				m_PlaceableObjects.Insert(placeable_item); 
				m_PlaceableObjectsByType[placeable_item.Type] = placeable_item;
				
				if (!m_PlaceableObjectsByP3d[placeable_item.Model]) {
					m_PlaceableObjectsByP3d[placeable_item.Model] = new array<EditorPlaceableItem>();
				}
				
				m_PlaceableObjectsByP3d[placeable_item.Model].Insert(placeable_item);
		    }
		}
		
		// Statics that belong to Editor / DF
		m_PlaceableObjects.Insert(EditorPlaceableItem.Create(NetworkSpotLight));
		m_PlaceableObjects.Insert(EditorPlaceableItem.Create(NetworkPointLight));
		m_PlaceableObjects.Insert(EditorPlaceableItem.Create(NetworkParticleBase));
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
		
		m_EditorStatistics.EditorPlacedObjects++;
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
		
		m_EditorStatistics.EditorRemovedObjects++;
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
		m_EditorStatistics.Save();
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
	
	EditorPlaceableItem GetPlaceableObject(string type)
	{
		return m_PlaceableObjectsByType[type];
	}
	
	array<ref EditorPlaceableItem> GetPlaceableObjects()
	{
		return m_PlaceableObjects;
	}
	
	// return a list of objects that use this p3d, useful for finding adequite replacements for 
	// otherwise unplaceable objects
	array<EditorPlaceableItem> GetReplaceableObjects(string p3d)
	{
		return m_PlaceableObjectsByP3d[p3d];
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