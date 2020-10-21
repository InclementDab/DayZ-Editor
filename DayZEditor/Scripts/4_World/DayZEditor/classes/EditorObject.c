class EditorWorldObject
{
	protected EntityAI m_WorldObject;
	EntityAI GetWorldObject() {
		return m_WorldObject;
	}
	
	void ~EditorWorldObject()
	{
		if (m_WorldObject) {
			GetGame().ObjectDelete(m_WorldObject);
		}
	}
	
	static EntityAI CreateObject(string type, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		// Set to ECE_SETUP for AI compat. DONT ADD ECE_LOCAL
		EntityAI obj; 
		if (!Class.CastTo(obj, GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH))) { // ECE_CREATEPHYSICS, ECE_UPDATEPATHGRAPH
			EditorLog.Error("EditorWorldObject: Invalid Object %1", type);
			return null;
		}
		
		obj.SetOrientation(orientation);
		obj.SetFlags(EntityFlags.STATIC, true);
		
		// Needed for AI Placement		
		obj.DisableSimulation(true);
		
		return obj;
	}
}


class EditorObject: EditorWorldObject
{
	protected EditorObjectData 				m_Data;
	protected ref EditorObjectMapMarker		m_EditorObjectMapMarker;
	protected ref EditorObjectWorldMarker	m_EditorObjectWorldMarker;
	protected ref EditorPlacedListItem 		m_EditorPlacedListItem;
	
	protected Object		m_BBoxLines[12];	
	protected Object 		m_BBoxBase;
	protected Object 		m_CenterLine;
	protected Object		m_BasePoint;
	
	protected ref array<ref EditorSnapPoint> m_SnapPoints = {};
	
	private vector m_LineCenters[12]; 
	private vector m_LineVerticies[8];
	
	static float line_width = 0.02;
	
	string Name;
	vector Position;
	vector Orientation;
	float Scale;
	
	bool Show;
	bool Locked;
	bool StaticObject;
	
	bool BoundingBoxEnabled;
	bool WorldMarkerEnabled;
	bool MapMarkerEnabled;
	bool ListItemEnabled;
	
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();

	void SetDisplayName(string display_name) {
		m_Data.DisplayName = display_name;
		EditorListItemController.Cast(m_EditorPlacedListItem.GetController()).Label = m_Data.DisplayName;
		m_EditorPlacedListItem.GetController().NotifyPropertyChanged("Label");
	}
	
	string GetDisplayName() { 
		return m_Data.DisplayName; 
	}
	
	string GetType() { 
		return m_Data.Type; 
	}
	
	int GetID() { 
		return m_Data.GetID(); 
	}

	EditorObjectFlags GetFlags() {
		return m_Data.Flags;
	}
	
	Object GetWorldObject() 
	{
		if (!m_WorldObject) {
			EditorLog.Error("World Object was null! ID: %1", GetID().ToString());
		}
		
		return m_WorldObject;
	}
	
