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
	ref EditorObjectMap m_WorldObjectIndex = new EditorObjectMap();

	// Stored list of all Hidden Objects, indexed by their WorldObject ID
	protected ref EditorDeletedObjectMap m_DeletedObjects = new EditorDeletedObjectMap();

	protected ref EditorDeletedObjectMap m_SelectedDeletedObjects = new EditorDeletedObjectMap();
	
	protected ref array<ref EditorPlaceableItem> m_PlaceableObjects = {};

	protected ref map<string, EditorPlaceableItem> m_PlaceableObjectsByType = new map<string, EditorPlaceableItem>;

	protected ref array<EditorCameraTrack> m_CameraTracks = {};

	protected ref array<EditorObject> m_SelectedObjectsOrdered = {};

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
				// I dont think ShowScopeZeroObjects is really useful anymore. 
				if (scope == 0/* && !GetEditor().GetSettings().ShowScopeZeroObjects*/) {
					continue;
				}
				
				string model = SystemPath.Format(GetGame().ConfigGetTextOut(string.Format("%1 %2 model", path, type)));
				model.ToLower();
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
					placeable_item.ConsoleFriendly = 1;	
				}

				// Yikes
				if (GetGame().IsKindOf(type, "Inventory_Base") || GetGame().IsKindOf(type, "Weapon_Base") || GetGame().IsKindOf(type, "DZ_LightAI") || GetGame().IsKindOf(type, "Magazine_Base")) {
					placeable_item.Scope = 1;
					placeable_item.ConsoleFriendly = 1;
				}
					
				// bldr_ check is a hack but I cannot easily check the folder a config is defined in. not sure the best way to go about this.
				if (model.Contains("dz/") && !type.Contains("bldr_")) {
					placeable_item.ConsoleFriendly = 1;
				}

				// Register as placeable
				m_PlaceableObjects.Insert(placeable_item);

				// Register placeable type
				m_PlaceableObjectsByType[placeable_item.Type] = placeable_item;
				
				// If our model exists we need to dig a little deeper
				if (model && model != "bmp" && !IsForbiddenModel(model)) {
					string model_file = File.GetName(model);

					// register into placeable p3d models
					if (!m_PlaceableObjectsByP3dPath[model]) {
						m_PlaceableObjectsByP3dPath[model] = {};
					}
					
					if (!m_PlaceableObjectsByP3dFile[model_file]) {
						m_PlaceableObjectsByP3dFile[model_file] = {};
					} else continue; // quite humorously this fixes duplication bugs. 
								
					// dont add p3ds of AI models. crashes		
					if (GetGame().IsKindOf(placeable_item.Type, "DZ_LightAI")) {
						continue;
					}
					
					m_PlaceableObjectsByP3dPath[model].Insert(placeable_item);
					m_PlaceableObjectsByP3dFile[model_file].Insert(placeable_item);
					
					// Add static variant of all config items
					EditorPlaceableItem placeable_item_static_variant = EditorPlaceableItem.Create(SystemPath.Format(model));
					if (ValidatePath(model)) {
						placeable_item_static_variant.ConsoleFriendly = 1;
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
			foreach (string p3d_file_unformat: p3d_files) {
				string p3d_file = p3d_file_unformat;
				p3d_file.ToLower();	
				string p3d_file_name = File.GetName(p3d_file);
				
				EditorPlaceableItem placeable_item_p3d = EditorPlaceableItem.Create(p3d_file);
				m_PlaceableObjects.Insert(placeable_item_p3d);

				if (!m_PlaceableObjectsByP3dPath[p3d_file]) {
					m_PlaceableObjectsByP3dPath[p3d_file] = {};
				}
				
				if (!m_PlaceableObjectsByP3dFile[p3d_file_name]) {
					m_PlaceableObjectsByP3dFile[p3d_file_name] = {};
				}
				
				if (ValidatePath(p3d_file_unformat)) {
					placeable_item_p3d.ConsoleFriendly = 1;
				}

				m_PlaceableObjectsByP3dPath[p3d_file].Insert(placeable_item_p3d);
				m_PlaceableObjectsByP3dFile[p3d_file_name].Insert(placeable_item_p3d);
			}
		}

		// Statics that belong to Editor / DF
		m_PlaceableObjects.Insert(EditorPlaceableItem.Create(NetworkSpotLight, false));
		m_PlaceableObjects.Insert(EditorPlaceableItem.Create(NetworkPointLight, false));
		m_PlaceableObjects.Insert(EditorPlaceableItem.Create(NetworkParticleBase, false));
		
		foreach (auto instance: RegisterScriptedEntity.Instances) {
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(instance.param1, false));
		}

		//	Experimental lights
		if (GetEditor().GetSettings().UseExperimentalLights) {
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(AnniversaryMainLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(BlowtorchLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(BonfireLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(BrightnessTestLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(ChemlightLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(EntranceLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(EntranceLightStairs1));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(EntranceLightStairs2));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(EntranceLightMain1));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(EntranceLightMain2));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(FireplaceLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(FlareLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(FlareLightRed));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(FlareLightGreen));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(FlareLightBlue));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(FlareLightContamination));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(GoatLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(MuzzleFlashLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(MuzzleFlashLight_1));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(MuzzleFlashLight_2));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(PartyLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(PersonalLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(PortableGasLampLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(RoadflareLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(StoveLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(TorchLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(WarheadStorageLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(XmasSleighLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(XmasTreeLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(ZombieMummyLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(CivilianSedanFrontLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(CivilianSedanRearLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Hatchback_02FrontLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Hatchback_02RearLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Offroad_02FrontLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Offroad_02RearLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(OffroadHatchbackFrontLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(OffroadHatchbackRearLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Sedan_02FrontLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Sedan_02RearLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Truck_01FrontLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(Truck_01RearLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(FlashlightLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(HeadtorchLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(PistollightLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(SpotlightLight));
			m_PlaceableObjects.Insert(EditorPlaceableItem.Create(UniversallightLight));
		}
	}

	// For console
	static const ref array<string> VALID_PATHS = {
		"DZ\\plants","DZ\\plants_bliss", "DZ\\plants_sakhal",
		"DZ\\rocks", "DZ\\rocks_bliss", "DZ\\rocks_sakhal",
		"DZ/plants","DZ/plants_bliss", "DZ/plants_sakhal",
		"DZ/rocks", "DZ/rocks_bliss", "DZ/rocks_sakhal",
	};
		
	static bool ValidatePath(string path)
	{		
		foreach (string p: VALID_PATHS)
		{
			if (path.Contains(p))
				return true;
		}
		
		return false;
	}

	EditorCameraTrack CreateCameraTrack(notnull EditorCameraTrackData camera_track_data)
	{
		EditorCameraTrack camera_track = new EditorCameraTrack(camera_track_data);

		// strong ref
		m_EditorObjectRefs[camera_track.GetID()] = camera_track;

		// weak ref
		m_CameraTracks.Insert(camera_track);
		m_WorldObjectIndex.Insert(camera_track.GetWorldObject().GetID(), camera_track);	
		
		EditorEvents.ObjectCreated(this, camera_track);

		GetEditor().GetStatistics().EditorPlacedCameraTracks++;
		return camera_track;
	}

	bool DeleteCameraTrack(EditorCameraTrack camera_track)
	{
		if (!camera_track) {
			return false;
		}

		m_CameraTracks.RemoveItem(camera_track);

		m_SelectedObjects.RemoveEditorObject(camera_track);
		m_SelectedObjectsOrdered.RemoveItem(camera_track);
		EditorEvents.ObjectDeleted(this, camera_track);

		// remove strong ref
		m_EditorObjectRefs.Remove(camera_track.GetID());
		return true;
	}
	
	int GetCameraTrackIndex(notnull EditorCameraTrack camera_track)
	{
		return m_CameraTracks.Find(camera_track);
	}

	void SetCameraTrackIndex(notnull EditorCameraTrack camera_track, int index)
	{
		int old_index = m_CameraTracks.Find(camera_track);
		if (old_index == index || old_index == -1) {
			return;
		}

		m_CameraTracks.Remove(old_index);
		m_CameraTracks.InsertAt(camera_track, index);

		// Updates list index for menu elements
		camera_track.SetListIndex(index);
	}
	
	array<EditorCameraTrack> GetCameraTracks()
	{
		return m_CameraTracks;
	}
		
	EditorObject CreateObject(notnull EditorObjectData editor_object_data)
	{
		EditorObject editor_object = new EditorObject(editor_object_data);
				
		// strong ref
		m_EditorObjectRefs[editor_object.GetID()] = editor_object;

		m_PlacedObjects.InsertEditorObject(editor_object);
		
		EditorEvents.ObjectCreated(this, editor_object);		
		return editor_object;
	}

	void DeleteObject(notnull EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeleteObject");
				
		EditorCameraTrack camera_track = EditorCameraTrack.Cast(target);
		if (camera_track) {
			m_CameraTracks.RemoveItem(camera_track);
		} else {
			m_PlacedObjects.RemoveEditorObject(target);
		}

		m_SelectedObjects.RemoveEditorObject(target);
		m_SelectedObjectsOrdered.RemoveItem(target);
		EditorEvents.ObjectDeleted(this, target);

		// remove strong ref
		m_EditorObjectRefs.Remove(target.GetID());
	}
		
	// Call to select an object
	void SelectObject(notnull EditorObject target)
	{
		if (target.IsSelected() || target.IsLocked()) {
			return;
		}
		
		EditorLog.Trace("EditorObjectManager::SelectObject");
		m_SelectedObjects.InsertEditorObject(target);
		m_SelectedObjectsOrdered.Insert(target);
		EditorEvents.ObjectSelected(this, target);
		target.OnSelected();
		
		float count_flt = m_SelectedObjects.Count();
		vector diff = target.GetPosition() - m_AveragePositionOfSelection;
		diff[0] = diff[0] / count_flt;
		diff[1] = diff[1] / count_flt;
		diff[2] = diff[2] / count_flt;
		m_AveragePositionOfSelection = m_AveragePositionOfSelection + diff;

		target.OnUpdated.Insert(RecalculateCenterOfSelectedObjects);
		// todo perhaps propagate selections to the children of the object?
	}

	// Call to deselect an object
	void DeselectObject(notnull EditorObject target)
	{
		EditorLog.Trace("EditorObjectManager::DeselectObject");
		m_SelectedObjects.RemoveEditorObject(target);
		m_SelectedObjectsOrdered.RemoveItem(target);
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

		target.OnUpdated.Remove(RecalculateCenterOfSelectedObjects);
	}

	// When a selected object gets updated we must re-cacluate the average position
	void RecalculateCenterOfSelectedObjects()
	{
		if (m_SelectedObjects.Count() == 0) {
			m_AveragePositionOfSelection = vector.Zero;
			return;
		}
		
		float count_flt = m_SelectedObjects.Count();		
		vector total_position = vector.Zero;
		foreach (EditorObject selected_object: m_SelectedObjects) {
			total_position = total_position + selected_object.GetPosition();
		}

		total_position[0] = total_position[0] / count_flt;
		total_position[1] = total_position[1] / count_flt;
		total_position[2] = total_position[2] / count_flt;
		m_AveragePositionOfSelection = total_position;
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
		m_SelectedObjectsOrdered.Clear();
		m_DeletedObjects.ClearSafe();
		m_SelectedDeletedObjects.ClearSafe();
		m_CameraTracks.Clear();

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

	// sometimes you just dont feel like refactoring
	array<EditorObject> GetSelectedObjectsOrdered()
	{
		return m_SelectedObjectsOrdered;
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
	
	static bool IsForbiddenModel(string model)
	{
		if (model.Contains("bodyparts_above0")) return true;
		
		return false;
	}
	
	vector GetAveragePositionOfSelection()
	{
		return m_AveragePositionOfSelection;
	}
}