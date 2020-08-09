
enum EditorObjectFlags
{
	NONE = 0,
	BBOX = 2,
	MAPMARKER = 4,
	OBJECTMARKER = 8,
	LISTITEM = 16,
	ALL = 256
};

typedef ref array<ref EditorObjectData> EditorObjectDataSet;
class EditorObjectData
{	
	string Type;
	int ID;
	
	vector Position;
	vector Orientation;
	float Scale;
	EditorObjectFlags Flags;
	
	void EditorObjectData(string type, vector position, vector orientation = "0 0 0", EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		Type = type; Position = position; Orientation = orientation; Flags = flags;
	}
	
}

class EditorObject
{
	protected EntityAI 		m_WorldObject;
	EntityAI GetWorldObject() { return m_WorldObject; }

	
	protected Widget 		m_EditorObjectMarkerWidget;
	protected Widget 		m_EditorObjectBrowserWidget;
	protected Widget 		m_EditorMapMarkerWidget;
	protected Widget 		m_EditorObjectPropertiesWidget;
	protected Widget 		m_EditorObjectContextWidget;
	
	ref UILinkedObject 		m_EditorObjectMarker = null;
	ref UILinkedObject 		m_EditorObjectBrowser = null;
	ref UILinkedObject		m_EditorMapMarker = null;
	ref UILinkedObject		m_EditorObjectPropertiesWindow = null;
	ref UILinkedObject		m_EditorObjectContextMenu = null;
	
	EntityAI 				m_BBoxLines[12];	
	protected EntityAI 		m_BBoxBase;
	protected EntityAI 		m_CenterLine;
	
	float LocalAngle; // temp
	
	static float line_width = 0.05;
	private vector clip_info[2];
	
	private EditorObjectData m_Data;
	EditorObjectData GetData() { return m_Data; }
	
	string GetType() { return m_Data.Type; }
	int GetID() { return m_Data.ID; }
	
