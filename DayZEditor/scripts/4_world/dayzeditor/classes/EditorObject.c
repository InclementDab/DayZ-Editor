typedef ref map<int, ref EditorObject> EditorObjectSet;


class EditorObject : Building
{
	private bool IsInitialized = false;
	private bool IsSelected = false;
	
	protected Object 		m_WorldObject;
	
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
	
	void EditorObject()
	{
		Print("EditorObject");
	}
	
	void ~EditorObject()
	{
		Print("~EditorObject");
		
	}
	
	
	override void EEDelete(EntityAI parent)
	{
		Print("EditorObject::EEDelete");
		Deselect();
		
		IsInitialized = false;
		
		g_Game.ObjectDelete(m_WorldObject);
		g_Game.ObjectDelete(m_BBoxBase);
		g_Game.ObjectDelete(m_CenterLine);
		
	
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
			g_Game.ObjectDelete(m_BBoxLines[i]);
		
		super.EEDelete(parent);
	}
	

	void Init(string type_name)
	{
		Print("EditorObject::Init");
	
		IsInitialized = true;
		Type = type_name;
		m_WorldObject = g_Game.CreateObjectEx(type_name, vector.Zero, ECE_LOCAL | ECE_SETUP | ECE_CREATEPHYSICS);
		AddChild(m_WorldObject, -1);
		Update();
		
		// todo move all this to EditorUIManager
		
		// World Object base marker
		m_EditorObjectMarker = new UILinkedObject();
		m_EditorObjectMarkerWidget = g_Game.GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		m_EditorObjectMarkerWidget.GetScript(m_EditorObjectMarker);
		m_EditorObjectMarker.SetObject(this);
				
		// Map marker
		m_EditorMapMarker = new UILinkedObject();
		m_EditorMapMarkerWidget = g_Game.GetWorkspace().CreateWidgets(layout_dir + "EditorMapMarker.layout");
		m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
		m_EditorMapMarker.SetObject(this);
		GetEditor().GetUIManager().GetEditorUI().GetMapWidget().AddChild(m_EditorMapMarkerWidget);
	
		// Browser item
		m_EditorObjectBrowser = new UILinkedObject();
		m_EditorObjectBrowserWidget = g_Game.GetWorkspace().CreateWidgets(layout_dir + "EditorPlacedListItem.layout");
		m_EditorObjectBrowserWidget.GetScript(m_EditorObjectBrowser);
		m_EditorObjectBrowser.SetObject(this);		
		
		// Properties Dialog
		m_EditorObjectPropertiesWindow = new UILinkedObject();
		m_EditorObjectPropertiesWidget = g_Game.GetWorkspace().CreateWidgets(layout_dir + "dialogs/EditorObjectProperties.layout");
		m_EditorObjectPropertiesWidget.GetScript(m_EditorObjectPropertiesWindow);
		m_EditorObjectPropertiesWindow.SetObject(this);
		m_EditorObjectPropertiesWidget.Show(false);		
		
		// Context Menu
		m_EditorObjectContextMenu = new UILinkedObject();
		m_EditorObjectContextWidget = g_Game.GetWorkspace().CreateWidgets(layout_dir + "EditorContextMenu.layout");
		m_EditorObjectContextWidget.GetScript(m_EditorObjectContextMenu);
		m_EditorObjectContextMenu.SetObject(this);
		m_EditorObjectContextWidget.Show(false);
		
		
		CreateBoundingBox();
		
	}
	
	
	
	static EditorObject CreateFromExistingObject(notnull Object target)	
	{
		// todo
	}
	
	
	
	
	private string Type;
	override string GetType()
	{
		return Type;
	}
	
	override string GetDisplayName()
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
		Print("EditorObject::CreateBoundingBox");
		
		vector clip_info[2];
		vector size = GetSize();
		

		float radius = m_WorldObject.ClippingInfo(clip_info); // idk do something cool w/ radius	
		
		//vector mins, maxs;
		//m_WorldObject.GetWorldBounds(mins, maxs);
		//clip_info[0] = mins - GetPosition();
		//clip_info[1] = maxs - GetPosition();
	
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
			 
			m_BBoxLines[i] = g_Game.CreateObjectEx("BoundingBoxBase", line_centers[i], ECE_NONE);
			m_BBoxLines[i].SetTransform(transform);			
			
