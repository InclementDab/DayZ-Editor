class EditorObject: Managed
{	
	protected Object m_WorldObject;
	protected EditorObjectData 				m_Data;
	protected ref EditorObjectMapMarker		m_EditorObjectMapMarker;
	protected ref EditorObjectWorldMarker	m_EditorObjectWorldMarker;
	protected ref EditorPlacedListItem 		m_EditorPlacedListItem;
	
	protected Object		m_BBoxLines[12];	
	protected Object 		m_BBoxBase;
	protected Object 		m_CenterLine;
	protected Object		m_BasePoint;
	
	protected ref map<string, ref EditorObjectAnimationSource> m_ObjectAnimations = new map<string, ref EditorObjectAnimationSource>();
	
	protected vector m_LineCenters[12]; 
	protected vector m_LineVerticies[8];
	protected bool m_IsSelected;
	
	// Object Data
	int ObjectID;
	string Name;
	vector Position;
	vector Orientation;
	float Scale = 1.0;
	
	// Object Properties
	float Health = 100;
	bool Show = true;
	bool Locked;
	bool Physics;
	bool Simulate = true;
	bool AllowDamage = false;
	bool Collision = true;
	bool EditorOnly = false;
	
	// Human Properties
	int CurrentAnimation;
	bool Animate;
	
	// Custom stuff
	string ExpansionTraderType;
	string TestingScript;
		
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();

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
	
	Object GetWorldObject() 
	{		
		return m_WorldObject;
	}
	
	void EditorObject(notnull EditorObjectData data)
	{
		EditorLog.Trace("EditorObject " + data);
		m_Data = data;
		
		m_WorldObject = m_Data.WorldObject;
		if (!m_WorldObject) {
			m_WorldObject = m_Data.CreateObject();
		}	
				
		// Trash the object because its uncreatable
		if (!m_WorldObject) { 
			EditorLog.Warning("Object failed to create: %1", m_Data.Type);
			return;
		}
		
		if (GetEditor()) {
			GetEditor().GetSessionCache().Insert(m_Data.GetID(), m_Data);
		}
		
		// Version 2
		EntityAI entity = EntityAI.Cast(m_WorldObject);
		if (entity) {
			foreach (string attachment: data.Attachments) {
				entity.GetInventory().CreateAttachment(attachment);
			}
		}
		
		if (data.Parameters["ExpansionTraderType"]) {
			ExpansionTraderType = SerializableParam1<string>.Cast(data.Parameters["ExpansionTraderType"]).param1;
		}
		
		// Version 3
		Locked = m_Data.Locked;
		Simulate = m_Data.Simulate;
		EditorOnly = m_Data.EditorOnly;
		AllowDamage = m_Data.AllowDamage;
		
		// If network light
		if (NetworkLightBase.Cast(m_WorldObject)) {
			NetworkLightBase.Cast(m_WorldObject).Read(m_Data.Parameters);
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
		m_BasePoint = GetGame().CreateObjectEx("BoundingBoxBase", base_point, ECE_NONE);
		m_BasePoint.SetScale(0.001);
		m_BasePoint.ClearFlags(EntityFlags.VISIBLE | EntityFlags.SOLID | EntityFlags.TOUCHTRIGGERS, true);

		AddChild(m_BasePoint, -1, true);
		
		// Bounding Box
		EnableBoundingBox(IsBoundingBoxEnabled());

		// Map marker
		EnableMapMarker(IsMapMarkerEnabled());

		// World marker
		EnableObjectMarker(IsWorldMarkerEnabled());

		// Browser item
		EnableListItem(IsListItemEnabled());

		if (m_WorldObject.HasDamageSystem()) {
			Health = m_WorldObject.GetHealth("", "Health");
		}
		
		// This is deliberately split due to issues with null errors, but i have to assign
		// Locked higher up or it gets set to 0 always. this is a mess, please fix
		PropertyChanged("Locked");
		PropertyChanged("Simulate");
		PropertyChanged("EditorOnly");
		PropertyChanged("AllowDamage");
		
		// Needed for AI Placement			
		EntityAI entity_ai;
		if (Class.CastTo(entity_ai, m_WorldObject)) {						
			// weeeeeeee
			if (GetEditor().Settings.SpawnItemsWithAttachments && (entity_ai.GetInventory().GetCargo() || entity_ai.GetInventory().GetAttachmentSlotsCount() > 0)) {
				entity_ai.OnDebugSpawn();
			}
		}	
		
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
	}
		
	void ~EditorObject()
	{
		EditorLog.Trace("~EditorObject");
		if (m_Data && m_WorldObject) {
			Update();
		}
		
		DestroyBoundingBox();
	
		GetGame().ObjectDelete(m_WorldObject);

		delete m_EditorObjectWorldMarker; 
		delete m_EditorPlacedListItem;
		delete m_EditorObjectMapMarker;
		
		delete OnObjectSelected;
		delete OnObjectDeselected;
	}
			
	bool IsSelected() 
	{
		return m_IsSelected;
	}
	
	void OnSelected()
	{
		if (Locked || IsSelected()) {
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

	vector GetPosition() 
	{ 
		return GetWorldObject().GetPosition(); 
	}
	
	void SetPosition(vector pos) 
	{ 
		if (Locked) return;
		GetWorldObject().SetPosition(pos);
		Update();
	}
	
	vector GetOrientation() { return GetWorldObject().GetOrientation(); }
	void SetOrientation(vector pos) 
	{ 
		if (Locked) return;
		GetWorldObject().SetOrientation(pos);
		GetWorldObject().SetScale(Scale);
		Update();
	}
	
	void GetTransform(out vector mat[4]) 
	{ 
		GetWorldObject().GetTransform(mat); 
	}
	
	void SetTransform(vector mat[4]) 
	{ 	
		if (Locked) return;
		GetWorldObject().SetTransform(mat); 
		Update();
	}
	
	void SetScale(float scale)
	{		
		if (Locked) return;
		GetWorldObject().SetScale(scale);
		Update();
	}
	
	float GetScale()
	{
		return GetWorldObject().GetScale();
	}
	
	void Update() 
	{ 
		if (!m_WorldObject) {
			return;
		}
		
		m_WorldObject.Update(); 
				
		ObjectID = m_WorldObject.GetID();
		if (m_Data) {
			m_Data.Position = GetPosition();
			m_Data.Orientation = GetOrientation();
			m_Data.Scale = GetScale();
			m_Data.BottomCenter = GetBottomCenter();
			
			m_Data.Locked = Locked;
			m_Data.EditorOnly = EditorOnly;
			m_Data.Simulate = Simulate;
			m_Data.AllowDamage = AllowDamage;
			
			// Update Attachments
			EntityAI entity = EntityAI.Cast(m_WorldObject);
			if (entity) {
				m_Data.Attachments.Clear();
				array<EntityAI> attachments = {};
				for (int i = 0; i < entity.GetInventory().AttachmentCount(); i++) {			
					EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);
					if (!attachment) {
						continue;
					}
					
					m_Data.Attachments.Insert(attachment.GetType());
				}
			}
			
		}
		
		Name = GetDisplayName();
		Position = GetPosition();
		Orientation = GetOrientation();
		Scale = GetScale();
	}
	
	// EditorObjects can also be psuedo-controllers
	void PropertyChanged(string property_name)
	{
		//EditorLog.Trace("EditorObject::PropertyChanged %1", property_name);
		switch (property_name) {
			case "Name": {
				SetDisplayName(Name);
				break;
			}
			
			case "Position": {
				EditorAction position_undo = new EditorAction("SetTransform", "SetTransform");
				position_undo.InsertUndoParameter(GetTransformArray());
				SetPosition(Position);
				position_undo.InsertRedoParameter(GetTransformArray());
				GetEditor().InsertAction(position_undo);
				break;
			}
			
			case "Orientation": {
				EditorAction orientation_undo = new EditorAction("SetTransform", "SetTransform");
				orientation_undo.InsertUndoParameter(GetTransformArray());
				SetOrientation(Orientation);
				orientation_undo.InsertRedoParameter(GetTransformArray());
				GetEditor().InsertAction(orientation_undo);
				break;
			}
			
			case "Scale": {
				if (Scale < 0.000001) {
					Scale = 0.000001;
				}
								
				SetScale(Scale);
				break;
			}
			
			case "Show": {
				Show(Show);
				break;
			}
			
			case "Locked": {
				Lock(Locked);
				break;
			}
			
			case "Physics": {
				EnablePhysics(Physics);
				break;
			}
						
			case "Simulate": {
				EntityAI ai = EntityAI.Cast(m_WorldObject);
				if (ai) {
					ai.DisableSimulation(!Simulate);
				}
				break;
			}
			
			case "Animate": {
				PlayerBase emote_player = PlayerBase.Cast(m_WorldObject);
				if (emote_player) {
					emote_player.GetEmoteManager().PlayEmote(CurrentAnimation);
				}
				
				break;
			}
						
			case "EditorOnly": {
				//m_Data.EditorOnly = EditorOnly;
				break;
			}
			
			case "AllowDamage": {
				m_WorldObject.SetAllowDamage(AllowDamage);
				break;
			}
			
			case "Health": {
				if (m_WorldObject.HasDamageSystem()) {
					Health = Math.Clamp(Health, 0, 100);
					m_WorldObject.SetHealth("", "Health", Health);
				}
				break;
			}
			
			case "Collision": {
				if (Collision) {
					m_WorldObject.SetFlags(EntityFlags.SOLID, true);
				} else {
					m_WorldObject.ClearFlags(EntityFlags.SOLID, true);
				}
				
				m_WorldObject.Update();
				break;
			}
			
			case "ExpansionTraderType": {
				// storing the custom data				
				m_Data.Parameters["ExpansionTraderType"] = SerializableParam1<string>.Create(ExpansionTraderType);
				break;
			}
		}
			
		Update();
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
		if (!enable || !GetEditor().Settings.ShowBoundingBoxes) {
			return;
		}
		
		_boundingBoxesCreated = enable;
		
		vector size = GetSize();
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		
		for (int i = 0; i < 12; i++) {
			vector transform[4];			
			transform[3] = m_LineCenters[i];
			
			for (int j = 0; j < 3; j++) {
				transform[j][j] = ((position[j] == m_LineCenters[i][j]) * size[j]/2) + BOUNDING_BOX_THICKNESS;						
			}
			 
			m_BBoxLines[i] = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", m_LineCenters[i], ECE_NONE));
			m_BBoxLines[i].SetTransform(transform);			
			
			AddChild(m_BBoxLines[i], -1);
		}
		
		vector y_axis_mat[4];
		vector bottom_center = GetBottomCenter() - GetPosition();
		y_axis_mat[0][0] = BOUNDING_BOX_THICKNESS;
		y_axis_mat[1][1] = 1000;
		y_axis_mat[2][2] = BOUNDING_BOX_THICKNESS;
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
		Show = show;
		
		if (m_EditorObjectMapMarker) {
			m_EditorObjectMapMarker.Show(Show);
		}
		
		if (m_EditorObjectWorldMarker) {
			m_EditorObjectWorldMarker.Show(Show);
		}
		
		if (Show) {
			GetWorldObject().SetFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
		} else {
			GetWorldObject().ClearFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
		}
	}
	
	void ShowWorldObject(bool show) 
	{
		if (show) {
			GetWorldObject().SetFlags(EntityFlags.VISIBLE, false);
		} else {
			GetWorldObject().ClearFlags(EntityFlags.VISIBLE, false);
		}
	}
		
	vector GetBottomCenter()
	{		
		vector pos = GetPosition();
		pos[1] = pos[1] - GetYDistance();
		return pos;
	}
	
	float GetYDistance()
	{
		return ((GetPosition() - m_BasePoint.GetPosition())[1]);
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
	
	vector GetTopCenter()
	{		
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector result;
		vector up = GetTransformAxis(1);
		result = up * (vector.Distance(Vector(0, clip_info[0][1], 0), Vector(0, clip_info[1][1], 0)) / 2);
		result += GetPosition();
		return result;
	}
		
	Param3<int, vector, vector> GetTransformArray() 
	{
		return new Param3<int, vector, vector>(GetID(), GetPosition(), GetOrientation());
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

	void ShowBoundingBox()
	{
		EditorLog.Trace("EditorObject::ShowBoundingBox");
		
		// Global Settings Check
		if (!GetEditor().Settings.ShowBoundingBoxes) return;
		
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
		Locked = locked;
		m_Data.Locked = Locked;
		
		EditorObjectMarker marker = GetMarker();
		if (marker) {
			marker.Show(!Locked);
		}
				
		if (m_EditorPlacedListItem) {
			m_EditorPlacedListItem.LockedImage.Show(Locked);
		}
	}
	
	void EnablePhysics(bool enable)
	{
		Physics = enable;
		
		if (m_WorldObject) {
			if (enable) {
				m_WorldObject.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
				m_WorldObject.SetDynamicPhysicsLifeTime(-1);
				dBodySetMass(m_WorldObject, 100);
			} else {
				m_WorldObject.SetDynamicPhysicsLifeTime(0.001);
			}
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
		Print(TestingScript);
		if (script_content == string.Empty) {
			script_content = TestingScript;
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