	void EditorObject(EditorObjectData data)
	{
		EditorPrint("EditorObject");
		m_Data = data;
		
		if (m_Data.Flags == EditorObjectFlags.ALL) {
			m_Data.Flags = EditorObjectFlags.BBOX | EditorObjectFlags.MAPMARKER | EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.LISTITEM;
		}

		m_WorldObject = GetGame().CreateObjectEx(data.Type, data.Position, ECE_NONE);
		m_WorldObject.SetFlags(EntityFlags.STATIC, true);
		Update();
		
		m_WorldObject.ClippingInfo(clip_info);
		
		
		// Bounding Box
		if ((m_Data.Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX) {
			CreateBoundingBox();
		}	

		// Map marker
		if ((m_Data.Flags & EditorObjectFlags.MAPMARKER) == EditorObjectFlags.MAPMARKER) {
			m_EditorMapMarker = new UILinkedObject();
			m_EditorMapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMapMarker.layout");
			m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
			m_EditorMapMarker.SetObject(this);
			GetEditor().GetUIManager().GetEditorUI().InsertMapObject(m_EditorMapMarkerWidget);
		}	
		
		// World Object base marker
		if ((m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER) {
			m_EditorObjectMarker = new UILinkedObject();
			m_EditorObjectMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorObjectMarker.layout");
			m_EditorObjectMarkerWidget.GetScript(m_EditorObjectMarker);
			m_EditorObjectMarker.SetObject(this);
		}
			
		// Browser item
		if ((m_Data.Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM) {
			m_EditorObjectBrowser = new UILinkedObject();
			m_EditorObjectBrowserWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorPlacedListItem.layout");
			m_EditorObjectBrowserWidget.GetScript(m_EditorObjectBrowser);
			m_EditorObjectBrowser.SetObject(this);
			GetEditor().GetUIManager().GetEditorUI().InsertPlacedObject(m_EditorObjectBrowser);
		}
		
		EditorEvents.ObjectCreateInvoke(this, this);
	
	}
	
	void ~EditorObject()
	{
		Print("~EditorObject");
		m_Data.Position = GetPosition();
		m_Data.Orientation = GetOrientation();
		
		delete m_EditorObjectMarker; 
		delete m_EditorObjectBrowser;
		delete m_EditorMapMarker;
		delete m_EditorObjectPropertiesWindow;
		delete m_EditorObjectContextMenu;
		
		delete m_EditorObjectMarkerWidget;
		delete m_EditorObjectBrowserWidget;
		delete m_EditorMapMarkerWidget;
		delete m_EditorObjectPropertiesWidget;
		
		for (int i = 0; i < 12; i++)
			GetGame().ObjectDelete(m_BBoxLines[i]);
		
		GetGame().ObjectDelete(m_WorldObject);
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);
		
		EditorEvents.ObjectDeleteInvoke(this, this);
	}
		
	
	/*
	* Initializers
	*/
	
	static EditorObject CreateFromExistingObject(notnull Object target)	
	{
		// todo
	}
	
	/*********
	* Events *
	*********/
	
	private bool m_IsSelected;
	bool IsSelected() { return m_IsSelected; }
	void OnSelected()
	{
		if (m_IsSelected) return;
		m_IsSelected = true;
		ShowBoundingBox();
	}
	
	void OnDeselected()
	{
		if (!m_IsSelected) return;
		m_IsSelected = false;
		HideBoundingBox();
	}
	
	
	bool OnMouseEnter(int x, int y)
	{
		return true;
	}
	
	bool OnMouseLeave(int x, int y)
	{
		return true;
	}

	
	/*
	* Functions
	*/
	
	vector GetPosition() { return m_WorldObject.GetPosition(); }
	void SetPosition(vector pos) 
	{ 
		m_WorldObject.SetPosition(pos); 
		m_Data.Position = pos;
	}
	
	vector GetOrientation() { return m_WorldObject.GetOrientation(); }
	void SetOrientation(vector pos) 
	{ 
		m_WorldObject.SetOrientation(pos);
		m_Data.Orientation = pos; 
	}
	
	void GetTransform(out vector mat[4]) { m_WorldObject.GetTransform(mat); }
	void SetTransform(vector mat[4]) { m_WorldObject.SetTransform(mat); }
	
	void Update() { m_WorldObject.Update(); }
	
	void PlaceOnSurfaceRotated(out vector trans[4], vector pos, float dx = 0, float dz = 0, float fAngle = 0, bool align = false) 
	{
		m_WorldObject.PlaceOnSurfaceRotated(trans, pos, dx, dz, fAngle, align); 
	}
	
	void ClippingInfo(out vector clip_info[]) { m_WorldObject.ClippingInfo(clip_info); }
	
	void SetDirection(vector direction) { m_WorldObject.SetDirection(direction); }
	
	void AddChild(notnull IEntity child, int pivot, bool position_only = false) { m_WorldObject.AddChild(child, pivot, position_only); }
	
	vector GetTransformAxis(int axis) { return m_WorldObject.GetTransformAxis(axis); }
	
	string GetModelName() { return m_WorldObject.GetModelName(); }
	
	string GetDisplayName()
	{
		string name = m_WorldObject.GetDisplayName();
		if (name != string.Empty)
			return name;
		else return GetType();
	}
	
	EditorWorldObject GetSaveData()
	{
		EditorWorldObject world_object = new EditorWorldObject();
		world_object.m_Typename = GetType();
		GetTransform(world_object.m_Transform);
		return world_object;
	}

	vector line_centers[12]; vector line_verticies[8];
	void CreateBoundingBox()
	{
		EditorPrint("EditorObject::CreateBoundingBox");
		
		vector size = GetSize();
			
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
		line_centers[1] = AverageVectors(line_verticies[1], line_verticies[2]);
		line_centers[2] = AverageVectors(line_verticies[2], line_verticies[3]);
		line_centers[3] = AverageVectors(line_verticies[3], line_verticies[4]);
		line_centers[4] = AverageVectors(line_verticies[4], line_verticies[5]);
		line_centers[5] = AverageVectors(line_verticies[5], line_verticies[6]);
		line_centers[6] = AverageVectors(line_verticies[6], line_verticies[7]);
		line_centers[7] = AverageVectors(line_verticies[0], line_verticies[3]);
		line_centers[8] = AverageVectors(line_verticies[7], line_verticies[4]);
		line_centers[9] = AverageVectors(line_verticies[2], line_verticies[5]);
		line_centers[10] = AverageVectors(line_verticies[1], line_verticies[6]);
		line_centers[11] = AverageVectors(line_verticies[0], line_verticies[7]);
				
	
		
		for (int i = 0; i < 12; i++) {
			
			vector transform[4];			
			transform[3] = line_centers[i];
			
			for (int j = 0; j < 3; j++) 
				transform[j][j] = ((position[j] == line_centers[i][j])*size[j]/2) + line_width;						
			 
			m_BBoxLines[i] = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", line_centers[i], ECE_NONE));
			m_BBoxLines[i].SetTransform(transform);			
			
			AddChild(m_BBoxLines[i], -1);
		}
		
		
		vector y_axis_mat[4];
		vector bottom_center = GetBottomCenter() - GetPosition();
		y_axis_mat[0][0] = line_width;
		y_axis_mat[1][1] = 1000;
		y_axis_mat[2][2] = line_width;
		y_axis_mat[3] = Vector(bottom_center[0], bottom_center[1] - y_axis_mat[1][1], bottom_center[2]);
		
		m_CenterLine = EntityAI.Cast(GetGame().CreateObjectEx("BoundingBoxBase", bottom_center, ECE_NONE));
		m_CenterLine.SetTransform(y_axis_mat);
		AddChild(m_CenterLine, -1);
		
		HideBoundingBox();
		Update();
	}
	
	


	vector GetBottomCenter()
	{
		vector result;
		vector up = GetTransformAxis(1);
		float dist = vector.Distance(Vector(0, clip_info[0][1], 0), Vector(0, 0, 0));
		
		result = up * -(dist);
		result += GetPosition();
		return result;
	}
	
	vector GetTopCenter()
	{
		vector result;
		vector up = GetTransformAxis(1);
		result = up * (vector.Distance(Vector(0, 0, 0), Vector(0, clip_info[1][1], 0)));
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

		//clip_info[0][1] = -clip_info[1][1];	
		result[0] = Math.AbsFloat(clip_info[0][0]) + Math.AbsFloat(clip_info[1][0]);
		result[1] = Math.AbsFloat(clip_info[0][1]) + Math.AbsFloat(clip_info[1][1]);
		result[2] = Math.AbsFloat(clip_info[0][2]) + Math.AbsFloat(clip_info[1][2]);
		
		return result;
	}
	
	

	void SetTransformWithSnapping(vector transform[4])
	{	
		SetTransform(transform);
		Update();
		

		// I cant wait to delete this... but not yet
		if (EditorSettings.SNAPPING_MODE) {
			vector current_size = GetSize();
			vector current_pos = GetPosition();
			float snap_radius = 5;

			foreach (EditorObject editor_object: GetEditor().GetObjectManager().GetPlacedObjects()) {
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
		EditorPrint("EditorObject::ShowBoundingBox");
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
		EditorPrint("EditorObject::HideBoundingBox");
		BoundingBoxVisible = false;
		for (int i = 0; i < 12; i++) {
			m_BBoxLines[i].SetObjectTexture(m_BBoxLines[i].GetHiddenSelectionIndex("BoundingBoxSelection"), "");
			m_BBoxLines[i].Update();
		}
		
		
		m_CenterLine.SetObjectTexture(m_CenterLine.GetHiddenSelectionIndex("BoundingBoxSelection"), "");
	}
	
	bool ListItemEnabled() { return (m_Data.Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM; }
	
	bool ObjectMarkerEnabled() { return (m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER; }
	void GetObjectMarkerPos(out float x, out float y)
	{
		if (ObjectMarkerEnabled()) {
			m_EditorObjectMarkerWidget.GetPos(x, y);
		} else {
			x = -1; y = -1;
		}
	}
	
	bool MapMarkerEnabled() { return (m_Data.Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER; }
	void GetMapMarkerPos(out float x, out float y)
	{
		if (MapMarkerEnabled()) {
			m_EditorMapMarkerWidget.GetPos(x, y);
		} else {
			x = -1; y = -1;
		}
	}
	
	bool BoundingBoxEnabled() { return (m_Data.Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX; }
	
	bool IsRootSelected(Widget root)
	{
		bool result;		
		if (ListItemEnabled()) {
			if (m_EditorObjectBrowserWidget == root)
				return true;
		}
		if (ObjectMarkerEnabled()) {
			if (m_EditorObjectMarkerWidget == root)
				return true;
		}
		
		return false;
	}
	

	
}






