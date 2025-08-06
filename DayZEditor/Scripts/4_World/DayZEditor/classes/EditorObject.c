class EditorObject: EditorWorldObject
{
	static ref map<Object, EditorObject> s_AllByObject = new map<Object, EditorObject>();

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
			
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	ref ScriptInvoker OnUpdated = new ScriptInvoker();
	ref ScriptInvoker OnChanged = new ScriptInvoker();

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
	
	override Object GetWorldObject() 
	{		
		return m_WorldObject;
	}
	
	void EditorObject(notnull EditorObjectData data)
	{
		EditorLog.Trace("EditorObject " + data);
		m_Data = data;
		
		if (!m_Data.WorldObject) {
			m_WorldObject = CreateObject(m_Data.Type, m_Data.Position, m_Data.Orientation, m_Data.Scale);
			m_Data.WorldObject = m_WorldObject;

			EntityAI entity = EntityAI.Cast(m_WorldObject);
			if (entity) {
				foreach (int slot_id, EditorObjectData attachment: m_Data.AttachmentMap) {
					entity.GetInventory().CreateAttachmentEx(attachment.Type, slot_id);
				}

				// After we've spawned everything in teh dze file, go back and grab stuff that mightve been spawned by OnDebugSpawn

				array<EntityAI> entities = {};
				entity.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
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
		
		m_WorldObject = m_Data.WorldObject;
				
		// Trash the object because its uncreatable
		if (!m_WorldObject) { 
			EditorLog.Warning("Object failed to create: %1", m_Data.Type);
			return;
		}

		if (!s_AllByObject) {
			s_AllByObject = new map<Object, EditorObject>();
		}

		s_AllByObject[m_WorldObject] = this;
		
		if (GetEditor()) {
			GetEditor().GetSessionCache().Insert(m_Data.GetID(), m_Data);
		}
				
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
		
		// Bounding Box
		EnableBoundingBox(IsBoundingBoxEnabled());

		// Map marker
		EnableMapMarker(IsMapMarkerEnabled());

		// World marker
		EnableObjectMarker(IsWorldMarkerEnabled());

		// Browser item
		EnableListItem(IsListItemEnabled());
		
		// This is deliberately split due to issues with null errors, but i have to assign
		// Locked higher up or it gets set to 0 always. this is a mess, please fix
		//PropertyChanged("Locked");
		//PropertyChanged("EditorOnly");
		//PropertyChanged("AllowDamage");
		
		// Load animations
		array<string> paths = {
			CFG_VEHICLESPATH,
			CFG_WEAPONSPATH
		};
		
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
		
		Update();
		
#ifdef DIAG_DEVELOPER
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(OnFrame);
#endif
	}
		
	void ~EditorObject()
	{
		EditorLog.Trace("~EditorObject");
		if (m_Data && m_WorldObject) {
			Update();
		}

		if (s_AllByObject && m_WorldObject) {
			s_AllByObject.Remove(m_WorldObject);
		}
		
		DestroyBoundingBox();
	
		GetGame().ObjectDelete(m_WorldObject);

		delete m_EditorObjectWorldMarker; 
		delete m_EditorPlacedListItem;
		delete m_EditorObjectMapMarker;
		
		delete OnObjectSelected;
		delete OnObjectDeselected;
	}
	
	protected void OnFrame(float dt)
	{
		//vector mat[4];
		//GetTransform(mat);
		
		
		//vector min_max[2];
		//m_WorldObject.ClippingInfo(min_max);
		//Print(min_max);
		//Shape.CreateSphere(-1, ShapeFlags.ONCE | ShapeFlags.NOZWRITE, m_WorldObject.GetBoundingCenter().Multiply4(mat), min_max[1][1]);
		
		
	}
	
	// Gets full model path, starting with DZ// etc..
	string GetModelPath()
	{
		if (m_Data.Type.Contains("p3d")) {
			return m_Data.Type;
		}
		
		string model_path = GetWorldObject().ConfigGetStringRaw("model");
		if (model_path) {
			return model_path;
		}
		
		if (m_Data.Type.Contains(".p3d")) {
			/*auto placeable_replacement = GetEditor().GetObjectManager().GetReplaceableObjects(string.Format("%1.p3d", model_name));
			if (placeable_replacement[0]) {
				return placeable_replacement[0].Type;
			}*/
		}
				
		return string.Empty;
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

		EditorLog.Trace("EditorObject::OnSelected");
		m_IsSelected = true;
		ShowBoundingBox();
		OnObjectSelected.Invoke(this);
	}
	
	void OnDeselected()
	{
		if (!IsSelected()) {
			return;
		}
		
		EditorLog.Trace("EditorObject::OnDeselected");
		m_IsSelected = false;
		HideBoundingBox();
		OnObjectDeselected.Invoke(this);
	}
	
	EditorObjectData GetData() 
	{
		return m_Data;
	}
	
	bool OnMouseEnter(int x, int y)	
	{
		return true;
	}
	
	bool OnMouseLeave(int x, int y) 
	{
		return true;
	}
				
	private void ApplyTransform()
	{
	    vector rot3[3];
	    Math3D.YawPitchRollMatrix(m_Data.Orientation, rot3);

	    vector mat[4];
	    mat[0] = rot3[0] * m_Data.Scale;
	    mat[1] = rot3[1] * m_Data.Scale;
	    mat[2] = rot3[2] * m_Data.Scale;
	    mat[3] = m_Data.Position;

	    m_WorldObject.SetTransform(mat);
	    m_WorldObject.Update();
	}
	
	vector GetPosition() 
	{ 
		return m_WorldObject.GetPosition(); 
	}
	
	void SetPosition(vector pos)
	{
	    if (IsLocked()) return;

	    m_Data.Position     = pos;
	    m_Data.BottomCenter = GetBottomCenter();

	    m_WorldObject.SetPosition(pos);
	    Update();
	}
	
	vector GetOrientation()
	{
	    return m_Data.Orientation;
	}

	void SetOrientation(vector orientation)
	{
	    if (IsLocked()) return;

	    m_Data.Orientation = orientation;
	    ApplyTransform(); 
	    Update();
	}
	    
	void SetScale(float scale)
	{
	    if (IsLocked()) return;

	    m_Data.Scale = scale;
	    ApplyTransform();

	    Update();
	}

	float GetScale()
	{
		return m_Data.Scale;
	}

	void GetTransform(out vector mat[4]) 
	{ 
		m_WorldObject.GetTransform(mat); 
	}
	
	void SetTransform(vector mat[4])
	{
	    if (IsLocked()) return;

	    m_Data.Position = mat[3];

	    float len0 = mat[0].Length();
	    float len1 = mat[1].Length();
	    float len2 = mat[2].Length();
	    m_Data.Scale = (len0 + len1 + len2) / 3.0;

		float inv0 = 1.0 / len0;
		float inv1 = 1.0 / len1;
		float inv2 = 1.0 / len2;

		vector normMat[4];
		normMat[0] = mat[0] * inv0;
		normMat[1] = mat[1] * inv1;
		normMat[2] = mat[2] * inv2;
		normMat[3] = mat[3];
		m_Data.Orientation = Math3D.MatrixToAngles(normMat);
	    m_Data.BottomCenter = GetBottomCenter();

	    ApplyTransform();
	    Update();
	}

	bool IsStatic()
	{
		return m_Data.Type.Contains(".p3d");
	}
	
	void Update() 
	{ 
		if (m_WorldObject) {
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
			m_Data.Write(rpc, int.MAX);
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
		GetWorldObject().ClippingInfo(clip_info); 
	}
	
	void SetDirection(vector direction) 
	{ 
		GetWorldObject().SetDirection(direction); 
	}
	
	void AddChild(notnull IEntity child, int pivot, bool position_only = false) 
	{ 
		GetWorldObject().AddChild(child, pivot, position_only); 
	}
	
	vector GetTransformAxis(int axis) 
	{ 
		return GetWorldObject().GetTransformAxis(axis); 
	}
	
	string GetModelName() 
	{
		return GetWorldObject().GetModelName(); 
	}
	
	void EnableListItem(bool enable) 
	{
		EditorLog.Trace("EditorObject::EnableListItem");
		
		delete m_EditorPlacedListItem;
		
		if (!enable) {
			return;
		}
		
		m_EditorPlacedListItem = new EditorPlacedListItem(this);
		GetEditor().GetEditorHud().GetTemplateController().RightbarPlacedData.Insert(m_EditorPlacedListItem);
	}
	
	void EnableObjectMarker(bool enable) 
	{
		EditorLog.Trace("EditorObject::EnableObjectMarker");
		
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

	private bool _boundingBoxesCreated;
	void EnableBoundingBox(bool enable) 
	{
		EditorLog.Trace("EditorObject::EnableBoundingBox");
		DestroyBoundingBox();
		
		// Global Settings Check		
		if (!enable || !GetEditor().GetSettings().BoundingBoxSize) {
			return;
		}
		
		_boundingBoxesCreated = enable;
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
		
		vector size = GetSize();
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		
		for (int i = 0; i < 12; i++) {
			vector transform[4];			
			transform[3] = m_LineCenters[i];
			
			for (int j = 0; j < 3; j++) {
				transform[j][j] = ((position[j] == m_LineCenters[i][j]) * size[j]/2) + bounding_box_thickness;						
			}
			 
			m_BBoxLines[i] = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", m_LineCenters[i], ECE_LOCAL));
			m_BBoxLines[i].SetTransform(transform);
			
			AddChild(m_BBoxLines[i], -1);
		}
		
		
		vector y_axis_mat[4];
		vector bottom_center = GetBottomCenter() - GetPosition();
		y_axis_mat[0][0] = bounding_box_thickness;
		y_axis_mat[1][1] = 1000;
		y_axis_mat[2][2] = bounding_box_thickness;
		y_axis_mat[3] = Vector(bottom_center[0], bottom_center[1] - y_axis_mat[1][1], bottom_center[2]);
		
		//m_CenterLine = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", bottom_center, ECE_NONE));
		//m_CenterLine.SetTransform(y_axis_mat);
		//AddChild(m_CenterLine, -1);
		Update();
		
		HideBoundingBox();
	}
	
	void DestroyBoundingBox()
	{		
		if (m_BBoxLines) {
			for (int i = 0; i < 12; i++) {
				GetGame().ObjectDelete(m_BBoxLines[i]);
			}
		}
		
		GetGame().ObjectDelete(m_BBoxBase);		
		GetGame().ObjectDelete(m_CenterLine);	
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
			GetWorldObject().SetFlags(EntityFlags.VISIBLE, false);
		} else {
			GetWorldObject().ClearFlags(EntityFlags.VISIBLE, false);
		}
	}

	void SetAllowDamage(bool damage)
	{
		m_WorldObject.SetAllowDamage(damage);
		m_Data.AllowDamage = damage;
		OnChanged.Invoke();
	}

	void SetSimulate(bool simulate)
	{
		EntityAI entity_world_object = EntityAI.Cast(GetWorldObject());
		if (entity_world_object) {
			entity_world_object.DisableSimulation(!simulate);
			m_Data.Simulate = simulate;
			OnChanged.Invoke();
		}
	}

	void SetPhysicsEnabled(bool physics)
	{
		if (!PlayerBase.Cast(m_WorldObject)) {
			if (m_WorldObject) {
				if (physics) {
					m_WorldObject.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
					m_WorldObject.SetDynamicPhysicsLifeTime(-1);
					dBodySetMass(m_WorldObject, 100);
				} else {
					m_WorldObject.SetDynamicPhysicsLifeTime(0.001);
				}
			}

			//m_Data.Physics = physics;
			OnChanged.Invoke();
		}
	}
	
	void SetHealth(float health)
	{
		m_WorldObject.SetHealth("GlobalHealth", "Health", health);
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
		m_WorldObject.GetTransform(transform);
		return (Vector(0, -m_BoundingCenter[1], 0)).Multiply4(transform);
	}
	
	void GetBottomTransform(out vector transform[4])
	{
		vector mat[4];
		m_WorldObject.GetTransform(mat);
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
	
	// Depreciated
	vector GetTopCenter()
	{		
		vector top_transform[4];
		GetTopTransform(top_transform);
		return top_transform[3];
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
		EditorLog.Trace("EditorObject::ShowBoundingBox");
		
		// Global Settings Check
		if (!GetEditor().GetSettings().BoundingBoxSize) return;
		
		if (!(GetData().Flags & EditorObjectFlags.BBOX)) return;
		
		// quick and dirty bugfix
		if (!_boundingBoxesCreated) {
			EnableBoundingBox(true);
		}
		
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
		}
	}
	
	void HideBoundingBox()
	{
		EditorLog.Trace("EditorObject::HideBoundingBox");
		
		for (int i = 0; i < 12; i++) {
			if (m_BBoxLines[i]) {
				m_BBoxLines[i].ClearFlags(EntityFlags.VISIBLE, false);
			}
		}
		
		if (m_BBoxBase) {
			m_BBoxBase.ClearFlags(EntityFlags.VISIBLE, false);
		}
		
		if (m_CenterLine) {
			m_CenterLine.ClearFlags(EntityFlags.VISIBLE, false);
		}
	}
	
	bool SetAnimation(string anim_name)
	{
		EditorLog.Trace("EditorObject::SetAnimation");
		if (m_WorldObject.IsMan()) {
			//DayZPlayerImplement.Cast(m_WorldObject).EditorAnimationStart(anim_name);
			return true;
		}
		
		return false;
	}
	
	void ResetAnimation()
	{
		EditorLog.Trace("EditorObject::SetAnimation");
		if (m_WorldObject.IsMan()) {
			//DayZPlayerImplement.Cast(GetWorldObject()).EditorAnimationReset();
		}
	}
	
	void PauseSimulation(bool pause)
	{
		EditorLog.Trace("EditorObject::PauseSimulation");
		
		EntityAI ent;
		if (Class.CastTo(ent, GetWorldObject())) {
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
		if (!ItemBase.Cast(m_WorldObject) || !ItemBase.Cast(m_WorldObject).GetHierarchyParent()) { // adding this because of the notnull check in GetEditorObject
			return null;
		}
		
		return GetEditor().GetEditorObject(ItemBase.Cast(m_WorldObject).GetHierarchyParent());
	}
	
	bool IsAttachedToObject()
	{
		return (GetAttachmentParent() != null);
	}
	
	bool HasObjectAttachments()
	{
		return (ItemBase.Cast(m_WorldObject) && ItemBase.Cast(m_WorldObject).GetInventory().AttachmentCount() > 0);
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
		ItemBase item = ItemBase.Cast(m_WorldObject);
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
}