			AddChild(m_BBoxLines[i], -1);
		}
		
		
		vector y_axis_mat[4];
		vector bottom_center = GetBottomCenter() - GetPosition();
		y_axis_mat[0][0] = line_width;
		y_axis_mat[1][1] = 1000;
		y_axis_mat[2][2] = line_width;
		y_axis_mat[3] = Vector(bottom_center[0], bottom_center[1] - y_axis_mat[1][1], bottom_center[2]);
		
		m_CenterLine = g_Game.CreateObjectEx("BoundingBoxBase", bottom_center, ECE_NONE);
		m_CenterLine.SetTransform(y_axis_mat);
		AddChild(m_CenterLine, -1);
		
		HideBoundingBox();
		Update();
	}
	
	

	bool OnMouseEnter(int x, int y)
	{
		return true;
	}
	
	bool OnMouseLeave(int x, int y)
	{
		return true;
	}
	

	void ToggleSelect()
	{
		if (IsSelected) Deselect();
		else Select();
	}
	
	void Select(bool clear_existing = true)
	{
		//Print("EditorObject::Select");
		
		if (clear_existing)
			GetEditor().GetObjectManager().ClearSelection();
		
		if (IsSelected) return;
		IsSelected = true;
		ShowBoundingBox();
		
		vector size = GetSize();
		vector position = vector.Zero;
		position[1] = position[1] + size[1]/2;
		//Editor.SelectedObjects.Insert(GetID(), this);
		//EditorEvents.ObjectSelectedInvoke(this, this);
		
		
	}
	
	void Deselect()
	{
		//Print("EditorObject::Deselect");
		if (!IsSelected) return;
		IsSelected = false;
		HideBoundingBox();
		
		//Editor.SelectedObjects.Remove(GetID());
		//EditorEvents.ObjectDeselectedInvoke(this, this);
	}

	vector GetBottomCenter()
	{
		if (!IsInitialized) return vector.Zero;
		
		vector clip_info[2];
		m_WorldObject.ClippingInfo(clip_info);
		//clip_info[0][1] = -clip_info[1][1];
		vector result;
		vector up = GetTransformAxis(1);
		result = up * -(vector.Distance(Vector(0, clip_info[0][1], 0), Vector(0, clip_info[1][1], 0)) / 2);
		result += GetPosition();
		return result;
	}
	
	vector GetTopCenter()
	{
		if (!IsInitialized) return vector.Zero;
		
		vector clip_info[2];
		m_WorldObject.ClippingInfo(clip_info);
		//clip_info[0][1] = -clip_info[1][1];
		vector result;
		vector up = GetTransformAxis(1);
		result = up * (vector.Distance(Vector(0, clip_info[0][1], 0), Vector(0, clip_info[1][1], 0)) / 2);
		result += GetPosition();
		return result;
	}
		
	ref Param4<vector, vector, vector, vector> TransformBeforeDrag;
	Param4<vector, vector, vector, vector> GetTransformArray()
	{
		vector mat[4];
		GetTransform(mat);
		return new Param4<vector, vector, vector, vector>(mat[0], mat[1], mat[2], mat[3]);
	}

	void SetTransformArray(Param4<vector, vector, vector, vector> param)
	{
		vector mat[4];
		
		mat[0] = param.param1; mat[1] = param.param2; mat[2] = param.param3; mat[3] = param.param4;
		SetTransform(mat);
		Update();
	}
	

	vector GetSize()
	{
		vector clip_info[2];
		vector result;

		m_WorldObject.ClippingInfo(clip_info);
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
		if (!IsInitialized || BoundingBoxVisible) return;
		Print("EditorObject::ShowBoundingBox");
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
		if (!IsInitialized || !BoundingBoxVisible) return;
		Print("EditorObject::HideBoundingBox");
		BoundingBoxVisible = false;
		for (int i = 0; i < 12; i++) {
			m_BBoxLines[i].SetObjectTexture(m_BBoxLines[i].GetHiddenSelectionIndex("BoundingBoxSelection"), "");
			m_BBoxLines[i].Update();
		}
		
		m_CenterLine.SetObjectTexture(m_CenterLine.GetHiddenSelectionIndex("BoundingBoxSelection"), "");
	}
	
	
	
	bool IsSelected() {	return IsSelected;	}

	
	Object GetObject()
	{
		return m_WorldObject;
	}
	
	vector GetMarkerPosition()
	{
		float x, y;
		m_EditorObjectMarker.GetLayoutRoot().GetPos(x, y);
		return Vector(x, y, 0);
	}
	
	Widget GetObjectMarker() { return m_EditorObjectMarkerWidget; }
	Widget GetObjectBrowser() { return m_EditorObjectBrowserWidget; }
	Widget GetMapMarker() { return m_EditorMapMarkerWidget; }
	Widget GetContextMenu() { return m_EditorObjectContextWidget; }
	
	UILinkedObject GetEditorObjectMarker() { return m_EditorObjectMarker; }
	
	void ShowPropertiesWindow(bool show) 
	{
		m_EditorObjectPropertiesWidget.Show(true);
		m_EditorObjectPropertiesWidget.Update();
		//SetModal(m_EditorObjectPropertiesWidget);
	}
	
	static EditorObject GetFromUILinkedRoot(Widget root)
	{
		Print("EditorObject::GetFromObjectRoot");
		foreach (EditorObject editor_object: GetEditor().GetObjectManager().GetPlacedObjects()) {
			if (editor_object.GetObjectBrowser() == root || editor_object.GetObjectMarker() == root)
				return editor_object;
		}
		
		Print("GetFromObjectRoot: Item Not Found!");
		return null;
	}

	
	

}