	void EditorObject(EditorObjectData data)
	{
		EditorLog.Trace("EditorObject " + data);
		m_Data = data;
		
		if (!m_Data.WorldObject) {
			m_WorldObject = CreateObject(m_Data.Type, m_Data.Position, m_Data.Orientation);
			m_Data.WorldObject = m_WorldObject;
		} else {
			m_WorldObject = m_Data.WorldObject;
		}
		
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
		m_BasePoint = GetGame().CreateObjectEx("BoundingBoxBase", base_point, ECE_NONE);
		m_BasePoint.SetScale(0.001);
		
		AddChild(m_BasePoint, 0);
		
		for (int i = 0; i < 8; i++) {
			m_SnapPoints.Insert(new EditorSnapPoint(this, m_LineVerticies[i]));
		}
		
		// Bounding Box
		BoundingBoxEnabled = ((m_Data.Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX);
		thread EnableBoundingBox(BoundingBoxEnabled);
		
		// Map marker
		MapMarkerEnabled = ((m_Data.Flags & EditorObjectFlags.MAPMARKER) == EditorObjectFlags.MAPMARKER);
		thread EnableMapMarker(MapMarkerEnabled);
		
		// World marker
		WorldMarkerEnabled = ((m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER);
		thread EnableObjectMarker(WorldMarkerEnabled);
		
		// Browser item
		ListItemEnabled = ((m_Data.Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM);
		thread EnableListItem(ListItemEnabled);
		

		m_SnapPoints.Insert(new EditorSnapPoint(this, Vector(0, -GetYDistance(), 5)));
	}
	
		
	void ~EditorObject()
	{
		EditorLog.Trace("~EditorObject");
		Update();

		DestroyBoundingBox();
		
		delete m_EditorObjectWorldMarker; 
		delete m_EditorPlacedListItem;
		delete m_EditorObjectMapMarker;
		
		delete OnObjectSelected;
		delete OnObjectDeselected;
	}
			
	/*********
	* Events *
	*********/
	
	private bool m_IsSelected;
	bool IsSelected() return m_IsSelected;
	void OnSelected()
	{
		if (m_IsSelected) return;
		EditorLog.Trace("EditorObject::OnSelected");
		m_IsSelected = true;
		ShowBoundingBox();
		OnObjectSelected.Invoke(this);
	}
	
	void OnDeselected()
	{
		if (!m_IsSelected) return;
		EditorLog.Trace("EditorObject::OnDeselected");
		m_IsSelected = false;
		HideBoundingBox();
		OnObjectDeselected.Invoke(this);
	}
	
	EditorObjectData GetData() {
		return m_Data;
	}
	
	bool OnMouseEnter(int x, int y)	{
		return true;
	}
	
	bool OnMouseLeave(int x, int y) {
		return true;
	}

	vector GetPosition() { 
		return GetWorldObject().GetPosition(); 
	}
	
	void SetPosition(vector pos) 
	{ 
		if (m_Locked) return;
		GetWorldObject().SetPosition(pos);
		Update();
	}
	
	vector GetOrientation() { return GetWorldObject().GetOrientation(); }
	void SetOrientation(vector pos) 
	{ 
		if (m_Locked) return;
		GetWorldObject().SetOrientation(pos);
		Update();
	}
	
	void GetTransform(out vector mat[4]) 
	{ 
		GetWorldObject().GetTransform(mat); 
	}
	
	void SetTransform(vector mat[4]) 
	{ 	
		if (m_Locked) return;	
		GetWorldObject().SetTransform(mat); 
		Update();
	}
	
	void SetScale(float scale)
	{		
		if (m_Locked) return;
		//GetWorldObject().SetScale(scale);
		Update();
	}
	
	float GetScale()
	{
		//return GetWorldObject().GetScale();
		return 1;
	}
	
	void Update() 
	{ 
		GetWorldObject().Update(); 
		
		if (m_Data) {
			m_Data.Position = GetPosition();
			m_Data.Orientation = GetOrientation();
			m_Data.Scale = GetScale();
			m_Data.BottomCenter = GetBottomCenter();
			
			Name = GetDisplayName();
			Position = GetPosition();
			Orientation = GetOrientation();
		}
		
		
		Debug.DestroyAllShapes();
		
		if (GetEditor().Settings.DebugMode) {
			foreach (EditorSnapPoint point: m_SnapPoints) {
				Debug.DrawSphere(point.GetWorldObject().GetWorldPosition());
			}
		}
	}
	
	// EditorObjects can also be psuedo-controllers
	void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorObject::PropertyChanged %1", property_name);
		
		
		
		switch (property_name) {
			
			case "Name": {
				SetDisplayName(Name);
				break;
			}
			
			case "Position": 
			case "Orientation": {
				EditorAction property_undo = new EditorAction("SetTransform", "SetTransform");
				property_undo.InsertUndoParameter(this, GetTransformArray());
				SetPosition(Position);
				SetOrientation(Orientation);
				property_undo.InsertRedoParameter(this, GetTransformArray());
				GetEditor().InsertAction(property_undo);
				break;
			}
			
			case "Scale": {
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
			
			case "StaticObject": {
				SetStaticObject(StaticObject);
				break;
			}			
			
			case "BoundingBoxEnabled": {
				//EnableBoundingBox(BoundingBoxEnabled);
				break;
			}
			
			case "WorldMarkerEnabled": {
				//EnableObjectMarker(WorldMarkerEnabled);
				break;
			}
			
			case "MapMarkerEnabled": {
				//EnableMapMarker(MapMarkerEnabled);
				break;
			}
			
			case "ListItemEnabled": {
				//EnableListItem(ListItemEnabled);
				break;
			}
		}
	}
	
	void PlaceOnSurfaceRotated(out vector trans[4], vector pos, float dx = 0, float dz = 0, float fAngle = 0, bool align = false) {
		
		EntityAI ent;
		if (Class.CastTo(ent, GetWorldObject())) {
			ent.PlaceOnSurfaceRotated(trans, pos, dx, dz, fAngle, align); 
		}
	}
	
	void ClippingInfo(out vector clip_info[2]) { 
		GetWorldObject().ClippingInfo(clip_info); 
	}
	
	void SetDirection(vector direction) { 
		GetWorldObject().SetDirection(direction); 
	}
	
	void AddChild(notnull IEntity child, int pivot, bool position_only = false) { 
		GetWorldObject().AddChild(child, pivot, position_only); 
	}
	
	vector GetTransformAxis(int axis) { 
		return GetWorldObject().GetTransformAxis(axis); 
	}
	
	string GetModelName() { 
		return GetWorldObject().GetModelName(); 
	}
	
	void EnableListItem(bool enable) 
	{
		EditorLog.Trace("EditorObject::EnableListItem");
		
		if (m_EditorPlacedListItem)
			delete m_EditorPlacedListItem;
		
		if (!enable) {
			return;
		}
		
		m_EditorPlacedListItem = new EditorPlacedListItem();
		m_EditorPlacedListItem.SetEditorObject(this);
		GetEditor().GetEditorHud().GetTemplateController().RightbarSpacerData.Insert(m_EditorPlacedListItem);
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

	void EnableBoundingBox(bool enable) 
	{
		EditorLog.Trace("EditorObject::EnableBoundingBox");
		DestroyBoundingBox();
		if (!enable) {
			return;
		}
		
		vector size = GetSize();
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		
		for (int i = 0; i < 12; i++) {
			
			vector transform[4];			
			transform[3] = m_LineCenters[i];
			
			for (int j = 0; j < 3; j++) 
				transform[j][j] = ((position[j] == m_LineCenters[i][j]) * size[j]/2) + line_width;						
			 
			m_BBoxLines[i] = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", m_LineCenters[i], ECE_LOCAL));
			m_BBoxLines[i].SetTransform(transform);			
			
			AddChild(m_BBoxLines[i], -1);
		}
		
		
		vector y_axis_mat[4];
		vector bottom_center = GetBottomCenter() - GetPosition();
		y_axis_mat[0][0] = line_width;
		y_axis_mat[1][1] = 1000;
		y_axis_mat[2][2] = line_width;
		y_axis_mat[3] = Vector(bottom_center[0], bottom_center[1] - y_axis_mat[1][1], bottom_center[2]);
		
		m_CenterLine = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", bottom_center, ECE_LOCAL));
		m_CenterLine.SetTransform(y_axis_mat);
		AddChild(m_CenterLine, -1);
		Update();
		
		HideBoundingBox();
	}
	
	void DestroyBoundingBox()
	{		
		for (int i = 0; i < 12; i++)
			if (m_BBoxLines[i])
				GetGame().ObjectDelete(m_BBoxLines[i]);
		
		if (m_BBoxBase)
			GetGame().ObjectDelete(m_BBoxBase);
		
		if (m_CenterLine)
			GetGame().ObjectDelete(m_CenterLine);	
	}
	
	private bool m_Visible = true;
	bool IsVisible() {
		return m_Visible;
	}
	
	void Show(bool show) 
	{
		m_Visible = show;
		
		if (m_EditorObjectMapMarker) {
			m_EditorObjectMapMarker.Show(m_Visible);
		}
		
		if (m_EditorObjectWorldMarker) {
			m_EditorObjectWorldMarker.Show(m_Visible);
		}
		
		if (m_Visible) {
			GetWorldObject().SetFlags(EntityFlags.VISIBLE | EntityFlags.SOLID | EntityFlags.TOUCHTRIGGERS, true);
		} else {
			GetWorldObject().ClearFlags(EntityFlags.VISIBLE | EntityFlags.SOLID | EntityFlags.TOUCHTRIGGERS, true);
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
		return m_BasePoint.GetWorldPosition();
	}
	
	float GetYDistance()
	{
		return ((GetPosition() - m_BasePoint.GetPosition())[1]);
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
		
		for (int i = 0; i < 12; i++) {
			if (m_BBoxLines[i]) {
				m_BBoxLines[i].SetFlags(EntityFlags.VISIBLE, false);
			}
		}
		
		if (m_BBoxBase)
			m_BBoxBase.SetFlags(EntityFlags.VISIBLE, false);
		
		if (m_CenterLine)
			m_CenterLine.SetFlags(EntityFlags.VISIBLE, false);
	}
	
	void HideBoundingBox()
	{
		EditorLog.Trace("EditorObject::HideBoundingBox");
		
		for (int i = 0; i < 12; i++) {
			if (m_BBoxLines[i]) {
				m_BBoxLines[i].ClearFlags(EntityFlags.VISIBLE, false);
			}
		}
		
		if (m_BBoxBase)
			m_BBoxBase.ClearFlags(EntityFlags.VISIBLE, false);
		
		if (m_CenterLine)
			m_CenterLine.ClearFlags(EntityFlags.VISIBLE, false);
	}
	

	bool SetAnimation(string anim_name)
	{
		EditorLog.Trace("EditorObject::SetAnimation");
		if (m_WorldObject.IsMan()) {
			DayZPlayerImplement.Cast(m_WorldObject).EditorAnimationStart(anim_name);
			return true;
		}
		
		return false;
	}

	
	void ResetAnimation()
	{
		EditorLog.Trace("EditorObject::SetAnimation");
		if (GetWorldObject().IsMan()) {
			DayZPlayerImplement.Cast(GetWorldObject()).EditorAnimationReset();
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

	protected bool m_StaticObject;
	bool IsStaticObject() {
		return m_StaticObject;
	}
	
	void SetStaticObject(bool static_object) {
		m_StaticObject = static_object;
	}
	
	protected bool m_Locked;
	bool IsLocked() {
		return m_Locked;
	}
	
	void Lock(bool locked) {
		m_Locked = locked;
		
		EditorObjectMarker marker = GetMarker();
		if (marker) {
			marker.GetLayoutRoot().Show(!m_Locked);
		}
	}
}