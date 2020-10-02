

class EditorObjectSet: ref map<int, ref EditorObject>
{
	
	void ~EditorObjectSet() {
		EditorLog.Trace("~EditorObjectSet");
	}
	
	bool InsertEditorObject(EditorObject target) { 
		return Insert(target.GetID(), target); 
	}
	
	void RemoveEditorObject(EditorObject target) { 
		Remove(target.GetID()); 
	}
}


class EditorObject
{
	protected autoptr EditorObjectData 		m_Data;
	protected ref EditorObjectMapMarker		m_EditorObjectMapMarker;
	protected ref EditorObjectWorldMarker	m_EditorObjectWorldMarker;
	protected ref EditorPlacedListItem 		m_EditorPlacedListItem;
	
	protected Object 		m_WorldObject;
	protected Object		m_BBoxLines[12];	
	protected Object 		m_BBoxBase;
	protected Object 		m_CenterLine;
	
	protected ref array<ref EditorSnapPoint> m_SnapPoints = {};
	
	private vector m_LineCenters[12]; 
	private vector m_LineVerticies[8];
	
	
	float LocalAngle; // temp
	
	static float line_width = 0.02;
	
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();

	void SetDisplayName(string display_name) {
		m_Data.DisplayName = display_name;
		EditorListItemController.Cast(m_EditorPlacedListItem.GetController()).ListItemLabel = m_Data.DisplayName;
		m_EditorPlacedListItem.GetController().NotifyPropertyChanged("ListItemLabel");
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

	private void EditorObject(notnull Object target, EditorObjectFlags flags)
	{
		EditorLog.Trace("EditorObject");
		m_WorldObject = target;		
		m_Data = EditorObjectData.Create(target, flags);
		m_Data.Flags = flags;
		
		if (GetEditor()) {
			GetEditor().GetSessionCache().InsertEditorData(m_Data);
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
		
		for (int i = 0; i < 8; i++) {
			m_SnapPoints.Insert(new EditorSnapPoint(this, m_LineVerticies[i]));
		}
		
		// Bounding Box
		if ((m_Data.Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX) {
			ShowBoundingBox();
		}	

		// Map marker
		if ((m_Data.Flags & EditorObjectFlags.MAPMARKER) == EditorObjectFlags.MAPMARKER) {
			m_EditorObjectMapMarker = new EditorObjectMapMarker(this);
			GetEditor().GetEditorHud().GetTemplateController().InsertMapMarker(m_EditorObjectMapMarker);
		}	
		
		// World Object base marker
		if ((m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER) {
			m_EditorObjectWorldMarker = new EditorObjectWorldMarker(this);
		}
		
		// Browser item
		if ((m_Data.Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM) {
			m_EditorPlacedListItem = new EditorPlacedListItem();
			m_EditorPlacedListItem.SetEditorObject(this);
			GetEditor().GetEditorHud().GetTemplateController().RightbarSpacerData.Insert(m_EditorPlacedListItem);
		}
		
		EditorEvents.OnMapToggled.Insert(OnMapToggled);
	}
	
		
	void ~EditorObject()
	{
		EditorLog.Trace("~EditorObject");
		GetTransform(m_Data.Transform);
		
		HideBoundingBox();
		
		delete m_EditorObjectWorldMarker; 
		delete m_EditorPlacedListItem;
		delete m_EditorObjectMapMarker;
		
		delete OnObjectSelected;
		delete OnObjectDeselected;
			
		GetGame().ObjectDelete(m_WorldObject);
	}
	
	static EditorObject Create(EditorObjectData data)
	{
		EditorLog.Trace("EditorObject::Create from EditorObjectData");
		Object world_object = GetGame().CreateObjectEx(data.Type, data.Transform[3], ECE_LOCAL | ECE_CREATEPHYSICS);
		world_object.SetTransform(data.Transform);
		world_object.SetFlags(EntityFlags.STATIC, true);
		
		return new EditorObject(world_object, data.Flags);
	}
	
	static EditorObject Create(notnull Object target, EditorObjectFlags flags)
	{
		EditorLog.Trace("EditorObject::Create from Object");
		return new EditorObject(target, flags);
	}	

	
	/*********
	* Events *
	*********/
	
	void OnMapToggled(Class context, MapWidget editor_map, bool state)
	{
		EditorLog.Trace("EditorObject::OnMapToggled");
		
		m_EditorObjectWorldMarker.Show(!state);
		m_EditorObjectMapMarker.Show(state);		
	}
	
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
	
	EntityAI GetWorldObject() {
		return m_WorldObject;
	}
	
	bool OnMouseEnter(int x, int y)	{
		return true;
	}
	
	bool OnMouseLeave(int x, int y) {
		return true;
	}

	vector GetPosition() { 
		return m_WorldObject.GetPosition(); 
	}
	
	void SetPosition(vector pos) 
	{ 
		m_WorldObject.SetPosition(pos); 
		GetTransform(m_Data.Transform);
		Update();
	}
	
	vector GetOrientation() { return m_WorldObject.GetOrientation(); }
	void SetOrientation(vector pos) 
	{ 
		m_WorldObject.SetOrientation(pos);
		GetTransform(m_Data.Transform);
		Update();
	}
	
	void GetTransform(out vector mat[4]) 
	{ 
		m_WorldObject.GetTransform(mat); 
	}
	
	void SetTransform(vector mat[4]) 
	{ 
		m_WorldObject.SetTransform(mat); 
		m_Data.Transform = mat;
		Update();
	}
	
	void Update() { 
		m_WorldObject.Update(); 
	}
	
	void PlaceOnSurfaceRotated(out vector trans[4], vector pos, float dx = 0, float dz = 0, float fAngle = 0, bool align = false) {
		
		EntityAI ent;
		if (Class.CastTo(ent, m_WorldObject)) {
			ent.PlaceOnSurfaceRotated(trans, pos, dx, dz, fAngle, align); 
		}
	}
	
	void ClippingInfo(out vector clip_info[2]) { 
		m_WorldObject.ClippingInfo(clip_info); 
	}
	
	void SetDirection(vector direction) { 
		m_WorldObject.SetDirection(direction); 
	}
	
	void AddChild(notnull IEntity child, int pivot, bool position_only = false) { 
		m_WorldObject.AddChild(child, pivot, position_only); 
	}
	
	vector GetTransformAxis(int axis) { 
		return m_WorldObject.GetTransformAxis(axis); 
	}
	
	string GetModelName() { 
		return m_WorldObject.GetModelName(); 
	}
	
	bool ListItemEnabled() { 
		return (m_Data.Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM; 
	}
	
	bool ObjectMarkerEnabled() { 
		return (m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER; 
	}
	
	bool MapMarkerEnabled() { 
		return (m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER;
	}

	bool BoundingBoxEnabled() { 
		return (m_Data.Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX; 
	}
	
	void GetObjectMarkerPosition(out float x, out float y) {
		if (ObjectMarkerEnabled()) {
			m_EditorObjectWorldMarker.GetPos(x, y);
		}
	}
	
	void GetMapMarkerPosition(out float x, out float y) {
		if (MapMarkerEnabled()) {
			m_EditorObjectMapMarker.GetPos(x, y);
		}
	}
	
	private bool m_Visible = true;
	bool IsVisible() {
		return m_Visible;
	}
	
	void Show(bool show) {
		m_Visible = show;
		
		if (MapMarkerEnabled()) {
			m_EditorObjectMapMarker.Show(m_Visible);
		}
		
		if (ListItemEnabled() && m_EditorPlacedListItem.GetLayoutRoot()) {
			m_EditorPlacedListItem.GetLayoutRoot().Show(m_Visible);
		}
	}
	
	void ShowWorldObject(bool show) { 
		
		if (show) {
			m_WorldObject.SetFlags(EntityFlags.VISIBLE, false);
		} else {
			m_WorldObject.ClearFlags(EntityFlags.VISIBLE, false);
		}
	}
	
	
	
	vector GetBottomCenter()
	{		
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector result;
		vector up = GetTransformAxis(1);
		result = up * -(vector.Distance(Vector(0, clip_info[0][1], 0), Vector(0, clip_info[1][1], 0)) / 2);
		result += GetPosition();
	
		return result;
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
		
	ref Param3<int, vector, vector> TransformBeforeDrag;
	Param3<int, vector, vector> GetTransformArray() {	
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

	void SetTransformWithSnapping(vector transform[4])
	{	
		SetTransform(transform);
		

		// I cant wait to delete this... but not yet
		if (GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().SnapButton) {
			vector current_size = GetSize();
			vector current_pos = GetPosition();
			float snap_radius = 5;

			foreach (EditorObject editor_object: GetEditor().GetPlacedObjects()) {
				if (editor_object == this) continue;
				
				vector size = editor_object.GetSize();
				vector position = editor_object.GetPosition();
				
				if (vector.Distance(current_pos, position) < 100) {
					
					for (int i = 0; i < 12; i++) {
						vector pos = editor_object.m_BBoxLines[i].GetPosition();
						if (vector.Distance(pos, current_pos) < snap_radius) {
							SetPosition(pos);
							Update();
							return;
						}						
					}
				}
			}		
		}
		
		Update();
	}
		

	private bool BoundingBoxVisible;
	void ShowBoundingBox()
	{
		if (!BoundingBoxEnabled() || BoundingBoxVisible) return;
		EditorLog.Trace("EditorObject::ShowBoundingBox");
		BoundingBoxVisible = true;

		vector size = GetSize();
		vector clip_info[2];
		ClippingInfo(clip_info);
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		
		for (int i = 0; i < 12; i++) {
			
			vector transform[4];			
			transform[3] = m_LineCenters[i];
			
			for (int j = 0; j < 3; j++) 
				transform[j][j] = ((position[j] == m_LineCenters[i][j])*size[j]/2) + line_width;						
			 
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
		
	}
	
	void HideBoundingBox()
	{
		if (!BoundingBoxEnabled() || !BoundingBoxVisible) return;
		EditorLog.Trace("EditorObject::HideBoundingBox");
		BoundingBoxVisible = false;
		
		for (int i = 0; i < 12; i++)
			GetGame().ObjectDelete(m_BBoxLines[i]);
		
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);	
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
		if (m_WorldObject.IsMan()) {
			DayZPlayerImplement.Cast(m_WorldObject).EditorAnimationReset();
		}
	}
	
	void PauseSimulation(bool pause)
	{
		EditorLog.Trace("EditorObject::PauseSimulation");
		
		EntityAI ent;
		if (Class.CastTo(ent, m_WorldObject)) {
			ent.DisableSimulation(pause);
		}
		
		
	}

	
}







