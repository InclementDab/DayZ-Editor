class EditorObjectManagerModule : Managed
{
	static const ref array<string> VALID_CONFIG_PATHS = {
		CFG_VEHICLESPATH,
		CFG_WEAPONSPATH,
		CFG_MAGAZINESPATH
	};

	// strong reference to objects, insert and remove expectedly
	protected ref map<int, ref EditorObject> m_EditorObjectRefs = new map<int, ref EditorObject>();
	protected ref map<int, ref EditorDeletedObject> m_EditorDeletedObjectRefs = new map<int, ref EditorDeletedObject>();

	// Stored list of all Placed Objects
	protected ref EditorObjectMap m_PlacedObjects = new EditorObjectMap();

	// Stored list of all selected Objects
	protected ref EditorObjectMap m_SelectedObjects = new EditorObjectMap();

	// Stored list of all Placed Objects, indexed by their WorldObject ID
	protected ref EditorObjectMap m_WorldObjectIndex = new EditorObjectMap();

	// Stored list of all Hidden Objects, indexed by their WorldObject ID
	protected ref EditorDeletedObjectMap m_DeletedObjects = new EditorDeletedObjectMap();

	protected ref EditorDeletedObjectMap m_SelectedDeletedObjects = new EditorDeletedObjectMap();

	protected ref array<ref EditorPlaceableItem> m_PlaceableObjects = {};

	protected ref map<string, EditorPlaceableItem> m_PlaceableObjectsByType = new map<string, EditorPlaceableItem>;

	// lookup table by p3d
	protected ref map<string, ref array<EditorPlaceableItem>> m_PlaceableObjectsByP3dFile = new map<string, ref array<EditorPlaceableItem>>();
	protected ref map<string, ref array<EditorPlaceableItem>> m_PlaceableObjectsByP3dPath = new map<string, ref array<EditorPlaceableItem>>();
	protected vector m_AveragePositionOfSelection;

	// Current Selected PlaceableListItem
	EditorPlaceableItem CurrentSelectedItem;

	void EditorObjectManagerModule(Editor editor)
	{
		// handle config objects
		foreach (string path: VALID_CONFIG_PATHS) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type, type_lower;
				GetGame().ConfigGetChildName(path, i, type);
				type_lower = type;
				type_lower.ToLower();
				int scope = GetGame().ConfigGetInt(path + " " + type + " scope");
				string model = SystemPath.Format(GetGame().ConfigGetTextOut(string.Format("%1 %2 model", path, type)));
				// DayZ has a difficult time supporting leading slashes
				if (model[0] == SystemPath.SEPERATOR) {
					model = model.Substring(1, model.Length() - 1);
				}
				
				// bug with some military tents missing models
				if (model && !File.GetExtension(model)) {
					model += ".p3d";
				}
				
				if (IsForbiddenItem(type)) {
					continue;
				}

				EditorPlaceableItem placeable_item = EditorPlaceableItem.Create(path, type, scope);
				if (type_lower.Contains("land_") || type_lower.Contains("staticobj_")) {
					placeable_item.Scope = 2;
				}

				// Yikes
				if (GetGame().IsKindOf(type, "Inventory_Base") || GetGame().IsKindOf(type, "Weapon_Base") || GetGame().IsKindOf(type, "DZ_LightAI") || GetGame().IsKindOf(type, "Magazine_Base")) {
					placeable_item.Scope = 0;
				}

				// Register as placeable
				m_PlaceableObjects.Insert(placeable_item);

				// Register placeable type
				m_PlaceableObjectsByType[placeable_item.Type] = placeable_item;
				
				// If our model exists we need to dig a little deeper
				if (model && model != "bmp") {
					string model_file = File.GetName(model);

					// register into placeable p3d models
					if (!m_PlaceableObjectsByP3dPath[model]) {
						m_PlaceableObjectsByP3dPath[model] = {};
					}
					
					if (!m_PlaceableObjectsByP3dFile[model_file]) {
						m_PlaceableObjectsByP3dFile[model_file] = {};
					} else continue; // quite humorously this fixes duplication bugs. 
										
					m_PlaceableObjectsByP3dPath[model].Insert(placeable_item);
					m_PlaceableObjectsByP3dFile[model_file].Insert(placeable_item);
					
					// Add static variant of all config items
					EditorPlaceableItem placeable_item_static_variant = EditorPlaceableItem.Create(SystemPath.Format(model));
					if (!ObjectSpawnerHandler.ValidatePath(model)) {
						placeable_item_static_variant.Scope = 0;
					}
					m_PlaceableObjectsByP3dPath[model].Insert(placeable_item_static_variant);
					m_PlaceableObjectsByP3dFile[model_file].Insert(placeable_item_static_variant);
					m_PlaceableObjects.Insert(placeable_item_static_variant);
				}
			}
		}

		array<string> paths = { "DZ/plants", "DZ/plants_bliss", "DZ/plants_sakhal", "DZ/rocks", "DZ/rocks_bliss", "DZ/rocks_sakhal" };
		// handle static objects
		foreach (string model_path: paths) {
			array<string> p3d_files = Directory.EnumerateFiles(model_path, "*.p3d");
			foreach (string p3d_file: p3d_files) {		
				string p3d_file_name = File.GetName(p3d_file);
				
				EditorPlaceableItem placeable_item_p3d = EditorPlaceableItem.Create(p3d_file);
				m_PlaceableObjects.Insert(placeable_item_p3d);

				if (!m_PlaceableObjectsByP3dPath[p3d_file]) {
					m_PlaceableObjectsByP3dPath[p3d_file] = {};
				}
				
				if (!m_PlaceableObjectsByP3dFile[p3d_file_name]) {
					m_PlaceableObjectsByP3dFile[p3d_file_name] = {};
				}
				
				if (!ObjectSpawnerHandler.ValidatePath(p3d_file)) {
					placeable_item_p3d.Scope = 0;
				}

				m_PlaceableObjectsByP3dPath[p3d_file].Insert(placeable_item_p3d);
				m_PlaceableObjectsByP3dFile[p3d_file_name].Insert(placeable_item_p3d);
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
		if (!editor_object || !editor_object.GetWorldObject())
		{
			return null;
		}

		// strong ref
		m_EditorObjectRefs[editor_object.GetID()] = editor_object;

		m_PlacedObjects.InsertEditorObject(editor_object);
		m_WorldObjectIndex.Insert(editor_object.GetWorldObject().GetID(), editor_object);

		EditorEvents.ObjectCreated(this, editor_object);

		GetEditor().GetStatistics().EditorPlacedObjects++;
		return editor_object;
	}

	void DeleteObject(notnull EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeleteObject");
		
		if (target.GetFlags() & EditorObjectFlags.NODELETE) {
			return;
		}

		m_SelectedObjects.RemoveEditorObject(target);
		m_PlacedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeleted(this, target);

		// remove strong ref
		m_EditorObjectRefs.Remove(target.GetID());
	}

	// Call to select an object
	void SelectObject(notnull EditorObject target)
	{
		if (target.IsSelected() || target.Locked) {
			return;
		}
		
		EditorLog.Trace("EditorObjectManager::SelectObject");
		m_SelectedObjects.InsertEditorObject(target);
		EditorEvents.ObjectSelected(this, target);
		target.OnSelected();
		
		float count_flt = m_SelectedObjects.Count();
		vector diff = target.GetPosition() - m_AveragePositionOfSelection;
		diff[0] = diff[0] / count_flt;
		diff[1] = diff[1] / count_flt;
		diff[2] = diff[2] / count_flt;
		m_AveragePositionOfSelection = m_AveragePositionOfSelection + diff;
		// todo perhaps propagate selections to the children of the object?
	}

	// Call to deselect an object
	void DeselectObject(notnull EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeselectObject");
		m_SelectedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeselected(this, target);
		target.OnDeselected();

		if (m_SelectedObjects.Count() == 0) {
			m_AveragePositionOfSelection = vector.Zero;
		} else {
			float count_flt = m_SelectedObjects.Count();
			vector n = (m_AveragePositionOfSelection * (count_flt + 1) - target.GetPosition());
			n[0] = n[0] / count_flt;
			n[1] = n[1] / count_flt;
			n[2] = n[2] / count_flt;
			m_AveragePositionOfSelection = n;
		}
	}

	// Call to toggle selection
	void ToggleSelection(notnull EditorObject target)
	{
		if (target.IsSelected())
			DeselectObject(target);
		else
			SelectObject(target);
	}

	// Call to clear selection
	void ClearSelection()
	{
		foreach (EditorObject editor_object: m_SelectedObjects) {
			if (editor_object) {
				DeselectObject(editor_object);
			}
		}

		foreach (EditorDeletedObject deleted_object: m_SelectedDeletedObjects) {
			if (deleted_object) {
				DeselectHiddenObject(deleted_object);
			}
		}
	}

	// Hidden object stuff
	void HideMapObject(notnull EditorDeletedObject target)
	{
		// strong ref
		m_EditorDeletedObjectRefs[target.GetID()] = target;

		m_DeletedObjects.InsertEditorDeletedObject(target);

		GetEditor().GetStatistics().EditorRemovedObjects++;
	}

	void UnhideMapObject(int target)
	{
		m_EditorDeletedObjectRefs.Remove(target);
		delete m_DeletedObjects[target]; // what happens when you have refs everywhere
		m_DeletedObjects.Remove(target);
	}

	void UnhideMapObject(notnull EditorDeletedObject target)
	{
		UnhideMapObject(target.GetID());
	}

	void SelectHiddenObject(notnull EditorDeletedObject target)
	{
		m_SelectedDeletedObjects.InsertEditorDeletedObject(target);
		EditorEvents.DeletedObjectSelected(this, target);
		target.OnSelected();
	}

	void DeselectHiddenObject(notnull EditorDeletedObject target)
	{
		m_SelectedDeletedObjects.RemoveEditorDeletedObject(target);
		EditorEvents.DeletedObjectDeselected(this, target);
		target.OnDeselected();
	}

	void ToggleHiddenObjectSelection(notnull EditorDeletedObject target)
	{
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
		GetEditor().GetStatistics().Save();
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
		if (!object) {
			return true; // i mean i guess its hidden /shrug
		}
		
		return (GetDayZGame().GetSuppressedObjectManager().IsSuppressed(object));
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
	array<EditorPlaceableItem> GetReplaceableObjects(string p3d_file_name)
	{
		return m_PlaceableObjectsByP3dFile[p3d_file_name];
	}
	
	// reverses full p3d file, mostly to overcome ItemPreviewWidgets requiring EntityAI
	string ConvertP3dFileToPotentialObjectType(string p3d_file)	
	{
		array<EditorPlaceableItem> placeables = m_PlaceableObjectsByP3dPath[p3d_file];
		if (placeables && placeables.Count() > 0) {
			return placeables[0].Type;
		}

		return string.Empty;
	}

	void Debug()
	{
		EditorLog.Debug("EditorObjectManager Debug Info");
		EditorLog.Debug(m_EditorObjectRefs.Count().ToString());
		EditorLog.Debug(m_EditorDeletedObjectRefs.Count().ToString());
	}
	
	static bool IsForbiddenItem(string model)
	{
		model.ToLower();
		model.TrimInPlace();
		//! In theory should be safe but just in case
		if (model == "itemoptics") return true;
		if (model == "access") return true;
		if (model == "transport") return true;
		if (model == "all") return true;
		if (model == "land_wreck_car_twodoors") return true;
		if (model == "land_wreck_car_threedoors") return true;

		//! Cursed items
		if (model == "akm_testbed") return true;
		if (model == "red9") return true;
		if (model == "quickiebow") return true;
		if (model == "largetentbackpack") return true;
		if (model == "survivormale_base" || model == "survivorfemale_base") return true;
		if (model == "land_vasicore" || model == "flagcarriercore") return true;
		if (GetGame().IsKindOf(model, "gp25base")) return true;
		if (GetGame().IsKindOf(model, "m203base")) return true;
		if (model == "itemoptics_base") return true;

		//? Added a few more to the list
		/* 
		Give console Error: SCRIPT    (E): [WeaponStableState::ValidateMuzzleArray] :: 
		[ERROR] :: Muzzle array validation has failed. Please set up the correct muzzle states by overriding InitMuzzleArray.
		*/
		if (model == "groza") return true;
		if (model == "pm73rak") return true;
		if (model == "trumpet") return true;
		//TODO add the abstract models 

		//! Everything is fine... I hope... :pain:
		return false;
	}
	
	vector GetAveragePositionOfSelection()
	{
		return m_AveragePositionOfSelection;
	}
}