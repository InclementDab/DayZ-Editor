class EditorObjectManagerModule: Managed
{
	// strong reference to objects, insert and remove expectedly
	protected ref map<int, ref EditorObject> 		m_EditorObjectRefs = new map<int, ref EditorObject>();
	protected ref map<int, ref EditorDeletedObject> m_EditorDeletedObjectRefs = new map<int, ref EditorDeletedObject>();
	
	// Stored list of all Placed Objects
	protected ref EditorObjectMap					m_PlacedObjects = new EditorObjectMap();
	
	// Stored list of all selected Objects
	protected ref EditorObjectMap					m_SelectedObjects = new EditorObjectMap();
	
	// Stored list of all Placed Objects, indexed by their WorldObject ID
	protected ref EditorObjectMap					m_WorldObjectIndex = new EditorObjectMap();
		
	// Stored list of all Hidden Objects, indexed by their WorldObject ID
	protected ref EditorDeletedObjectMap 			m_DeletedObjects = new EditorDeletedObjectMap();
	
	protected ref EditorDeletedObjectMap			m_SelectedDeletedObjects = new EditorDeletedObjectMap();
	
	protected ref array<ref EditorPlaceableItem> m_AllPlaceableItems = {};
	
	protected ref map<int, ref array<EditorPlaceableItem>> m_PlaceableItems = new map<int, ref array<EditorPlaceableItem>>();
			
	protected ref map<string, EditorPlaceableItem>	m_PlaceableObjectsByType = new map<string, EditorPlaceableItem>();
	
	// lookup table by p3d
	protected ref map<string, ref array<EditorPlaceableItem>>	m_PlaceableObjectsByP3d = new map<string, ref array<EditorPlaceableItem>>();
	
	// Current Selected PlaceableListItem
	EditorPlaceableItem CurrentSelectedItem;
	
	void EditorObjectManagerModule()
	{
		// Loads placeable objects	
		g_Game.ReportProgress("Loading Placeable Objects");
		
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
				
		// handle config objects
		foreach (string path: config_paths) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type;
		        GetGame().ConfigGetChildName(path, i, type);
				if (GetGame().ConfigGetInt(path + " " + type + " scope") < 1) {
					continue;
				}
				
				m_AllPlaceableItems.Insert(new EditorConfigPlaceableItem(path, type));
		    }
		}
		
		array<string> paths = { "DZ/plants", "DZ/plants_bliss", "DZ/rocks", "DZ/rocks_bliss" };
		foreach (string model_path: paths) {
			array<ref CF_File> files = {};
			if (!CF_Directory.GetFiles(model_path + "/*", files, FindFileFlags.ARCHIVES)) {
				continue;
			}
				
			foreach (CF_File file: files) {		
				if (!file || file.GetExtension() != ".p3d") {
					continue;
				}
				
				m_AllPlaceableItems.Insert(new EditorStaticPlaceableItem(file.GetFullPath()));
			}
		}
	
		// Statics that belong to Editor / DF
		m_AllPlaceableItems.Insert(new EditorScriptedPlaceableItem(NetworkSpotLight));
		m_AllPlaceableItems.Insert(new EditorScriptedPlaceableItem(NetworkPointLight));
		m_AllPlaceableItems.Insert(new EditorScriptedPlaceableItem(NetworkParticleBase));
			
		foreach (EditorPlaceableItem placeable_item: m_AllPlaceableItems) {
			if (!m_PlaceableItems[placeable_item.GetCategory()]) {
				m_PlaceableItems[placeable_item.GetCategory()] = {};
			}
			
			if (!m_PlaceableObjectsByP3d[placeable_item.GetModel()]) {
				m_PlaceableObjectsByP3d[placeable_item.GetModel()] = {};
			}
			
			m_PlaceableItems[placeable_item.GetCategory()].Insert(placeable_item);
			m_PlaceableObjectsByP3d[placeable_item.GetModel()].Insert(placeable_item);
			m_PlaceableObjectsByType[placeable_item.GetName()] = placeable_item;
		}
	}

	static bool IsForbiddenItem(string model)
	{
		//! In theory should be safe but just in case
		if (model.Contains("Fx")) return true;
		if (model == "ItemOptics") return true;

		//! Cursed items
		if (model == "AKM_TESTBED") return true;
		if (model == "Red9") return true;
		if (model == "QuickieBow") return true;
		if (model == "LargeTentBackPack") return true;
		if (model == "SurvivorMale_Base" || model == "SurvivorFemale_Base") return true;
		if (model == "Land_VASICore" || model == "FlagCarrierCore") return true;
		if (GetGame().IsKindOf(model, "GP25Base")) return true;
		if (GetGame().IsKindOf(model, "M203Base")) return true;
		if (model == "ItemOptics_Base") return true;
		
		//! Everything is fine... I hope... :pain:
		return false;
	}
	
	static void RecursiveGetFiles(string directory, inout array<ref CF_File> files, string pattern = "*")
	{		
		array<ref CF_File> directories = {};
		// first get all directories and recurse them
		if (CF_Directory.GetFiles(directory + "*", directories, FindFileFlags.ARCHIVES)) {
			foreach (CF_File subdirectory: directories) {
				if (subdirectory.IsDirectory()) {
					RecursiveGetFiles(subdirectory.GetFullPath() + "/", files, pattern);
				}
			}
		}
		
		CF_Directory.GetFiles(directory + pattern, files, FindFileFlags.ARCHIVES);
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
		
		GetEditor().Statistics.EditorPlacedObjects++;
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
		
		GetEditor().Statistics.EditorRemovedObjects++;
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
		GetEditor().Statistics.Save();
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
		return m_AllPlaceableItems;
	}
	
	// return a list of objects that use this p3d, useful for finding adequite replacements for 
	// otherwise unplaceable objects
	array<EditorPlaceableItem> GetReplaceableObjects(string p3d)
	{
		return m_PlaceableObjectsByP3d[p3d];
	}

	map<int, ref array<EditorPlaceableItem>> GetPlaceableItemsByCategory()
	{
		return m_PlaceableItems;
	}
	
	void Debug()
	{
		EditorLog.Debug("EditorObjectManager Debug Info");
		EditorLog.Debug(m_EditorObjectRefs.Count().ToString());
		EditorLog.Debug(m_EditorDeletedObjectRefs.Count().ToString());
	}
}