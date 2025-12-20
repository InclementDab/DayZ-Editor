class EditorObject: EditorWorldObject
{
	static ref map<Object, EditorObject> s_AllByObject = new map<Object, EditorObject>();
	
	protected ref EditorObjectData 			m_Data;
	protected ref EditorObjectWorldMarker	m_EditorObjectWorldMarker;
	protected ref EditorPlacedListItem 		m_EditorPlacedListItem;
	
	protected Object		m_BBoxLines[12];	
	protected Object 		m_BBoxBase;
	protected Object 		m_CenterLine;
	protected Object		m_BasePoint;
	
	protected ref map<string, ref EditorObjectAnimationSource> m_ObjectAnimations = new map<string, ref EditorObjectAnimationSource>();
		
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
		} 
		// Instead of letting this object fall through and poll every frame, we register it with the manager.
		// The manager will handle it in a single, periodic loop.
		else if (GetGame().IsMultiplayer() && (m_LowBits != 0 || m_HighBits != 0)) 
		{
			GetEditor().GetObjectManager().RegisterUnresolvedObject(this);
		} 
		else 
		{
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
		
// #ifndef DIAG_DEVELOPER
// 		if (GetGame().IsMultiplayer()) {
// #endif
// 			GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(OnFrame);
// #ifndef DIAG_DEVELOPER
// 		}
// #endif
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
		
		delete OnObjectSelected;
		delete OnObjectDeselected;
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
				
#ifdef DIAG_DEVELOPER
		return;
		// Local space
		vector mat[4];
		GetTransform(mat);
		array<ref EditorSnapPlane> snap_points = GetSnapPlanes();
		foreach (EditorSnapPlane snap_point: snap_points) {
			vector snap_point_ws[4];
			snap_point.m_SnapPoint.GetTransformWS(snap_point_ws);
			//Shape.CreateMatrix(snap_point_ws, 4);
			
			snap_point.Debug(LinearColor.RED, ShapeFlags.ONCE);
		}
#endif
	}
		
	void SetDisplayName(string display_name) 
	{
		m_Data.DisplayName = display_name;
		if (m_EditorPlacedListItem) {
			m_EditorPlacedListItem.GetTemplateController().Label = m_Data.DisplayName;
			m_EditorPlacedListItem.GetTemplateController().NotifyPropertyChanged("Label");
		}
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
	
	int GetNetIdLow() { return m_LowBits; }
	int GetNetIdHigh() { return m_HighBits; }

	void SetWorldObjectNetworkId(int low, int high)
	{
		m_LowBits = low;
		m_HighBits = high;
	}
	
	override void SetWorldObject(Object object)
	{		
		super.SetWorldObject(object);
												
		ShowBoundingBox();
		
		// Map marker
		EnableMapMarker(IsMapMarkerEnabled());

		// World marker
		if (!IsLocked() && IsWorldMarkerEnabled()) {
			EnableObjectMarker(true);
		}
		
		Update();
		
		SerializedBuilding serialized_building = SerializedBuilding.Cast(m_WorldObject);
		if (serialized_building) {
			serialized_building.Read(m_Data.Parameters);
		}
		
		GetEditor().GetObjectManager().m_WorldObjectIndex.Insert(m_WorldObject.GetID(), this);
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
		HideBoundingBox();
		
		if (!IsSelected()) {
			return;
		}
		
		m_IsSelected = false;
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
		object_data.Position = GetPosition();

		object_data.Orientation = GetOrientation();
		object_data.Scale = GetScale();
				
		if (object_data.Type == string.Empty || GetGame().ConfigIsExisting(string.Format("CfgNonAIVehicles %1", object_data.Type))) {
			object_data.Type = m_WorldObject.GetShapeName();

			/*
			// Todo: this isnt modifying anything. this is where the offset should really occur.
			vector bounding_center = GetP3dBoundingCenter(object_data.Type);
			object_data.Position = object_data.Position + bounding_center;
			object_data.Orientation = object_data.Orientation * Math.RAD2DEG;*/
		}
		
		m_WorldObject.GetNetworkID(m_LowBits, m_HighBits);
		
		object_data.AllowDamage = m_WorldObject.GetAllowDamage();
		object_data.Model = m_WorldObject.GetShapeName();
		object_data.WorldObject = m_WorldObject;
		
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
		SerializedBuilding serial_building = SerializedBuilding.Cast(m_WorldObject);
		if (serial_building) {
			serial_building.Write(object_data.Parameters);
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
		
		float scale = GetScale();
		
		vector matrix[4];
		Math3D.YawPitchRollMatrix(GetOrientation(), matrix);
		matrix[0] = matrix[0] * scale;
		matrix[1] = matrix[1] * scale;
		matrix[2] = matrix[2] * scale;
		matrix[3] = pos;
						
		SetTransform(matrix);
	}
	
	vector GetOrientation()
	{		
		vector transform[4];
		m_WorldObject.GetTransform(transform);
		Math3D.MatrixOrthogonalize3(transform);
		return Math3D.MatrixToAngles(transform);
	}

	void SetOrientation(vector orientation)
	{
	    if (IsLocked()) {
			return;
		}
		
		float scale = GetScale();
		
		vector matrix[4];
		Math3D.YawPitchRollMatrix(orientation, matrix);
		matrix[0] = matrix[0] * scale;
		matrix[1] = matrix[1] * scale;
		matrix[2] = matrix[2] * scale;
		matrix[3] = GetPosition();
						
		SetTransform(matrix);
	}
	    
	void SetScale(float scale)
	{
	    if (IsLocked()) {
			return;
		}
		
		vector matrix[4];
		Math3D.YawPitchRollMatrix(GetOrientation(), matrix);
		matrix[0] = matrix[0] * scale;
		matrix[1] = matrix[1] * scale;
		matrix[2] = matrix[2] * scale;
		matrix[3] = GetPosition();
						
		SetTransform(matrix);
	}

	float GetScale()
	{
		if (m_WorldObject) {
			return m_WorldObject.GetScale();
		}
		
		return 0;
	}
	
	bool IsStatic()
	{
		return GetData().Type.Contains(".p3d");
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
		if (GetGame().IsMultiplayer() && GetEditor() && GetEditor().GetNetActionManager())
			GetEditor().GetNetActionManager().SendObjectUpdate(this);
	}
	
	void PlaceOnSurfaceRotated(out vector trans[4], vector pos, float dx = 0, float dz = 0, float fAngle = 0, bool align = false) 
	{
		EntityAI ent;
		if (Class.CastTo(ent, GetWorldObject())) {
			ent.PlaceOnSurfaceRotated(trans, pos, dx, dz, fAngle, align); 
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
		
		if (!enable) {
			return;
		}
		
		//GetEditor().GetEditorHud().GetTemplateController().InsertMapMarker(m_EditorObjectMapMarker);
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
			
			m_EditorObjectWorldMarker.Show(true);
		} else {
			m_Data.Flags |= EditorObjectFlags.HIDDEN;
			GetWorldObject().ClearFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
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

	protected bool m_PhysicsEnabled;
	
	void SetPhysicsEnabled(bool physics)
	{
		if (PlayerBase.Cast(m_WorldObject)) {
			return;
		}
		
		if (!m_WorldObject) {
			return;
		}
		
		m_PhysicsEnabled = physics;
		if (m_PhysicsEnabled) {
			m_WorldObject.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			m_WorldObject.SetDynamicPhysicsLifeTime(-1);
			dBodySetMass(m_WorldObject, 100);
		} else {
			m_WorldObject.SetDynamicPhysicsLifeTime(0.001);
		}
		
		OnChanged.Invoke();
	}
	
	bool IsPhysicsEnabled()
	{
		return m_PhysicsEnabled;
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
		
		int i;
		if (m_BBoxLines[0] != null) {
			for (i = 0; i < 12; i++) {
				m_BBoxLines[i].SetFlags(EntityFlags.VISIBLE, false);
			}
		} else {
			for (i = 0; i < 12; i++) {
				vector transform[4];			
				transform[3] = m_LineCenters[i];
				
				for (int j = 0; j < 3; j++) {
					transform[j][j] = ((position[j] == m_LineCenters[i][j]) * size[j] / 2) + bounding_box_thickness;
				}
				 
				m_BBoxLines[i] = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", m_LineCenters[i], ECE_LOCAL));
				m_BBoxLines[i].SetTransform(transform);
				
				m_WorldObject.AddChild(m_BBoxLines[i], -1);
			}
		}
		
		Update();
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
		return m_EditorObjectWorldMarker;
	}
	
	EditorPlacedListItem GetListItem()
	{
		return m_EditorPlacedListItem;
	}
		
	void Lock(bool locked) 
	{
		m_Data.Locked = locked;
		
		EditorObjectMarker marker = GetMarker();
		if (IsLocked()) {
			delete m_EditorObjectWorldMarker;
		} else {
			if (!m_EditorObjectWorldMarker && IsWorldMarkerEnabled()) {
				m_EditorObjectWorldMarker = new EditorObjectWorldMarker(this);
			}
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
		UsePhysics = m_EditorObject.IsPhysicsEnabled();
		
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
						
			case "Name": {
				m_EditorObject.SetDisplayName(Name);
				break;
			}
			
			case "Position": {
				m_EditorObject.SetPosition(Position);
				m_EditorObject.Update();
				break;
			}
			
			case "Orientation": {
				m_EditorObject.SetOrientation(Orientation);
				m_EditorObject.Update();
				break;
			}

			case "Scale": {
                if (Math.AbsFloat(Scale) <= Math.EPSILON * 2) return;
				m_EditorObject.SetScale(Scale);
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
			
			case "UsePhysics": {
				m_EditorObject.SetPhysicsEnabled(UsePhysics);
				break;
			}
		}
	}
}