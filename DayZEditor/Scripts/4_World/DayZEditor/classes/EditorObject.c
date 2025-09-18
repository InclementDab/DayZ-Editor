class EditorObject: EditorWorldObject
{
	static ref map<Object, EditorObject> s_AllByObject = new map<Object, EditorObject>();

	protected EntityAI m_WorldEntity;
	
	protected ref EditorObjectData 			m_Data;
	protected ref EditorObjectMapMarker		m_EditorObjectMapMarker;
	protected ref EditorObjectWorldMarker	m_EditorObjectWorldMarker;
	protected ref EditorPlacedListItem 		m_EditorPlacedListItem;
	
	protected Object		m_BBoxLines[12];	
	protected Object 		m_BBoxBase;
	protected Object 		m_CenterLine;
	protected Object		m_BasePoint;
	protected vector m_VectorBasePoint;
	
	protected ref map<string, ref EditorObjectAnimationSource> m_ObjectAnimations = new map<string, ref EditorObjectAnimationSource>();
	
	protected vector m_LineCenters[12]; 
	protected vector m_LineVerticies[8];
	protected vector m_BoundingCenter;
	protected bool m_IsSelected;
	
	string Uuid;
	bool IsBeingDragged;
			
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	ref ScriptInvoker OnUpdated = new ScriptInvoker();
	ref ScriptInvoker OnChanged = new ScriptInvoker();
	
	protected int m_LowBits, m_HighBits;
		
	void EditorObject(notnull EditorObjectData data)
	{
		m_Data = data;
		
		m_LowBits = m_Data.m_LowBits;
		m_HighBits = m_Data.m_HighBits;
		
		if (m_Data.WorldObject) {
			SetWorldObject(m_Data.WorldObject);
		} else {
			if (m_Data.m_LowBits == 0 && m_Data.m_HighBits == 0) {
				SetWorldObject(CreateObject(m_Data.Type, m_Data.Position, m_Data.Orientation, m_Data.Scale));
				
				EntityAI entity2 = EntityAI.Cast(GetWorldObject());
				if (entity2) {
					foreach (int slot_id, EditorObjectData attachment: m_Data.AttachmentMap) {
						entity2.GetInventory().CreateAttachmentEx(attachment.Type, slot_id);
					}
	
					// After we've spawned everything in teh dze file, go back and grab stuff that mightve been spawned by OnDebugSpawn
	
					array<EntityAI> entities = {};
					entity2.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
					foreach (EntityAI existing_entity: entities) {
						InventoryLocation il_loc = new InventoryLocation();
						existing_entity.GetInventory().GetCurrentInventoryLocation(il_loc);
						int slot_id2 = il_loc.GetSlot();
						if (!m_Data.AttachmentMap[slot_id2]) {
							m_Data.AttachmentMap[slot_id2] = EditorObjectData.Create(existing_entity);
						}
					}
				}
			}
		}
						
		// Trash the object because its uncreatable
		/*
		if (!GetWorldObject()) { 
			EditorLog.Warning("Object failed to create: %1", m_Data.Type);
			return;
		}*/

		if (!s_AllByObject) {
			s_AllByObject = new map<Object, EditorObject>();
		}

		Object world_object = GetWorldObject();
		s_AllByObject[world_object] = this;
		
		if (GetEditor()) {
			GetEditor().GetSessionCache().Insert(m_Data.GetID(), m_Data);
		}
		
		// Browser item
		EnableListItem(IsListItemEnabled());
				
		// Load animations
		array<string> paths = {
			CFG_VEHICLESPATH,
			CFG_WEAPONSPATH
		};
		
		EntityAI entity = EntityAI.Cast(m_WorldObject);
		foreach (string path: paths) {
			string config_path = path + " " + GetType() + " AnimationSources";
			if (GetGame().ConfigIsExisting(config_path) && entity) {
				for (int j = 0; j < GetGame().ConfigGetChildrenCount(config_path); j++) {
					string child_name;
					GetGame().ConfigGetChildName(config_path, j, child_name);
					m_ObjectAnimations[child_name] = new EditorObjectAnimationSource(entity, child_name, path);
				}
			}	
		}		
		
		if (GetGame().IsMultiplayer()) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(OnFrame);
		}
	}
		
	void ~EditorObject()
	{
		EditorLog.Trace("~EditorObject");
		if (m_Data && GetWorldObject()) {
			Update();
		}

		if (s_AllByObject && GetWorldObject()) {
			s_AllByObject.Remove(GetWorldObject());
		}
		
		HideBoundingBox();

		delete m_EditorObjectWorldMarker; 
		delete m_EditorPlacedListItem;
		delete m_EditorObjectMapMarker;
		
		delete OnObjectSelected;
		delete OnObjectDeselected;
	}
	
	void SetDisplayName(string display_name) 
	{
		m_Data.DisplayName = display_name;
		m_EditorPlacedListItem.GetTemplateController().Label = m_Data.DisplayName;
		m_EditorPlacedListItem.GetTemplateController().NotifyPropertyChanged("Label");
	}
	
	string GetDisplayName() 
	{
		return m_Data.DisplayName; 
	}
	
	string GetType() 
	{
		return m_Data.Type; 
	}
	
	int GetID() 
	{
		return m_Data.GetID(); 
	}

	EditorObjectFlags GetFlags() 
	{
		return m_Data.Flags;
	}
	
	void SetWorldObjectNetworkId(int low, int high)
	{
		m_LowBits = low;
		m_HighBits = high;
	}
	
	override void SetWorldObject(Object object)
	{		
		super.SetWorldObject(object);
		
		if (m_Data) {
			m_Data.WorldObject = m_WorldObject;
		}
		
		m_WorldEntity = EntityAI.Cast(object);
		
		vector clip_info[2];
		ClippingInfo(clip_info);
		
		m_LineVerticies[0] = clip_info[0];
		m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		m_LineVerticies[4] = Vector(clip_info[1][0], clip_info[1][1], clip_info[0][2]);
		m_LineVerticies[5] = clip_info[1];
		m_LineVerticies[6] = Vector(clip_info[0][0], clip_info[1][1], clip_info[1][2]);
		m_LineVerticies[7] = Vector(clip_info[0][0], clip_info[1][1], clip_info[0][2]);
				
		m_LineCenters[0] = AverageVectors(m_LineVerticies[0], m_LineVerticies[1]);
		m_LineCenters[1] = AverageVectors(m_LineVerticies[0], m_LineVerticies[3]);
		m_LineCenters[2] = AverageVectors(m_LineVerticies[0], m_LineVerticies[7]);
		m_LineCenters[3] = AverageVectors(m_LineVerticies[4], m_LineVerticies[7]);
		m_LineCenters[4] = AverageVectors(m_LineVerticies[6], m_LineVerticies[7]);
		
		m_LineCenters[5] = AverageVectors(m_LineVerticies[1], m_LineVerticies[2]);
		m_LineCenters[6] = AverageVectors(m_LineVerticies[1], m_LineVerticies[6]);
		m_LineCenters[7] = AverageVectors(m_LineVerticies[3], m_LineVerticies[2]);
		m_LineCenters[8] = AverageVectors(m_LineVerticies[3], m_LineVerticies[4]);
		
		m_LineCenters[9] = AverageVectors(m_LineVerticies[5], m_LineVerticies[2]);
		m_LineCenters[10] = AverageVectors(m_LineVerticies[5], m_LineVerticies[4]);		
		m_LineCenters[11] = AverageVectors(m_LineVerticies[5], m_LineVerticies[6]);
		
		vector base_point = AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3]));
		m_VectorBasePoint = base_point;
		m_BoundingCenter = m_WorldObject.GetBoundingCenter();
		
		ShowBoundingBox();
		
		// Map marker
		EnableMapMarker(IsMapMarkerEnabled());

		// World marker
		EnableObjectMarker(IsWorldMarkerEnabled());
		
		Update();
		
		SerializedBuilding serialized_building = SerializedBuilding.Cast(m_WorldObject);
		if (serialized_building) {
			serialized_building.Read(m_Data.Parameters);
		}
		
		GetEditor().GetObjectManager().m_WorldObjectIndex.Insert(m_WorldObject.GetID(), this);
	}
	
	protected void OnFrame(float dt)
	{
		if (!m_Data.WorldObject || !m_WorldObject) {
			// Trolly for the world object every frame to see if we've gotten into its network bubble
			Object world_object_found = GetGame().GetObjectByNetworkId(m_LowBits, m_HighBits);
			
			if (world_object_found) {
				SetWorldObject(world_object_found);
			}
		}
	}
		
	// Gets full model path, starting with DZ// etc..
	string GetModelPath()
	{
		if (!m_WorldObject) {
			return string.Empty;
		}
		
		string model_name = m_WorldObject.GetShapeName();
		return Directory.GetDirectory(model_name);
	}
	
	string GetModelName()
	{
		if (!m_WorldObject) {
			return string.Empty;
		}
		
		return File.GetName(m_WorldObject.GetShapeName());
	}
	
	string GetModel()
	{
		return m_WorldObject.GetShapeName();
	}
	
	bool IsSelected() 
	{
		return m_IsSelected;
	}
	
	void OnSelected()
	{
		if (IsLocked() || IsSelected()) {
			return;
		}
		
		m_IsSelected = true;
		ShowBoundingBox();
		OnObjectSelected.Invoke(this);
	}
	
	void OnDeselected()
	{
		if (!IsSelected()) {
			return;
		}
		
		m_IsSelected = false;
		HideBoundingBox();
		OnObjectDeselected.Invoke(this);
	}
	
	EditorObjectData GetData() 
	{		
		EditorObjectData object_data = new EditorObjectData();		
		if (!m_WorldObject) {
			EditorLog.Warning(string.Format("World Object is null!"));
			return object_data;
		}
		
		object_data.Type = m_WorldObject.GetType();		
		object_data.Position = m_WorldObject.GetPosition();
		object_data.Orientation = m_WorldObject.GetOrientation();
		
		if (object_data.Type == string.Empty || GetGame().ConfigIsExisting(string.Format("CfgNonAIVehicles %1", object_data.Type))) {
			object_data.Type = m_WorldObject.GetShapeName();

			/*
			// Todo: this isnt modifying anything. this is where the offset should really occur.
			vector bounding_center = GetP3dBoundingCenter(object_data.Type);
			object_data.Position = object_data.Position + bounding_center;
			object_data.Orientation = object_data.Orientation * Math.RAD2DEG;*/
		}
		
		m_WorldObject.GetNetworkID(object_data.m_LowBits, object_data.m_HighBits);
		
		object_data.Scale = m_WorldObject.GetScale();
		object_data.AllowDamage = m_WorldObject.GetAllowDamage();
		object_data.Model = m_WorldObject.GetShapeName();
		
		// Deprecate this
		object_data.BottomCenter = GetBottomCenter();
		
		object_data.Simulate = false;
		if (m_WorldEntity) {
			object_data.Simulate = !m_WorldEntity.GetIsSimulationDisabled();
		}
		
		// Anything in here needs to either be stored on EditorObject, or found from m_WorldObject itself
		object_data.DisplayName = m_Data.DisplayName;
		object_data.EditorOnly = m_Data.EditorOnly;
		object_data.Locked = m_Data.Locked;
		object_data.Flags = m_Data.Flags;
		
		// Copy parameters
		foreach (string parameter_name, SerializableParam parameter: m_Data.Parameters) {
			object_data.Parameters[parameter_name] = parameter;
		}
		
		return object_data;
	}
	
	bool OnMouseEnter(int x, int y)	
	{
		return true;
	}
	
	bool OnMouseLeave(int x, int y) 
	{
		return true;
	}
	
	vector GetPosition() 
	{ 
		if (!m_WorldObject) {
			return vector.Zero;
		}
		
		return m_WorldObject.GetPosition(); 
	}
	
	void SetPosition(vector pos)
	{
	    if (IsLocked()) {
			return;
		}
				
		if (m_WorldObject) {
			// handle grid snapping
			if (GetEditor().GridMode) {
				float grid_size = GetEditor().GetGridSize();
				vector rot3[3];
			    Math3D.YawPitchRollMatrix(GetOrientation(), rot3);
				
				vector t = pos.InvMultiply3(rot3);			
				t = Vector(
					Math.Round(t[0] / grid_size) * grid_size, 
					Math.Round(t[1] / grid_size) * grid_size, 
					Math.Round(t[2] / grid_size) * grid_size);
				pos = t.Multiply3(rot3);
			}
			
			m_WorldObject.SetPosition(pos);
		}
	}
	
	vector GetOrientation()
	{
	    if (m_WorldObject) {
			return m_WorldObject.GetOrientation();
		}
		
		return vector.Zero;
	}

	void SetOrientation(vector orientation)
	{
	    if (IsLocked()) {
			return;
		}

		m_WorldObject.SetOrientation(orientation);
	}
	    
	void SetScale(float scale)
	{
	    if (IsLocked()) {
			return;
		}

	    m_WorldObject.SetScale(scale);
	}

	float GetScale()
	{
		if (m_WorldObject) {
			return m_WorldObject.GetScale();
		}
		
		return 0;
	}

	void GetTransform(out vector mat[4]) 
	{ 
		if (m_WorldObject) {
			m_WorldObject.GetTransform(mat); 
		}
	}
	
	void SetTransform(vector mat[4])
	{		
		if (m_WorldObject) {
			m_WorldObject.SetTransform(mat);
		}
	}

	bool IsStatic()
	{
		return m_Data.Type.Contains(".p3d");
	}
	
	void Update(bool update_world_object = true) 
	{ 				
		if (update_world_object && m_WorldObject) {
			m_WorldObject.Update(); 
		}
		
		OnUpdated.Invoke();
	}
	
	void UpdateNet()
	{
		if (GetGame().IsMultiplayer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(1);
			rpc.Write(Uuid);
			GetData().Write(rpc, int.MAX);
			rpc.Send(null, 39254, true);
		}
	}
	
	void PlaceOnSurfaceRotated(out vector trans[4], vector pos, float dx = 0, float dz = 0, float fAngle = 0, bool align = false) 
	{
		EntityAI ent;
		if (Class.CastTo(ent, GetWorldObject())) {
			ent.PlaceOnSurfaceRotated(trans, pos, dx, dz, fAngle, align); 
		}
	}
	
	void ClippingInfo(out vector clip_info[2]) 
	{ 
		vector min, max;
		if (m_WorldObject.IsItemBase()) {
			m_WorldObject.GetActionComponentMinMax(m_WorldObject.GetViewGeometryLevel(), 0, min, max);
			clip_info = { min, max };
		} else {
			m_WorldObject.ClippingInfo(clip_info); 
		}		
	}
	
	void SetDirection(vector direction) 
	{ 
		if (IsLocked()) {
			return;
		}
		
		m_WorldObject.SetDirection(direction); 
	}
		
	vector GetTransformAxis(int axis) 
	{ 
		return m_WorldObject.GetTransformAxis(axis); 
	}
		
	void EnableListItem(bool enable) 
	{		
		delete m_EditorPlacedListItem;
		
		if (!enable) {
			return;
		}
		
		m_EditorPlacedListItem = new EditorPlacedListItem(this);
		GetEditor().GetEditorHud().GetTemplateController().RightbarPlacedData.Insert(m_EditorPlacedListItem);
	}
	
	void EnableObjectMarker(bool enable) 
	{		
		if (m_EditorObjectWorldMarker)
			delete m_EditorObjectWorldMarker;
		
		if (!enable) {
			return;
		}
		
		m_EditorObjectWorldMarker = new EditorObjectWorldMarker(this);
	}
	
	void EnableMapMarker(bool enable) 
	{
		EditorLog.Trace("EditorObject::EnableMapMarker");
		if (m_EditorObjectMapMarker)
			delete m_EditorObjectMapMarker;
		
		if (!enable) {
			return;
		}
		
		m_EditorObjectMapMarker = new EditorObjectMapMarker(this);
		GetEditor().GetEditorHud().GetTemplateController().InsertMapMarker(m_EditorObjectMapMarker);
	}
		
	void Show(bool show) 
	{
		if (show) {
			m_Data.Flags &= ~EditorObjectFlags.HIDDEN;
			GetWorldObject().SetFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
			if (m_IsSelected) {
				ShowBoundingBox();
			} else {
				HideBoundingBox();
			}
			
			m_EditorObjectMapMarker.Show(true);
			m_EditorObjectWorldMarker.Show(true);
		} else {
			m_Data.Flags |= EditorObjectFlags.HIDDEN;
			GetWorldObject().ClearFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
			m_EditorObjectMapMarker.Show(false);
			m_EditorObjectWorldMarker.Show(false);
		}

		OnChanged.Invoke();
	}
	
	void ShowWorldObject(bool show) 
	{
		if (show) {
			m_WorldObject.SetFlags(EntityFlags.VISIBLE, false);
		} else {
			m_WorldObject.ClearFlags(EntityFlags.VISIBLE, false);
		}
	}

	void SetAllowDamage(bool damage)
	{
		if (IsLocked()) {
			return;
		}
		
		m_WorldObject.SetAllowDamage(damage);
		OnChanged.Invoke();
	}

	void SetSimulate(bool simulate)
	{
		if (m_WorldEntity) {
			m_WorldEntity.DisableSimulation(!simulate);
			OnChanged.Invoke();
		}
	}

	void SetPhysicsEnabled(bool physics)
	{
		if (!PlayerBase.Cast(GetWorldObject())) {
			if (GetWorldObject()) {
				if (physics) {
					GetWorldObject().CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
					GetWorldObject().SetDynamicPhysicsLifeTime(-1);
					dBodySetMass(GetWorldObject(), 100);
				} else {
					GetWorldObject().SetDynamicPhysicsLifeTime(0.001);
				}
			}

			//m_Data.Physics = physics;
			OnChanged.Invoke();
		}
	}
	
	void SetHealth(float health)
	{
		GetWorldObject().SetHealth("GlobalHealth", "Health", health);
	}
	
	float GetHealth()
	{
		if (GetWorldObject().HasDamageSystem()) {
			return GetWorldObject().GetHealth("GlobalHealth", "Health");
		}
		
		return 0;
	}
	
	void SetIsEditorOnly(bool editor_only)
	{
		m_Data.EditorOnly = editor_only;
	}

	bool IsLocked()
	{
		return m_Data.Locked;
	}

	bool IsVisible()
	{
		return !((m_Data.Flags & EditorObjectFlags.HIDDEN) == EditorObjectFlags.HIDDEN);
	}

	bool IsEditorOnly()
	{
		return m_Data.EditorOnly;
	}

	bool IsAllowDamage()
	{
		return m_Data.AllowDamage;
	}
		
	vector GetBottomCenter()
	{		
		vector transform[4];
		GetWorldObject().GetTransform(transform);
		return (Vector(0, -m_BoundingCenter[1], 0)).Multiply4(transform);
	}
	
	void GetBottomTransform(out vector transform[4])
	{
		vector mat[4];
		GetWorldObject().GetTransform(mat);
		copyarray(transform, mat);
		transform[3] = (Vector(0, -m_BoundingCenter[1],	 0)).Multiply4(mat);
	}	
	
	void SetBottomTransform(vector transform[4])	
	{
		vector pos_offset = Vector(0, m_BoundingCenter[1], 0).Multiply3(transform);
		transform[3] = transform[3] + pos_offset;
		SetTransform(transform);
	}
	
	void SetTopTransform(vector transform[4])
	{
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector pos_offset = Vector(0, clip_info[1][1], 0).Multiply3(transform);
		transform[3] = transform[3] - pos_offset;
		SetTransform(transform);
	}
	
	void GetTopTransform(out vector transform[4])
	{
		vector clip_info[2];
		ClippingInfo(clip_info);
		GetTransform(transform);
		vector pos_offset = Vector(0, clip_info[1][1], 0).Multiply3(transform);
		transform[3] = transform[3] + pos_offset;
	}
	
	float GetYDistance()
	{
		return m_BoundingCenter[1];
	}
	
	float GetAngle()
	{	
		vector orientation = GetOrientation();
		float a;
		if (orientation[1] <= -90) {
			a = -orientation[1] - 270;
		} else {
			a = 90 - orientation[1];
		}
		
		return a;
	}
			
	Param4<int, vector, vector, float> GetTransformArray() 
	{
		return new Param4<int, vector, vector, float>(GetID(), GetPosition(), GetOrientation(), GetScale());
	}

	vector GetSize()
	{
		vector result;
		vector clip_info[2];
		ClippingInfo(clip_info);
		result[0] = Math.AbsFloat(clip_info[0][0]) + Math.AbsFloat(clip_info[1][0]);
		result[1] = Math.AbsFloat(clip_info[0][1]) + Math.AbsFloat(clip_info[1][1]);
		result[2] = Math.AbsFloat(clip_info[0][2]) + Math.AbsFloat(clip_info[1][2]);
		
		return result;
	}	

	void SetBoundingBox(bool state, bool set_flags = false)
	{
		if (set_flags) {
			if (state) {
				m_Data.Flags |= EditorObjectFlags.BBOX;
			} else {
				m_Data.Flags &= ~EditorObjectFlags.BBOX;
			}
		}
		
		if (state && IsSelected()) {
			ShowBoundingBox();
		} else {
			HideBoundingBox();
		}
	}
		
	void ShowBoundingBox()
	{		
		// Global Settings Check
		if (!GetEditor().GetSettings().BoundingBoxSize) {
			return;
		}
		
		if (!(GetData().Flags & EditorObjectFlags.BBOX)) {
			return;
		}
		
		// Already showing
		if (m_BBoxLines[0]) {
			return;
		}
		
		if (!GetWorldObject()) {
			return;
		}
								
		float bounding_box_thickness = 0;
		switch (GetEditor().GetSettings().BoundingBoxSize) {
			case 1: { // small
				bounding_box_thickness = 0.008;
				break;
			}
			
			case 2: { // medium
				bounding_box_thickness = 0.016;
				break;
			}
			
			case 3: { // large
				bounding_box_thickness = 0.032;
				break;
			}
			
			case 4: { // gigantic
				bounding_box_thickness = 0.064;
				break;
			}
		}
		
		bounding_box_thickness /= 2;
		
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector min = clip_info[0];
		vector max = clip_info[1];
		
		vector position = AverageVectors(min, max);
		vector size = max - min;
		
		for (int i = 0; i < 12; i++) {
			vector transform[4];			
			transform[3] = m_LineCenters[i];
			
			for (int j = 0; j < 3; j++) {
				transform[j][j] = ((position[j] == m_LineCenters[i][j]) * size[j] / 2) + bounding_box_thickness;
			}
			 
			m_BBoxLines[i] = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", m_LineCenters[i], ECE_LOCAL));
			m_BBoxLines[i].SetTransform(transform);
			
			m_WorldObject.AddChild(m_BBoxLines[i], -1);
		}
	
		Update();
		
		/*
		for (int i = 0; i < 12; i++) {
			if (m_BBoxLines[i]) {
				m_BBoxLines[i].SetFlags(EntityFlags.VISIBLE, false);
			}
		}
		
		if (m_BBoxBase) {
			m_BBoxBase.SetFlags(EntityFlags.VISIBLE, false);
		}
		
		if (m_CenterLine) {
			m_CenterLine.SetFlags(EntityFlags.VISIBLE, false);
		}*/
	}
	
	void HideBoundingBox()
	{
		for (int i = 0; i < 12; i++) {
			if (m_BBoxLines[i]) {
				m_BBoxLines[i].Delete();
			}
		}
	}
	
	bool SetAnimation(string anim_name)
	{
		if (m_WorldObject.IsMan()) {
			//DayZPlayerImplement.Cast(m_WorldObject).EditorAnimationStart(anim_name);
			return true;
		}
		
		return false;
	}
	
	void ResetAnimation()
	{
		if (m_WorldObject.IsMan()) {
			//DayZPlayerImplement.Cast(m_WorldObject).EditorAnimationReset();
		}
	}
	
	void PauseSimulation(bool pause)
	{		
		EntityAI ent;
		if (Class.CastTo(ent, m_WorldObject)) {
			ent.DisableSimulation(pause);
		}		
	}
	
	// Returns active Marker, either World or Map marker
	// Can return null
	EditorObjectMarker GetMarker()
	{
		//EditorLog.Trace("EditorObject::GetMarker");
		
		if (g_Editor.GetEditorHud().IsMapVisible()) {
			return m_EditorObjectMapMarker;
		}
		
		return m_EditorObjectWorldMarker;
	}
	
	EditorPlacedListItem GetListItem()
	{
		//EditorLog.Trace("EditorObject::GetListItem");
		return m_EditorPlacedListItem;
	}
		
	void Lock(bool locked) 
	{
		m_Data.Locked = locked;
		
		EditorObjectMarker marker = GetMarker();
		if (marker) {
			marker.Show(!locked);
		}
				
		if (m_EditorPlacedListItem) {
			m_EditorPlacedListItem.LockedImage.Show(locked);
		}
	}
	
	bool IsBoundingBoxEnabled()
	{
		return ((m_Data.Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX);
	}
	
	bool IsMapMarkerEnabled()
	{
		return ((m_Data.Flags & EditorObjectFlags.MAPMARKER) == EditorObjectFlags.MAPMARKER);
	}
	
	bool IsWorldMarkerEnabled()
	{
		return ((m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER);
	}
	
	bool IsListItemEnabled()
	{
		return ((m_Data.Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM);
	}
	
	EditorObject GetAttachmentParent()
	{
		if (!ItemBase.Cast(GetWorldObject()) || !ItemBase.Cast(GetWorldObject()).GetHierarchyParent()) { // adding this because of the notnull check in GetEditorObject
			return null;
		}
		
		return GetEditor().GetEditorObject(ItemBase.Cast(GetWorldObject()).GetHierarchyParent());
	}
	
	bool IsAttachedToObject()
	{
		return (GetAttachmentParent() != null);
	}
	
	bool HasObjectAttachments()
	{
		return (ItemBase.Cast(GetWorldObject()) && ItemBase.Cast(GetWorldObject()).GetInventory().AttachmentCount() > 0);
	}
	
	map<string, ref EditorObjectAnimationSource> GetObjectAnimations()
	{
		return m_ObjectAnimations;
	}
	
	bool HasAnimations()
	{
		return (m_ObjectAnimations.Count() != 0);
	}
	
	EditorObjectMap GetObjectAttachments()
	{
		ItemBase item = ItemBase.Cast(GetWorldObject());
		EditorObjectMap editor_objects();
		for (int i = 0; i < item.GetInventory().AttachmentCount(); i++) {
			EntityAI attachment = item.GetInventory().GetAttachmentFromIndex(i);
			if (!attachment) {
				continue;
			}
			
			editor_objects.InsertEditorObject(GetEditor().GetEditorObject(attachment));
		}
		
		return editor_objects;
	}
	
	// pass in `this` in the context of the WorldObject
	void ExecuteCode(string script_content = string.Empty)
	{
		if (script_content == string.Empty) {
			//script_content = TestingScript;
		}
		
		if (script_content == string.Empty) {
			EditorLog.Debug("No content to execute");
			return;
		}
		
		script_content.Replace("this", "world_object");
		
		string sanitized_content;
		for (int i = 0; i < script_content.Length(); i++) {
			if (script_content[i] == "\n") {
				continue;
			}
			
			sanitized_content += script_content[i];
		}
		
		string file_name = "$saves:_.c";
		FileHandle handle = OpenFile(file_name, FileMode.WRITE);
		string file_data = "static void main(Object world_object)\n{\n" + sanitized_content + "\n}";
		FPrintln(handle, file_data);		
		
		if (handle) {
			CloseFile(handle);
		}
		
		ScriptModule script_module = ScriptModule.LoadScript(GetGame().GetMission().MissionScript, file_name, true);
		if (!script_module) {
			MessageBox.Show("Error", "Invalid Syntax in Script Editor", MessageBoxButtons.OK);
			return;
		}
		
		script_module.CallFunction(null, "main", null, m_WorldObject);
		
		DeleteFile(file_name);	
	}
	
	protected ref EditorObjectController m_Controller;
	
	EditorObjectController GetController()
	{
		if (!m_Controller) {
			m_Controller = new EditorObjectController();
		}
		
		m_Controller.Update(this);
		return m_Controller;
	}
}

class EditorObjectController: Managed
{
	protected EditorObject m_EditorObject;
	
	bool Show = true;
	string Name;
	vector Position, DeltaPosition;
	vector Orientation, DeltaOrientation;
	protected ref map<Object, vector> OriginalPositions = new map<Object, vector>();
	protected ref map<Object, vector> OriginalOrientations = new map<Object, vector>();
	float Scale = 1.0;
	
	float Health = 100;
	bool Locked;
	bool UsePhysics;
	bool AllowDamage = false;
	bool Collision = true;
	bool EditorOnly = false;
	
	string ExpansionTraderType;
	
	void Update(notnull EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		
		Show = m_EditorObject.IsVisible();
		Name = m_EditorObject.GetDisplayName();
		Position = m_EditorObject.GetPosition();
		Orientation = m_EditorObject.GetOrientation();
		Scale = m_EditorObject.GetScale();
		AllowDamage = m_EditorObject.IsAllowDamage();
		Locked = m_EditorObject.IsLocked();
		EditorOnly = m_EditorObject.IsEditorOnly();
		Health = m_EditorObject.GetHealth();
		
		// Yikes
		if (m_EditorObject.GetData().Parameters["ExpansionTraderType"]) {
			ExpansionTraderType = SerializableParam1<string>.Cast(m_EditorObject.GetData().Parameters["ExpansionTraderType"]).param1;
		}
	}
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Show": {
				m_EditorObject.Show(Show);
				break;
			}
			
			case "Position": {
				m_EditorObject.SetPosition(Position);
				m_EditorObject.Update();
				break;
			}
			
			case "Name": {
				m_EditorObject.SetDisplayName(Name);
				break;
			}
			
			case "Orientation": {
				m_EditorObject.SetOrientation(Orientation);
				m_EditorObject.SetScale(Math.Clamp(Scale, 0.0001, float.MAX));
				m_EditorObject.Update();
				break;
			}
			
			case "Scale": {
				m_EditorObject.SetScale(Math.Clamp(Scale, 0.0001, float.MAX));
				m_EditorObject.Update();
				break;
			}
			
			case "Locked": {
				m_EditorObject.Lock(Locked);
				break;
			}
			
			case "AllowDamage": {
				m_EditorObject.SetAllowDamage(AllowDamage);
				break;
			}
			
			case "EditorOnly": {
				m_EditorObject.SetIsEditorOnly(EditorOnly);
				break;
			}
			
			case "Health": {
				m_EditorObject.SetHealth(Health);
				break;
			}
		}
	}
}