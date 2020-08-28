

class EditorObjectSet: map<int, ref EditorObject>
{
	
	void ~EditorObjectSet()
	{
		EditorLog.Trace("~EditorObjectSet");
		GetEditor().DeleteObjects(this);
	}
	
	bool InsertEditorObject(EditorObject target) { return Insert(target.GetID(), target); }
	void RemoveEditorObject(EditorObject target) { Remove(target.GetID()); }
}

class EditorObject
{
	protected ref EditorObjectData 			m_Data;
	protected ref EditorObjectMapMarker		m_EditorObjectMapMarker;
	protected ref EditorObjectWorldMarker	m_EditorObjectWorldMarker;
	protected ref EditorPlacedListItem 		m_EditorPlacedListItem;
	
	protected EntityAI 		m_WorldObject;
	protected EntityAI		m_BBoxLines[12];	
	protected EntityAI 		m_BBoxBase;
	protected EntityAI 		m_CenterLine;
	
	
	float LocalAngle; // temp
	
	static float line_width = 0.02;
	
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();

	void SetDisplayName(string display_name) {
		m_Data.DisplayName = display_name;
		m_EditorPlacedListItem.GetController().EditorListItemLabelData = m_Data.DisplayName;
		m_EditorPlacedListItem.GetController().NotifyPropertyChanged("EditorListItemLabelData");
	}
	
	string GetDisplayName() { 
		return m_Data.DisplayName; 
	}
	string GetType() { return m_Data.Type; }
	int GetID() { return m_Data.GetID(); }
	
	
	void EditorObject(ref EditorObjectData data)
	{
		EditorLog.Trace("EditorObject");
		m_Data = data;
	
		GetEditor().GetSessionCache().InsertEditorData(m_Data);
	
		if (m_Data.Flags == EditorObjectFlags.ALL) {
			m_Data.Flags = EditorObjectFlags.BBOX | EditorObjectFlags.MAPMARKER | EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.LISTITEM;
		}
		
		m_WorldObject = GetGame().CreateObjectEx(m_Data.Type, m_Data.Position, ECE_LOCAL);
		m_WorldObject.SetOrientation(m_Data.Orientation);
		m_WorldObject.SetFlags(EntityFlags.STATIC, true);
		Update();
		
		
		
		// Bounding Box
		if ((m_Data.Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX) {
			CreateBoundingBox();
		}	

		// Map marker
		if ((m_Data.Flags & EditorObjectFlags.MAPMARKER) == EditorObjectFlags.MAPMARKER) {
			m_EditorObjectMapMarker = new EditorObjectMapMarker(this);
			GetEditor().GetEditorHud().GetController().InsertMapMarker(m_EditorObjectMapMarker);
		}	
		
		// World Object base marker
		if ((m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER) {
			m_EditorObjectWorldMarker = new EditorObjectWorldMarker(this);
		}
		
		// Browser item
		if ((m_Data.Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM) {
			m_EditorPlacedListItem = new EditorPlacedListItem(this);
			GetEditor().GetEditorHud().GetController().InsertPlacedObject(m_EditorPlacedListItem);
		}
		
		
	
	}
	
	void ~EditorObject()
	{
		EditorLog.Trace("~EditorObject");
		m_Data.Position = GetPosition();
		m_Data.Orientation = GetOrientation();
		
		delete m_EditorObjectWorldMarker; 
		delete m_EditorPlacedListItem;
		delete m_EditorObjectMapMarker;
	
		
		for (int i = 0; i < 12; i++)
			GetGame().ObjectDelete(m_BBoxLines[i]);
		
		GetGame().ObjectDelete(m_WorldObject);
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);
		
		
	}
		
	
	
	static EditorObject CreateFromExistingObject(notnull Object target)	
	{
		// todo
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
		m_Data.Position = pos;
		Update();
	}
	
	vector GetOrientation() { return m_WorldObject.GetOrientation(); }
	void SetOrientation(vector pos) 
	{ 
		m_WorldObject.SetOrientation(pos);
		m_Data.Orientation = pos; 
		Update();
	}
	
	void GetTransform(out vector mat[4]) { 
		m_WorldObject.GetTransform(mat); 
	}
	
	void SetTransform(vector mat[4]) { 
		m_WorldObject.SetTransform(mat); 
		Update();
	}
	
	void Update() { 
		m_WorldObject.Update(); 
	}
	
	void PlaceOnSurfaceRotated(out vector trans[4], vector pos, float dx = 0, float dz = 0, float fAngle = 0, bool align = false) {
		m_WorldObject.PlaceOnSurfaceRotated(trans, pos, dx, dz, fAngle, align); 
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
	

	vector line_centers[12]; vector line_verticies[8];
	void CreateBoundingBox()
	{
		EditorLog.Trace("EditorObject::CreateBoundingBox");
		
		vector size = GetSize();
			
		vector clip_info[2];
		ClippingInfo(clip_info);
		//clip_info[0][1] = -clip_info[1][1];
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		
		line_verticies[0] = clip_info[0];
		line_verticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		line_verticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		line_verticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		line_verticies[4] = Vector(clip_info[1][0], clip_info[1][1], clip_info[0][2]);
		line_verticies[5] = clip_info[1];
		line_verticies[6] = Vector(clip_info[0][0], clip_info[1][1], clip_info[1][2]);
		line_verticies[7] = Vector(clip_info[0][0], clip_info[1][1], clip_info[0][2]);
		
		line_centers[0] = AverageVectors(line_verticies[0], line_verticies[1]);
		line_centers[1] = AverageVectors(line_verticies[0], line_verticies[3]);
		line_centers[2] = AverageVectors(line_verticies[0], line_verticies[7]);
		line_centers[3] = AverageVectors(line_verticies[4], line_verticies[7]);
		line_centers[4] = AverageVectors(line_verticies[6], line_verticies[7]);
		
		line_centers[5] = AverageVectors(line_verticies[1], line_verticies[2]);
		line_centers[6] = AverageVectors(line_verticies[1], line_verticies[6]);
		line_centers[7] = AverageVectors(line_verticies[3], line_verticies[2]);
		line_centers[8] = AverageVectors(line_verticies[3], line_verticies[4]);
		
		line_centers[9] = AverageVectors(line_verticies[5], line_verticies[2]);
		line_centers[10] = AverageVectors(line_verticies[5], line_verticies[4]);		
		line_centers[11] = AverageVectors(line_verticies[5], line_verticies[6]);
		
		
		
		for (int i = 0; i < 12; i++) {
			
			vector transform[4];			
			transform[3] = line_centers[i];
			
			for (int j = 0; j < 3; j++) 
				transform[j][j] = ((position[j] == line_centers[i][j])*size[j]/2) + line_width;						
			 
			m_BBoxLines[i] = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", line_centers[i], ECE_LOCAL));
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
		
		HideBoundingBox();
		Update();
	}
	
	
	
	vector GetBottomCenter()
	{		
		vector clip_info[2];
		ClippingInfo(clip_info);
		//clip_info[0][1] = -clip_info[1][1];
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
		//clip_info[0][1] = -clip_info[1][1];
		vector result;
		vector up = GetTransformAxis(1);
		result = up * (vector.Distance(Vector(0, clip_info[0][1], 0), Vector(0, clip_info[1][1], 0)) / 2);
		result += GetPosition();
		return result;
	}
		
	ref Param3<int, vector, vector> TransformBeforeDrag;
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
	
	

	void SetTransformWithSnapping(vector transform[4])
	{	
		SetTransform(transform);
		

		// I cant wait to delete this... but not yet
		if (EditorSettings.SNAPPING_MODE) {
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
						Print(vector.Distance(pos, current_pos));
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
		if (!BoundingBoxEnabled()) return;
		if (BoundingBoxVisible) return;
		EditorLog.Trace("EditorObject::ShowBoundingBox");
		BoundingBoxVisible = true;
		for (int i = 0; i < 12; i++) {
			m_BBoxLines[i].SetObjectTexture(m_BBoxLines[i].GetHiddenSelectionIndex("BoundingBoxSelection"), "#(argb,8,8,3)color(1,1,0,1,co)");
			m_BBoxLines[i].Update();
		}
		
		m_CenterLine.SetObjectTexture(m_CenterLine.GetHiddenSelectionIndex("BoundingBoxSelection"), "#(argb,8,8,3)color(1,1,0,1,co)");
		//m_BBoxBase.SetObjectTexture(m_BBoxBase.GetHiddenSelectionIndex("BoundingBoxBase"), "#(argb,8,8,3)color(1,1,0,1,co)");
	}
	
	void HideBoundingBox()
	{
		if (!BoundingBoxEnabled()) return;
		if (!BoundingBoxVisible) return;
		EditorLog.Trace("EditorObject::HideBoundingBox");
		BoundingBoxVisible = false;
		for (int i = 0; i < 12; i++) {
			m_BBoxLines[i].SetObjectTexture(m_BBoxLines[i].GetHiddenSelectionIndex("BoundingBoxSelection"), "");
			m_BBoxLines[i].Update();
		}
		
		
		m_CenterLine.SetObjectTexture(m_CenterLine.GetHiddenSelectionIndex("BoundingBoxSelection"), "");
	}
	

	

	

	
}






