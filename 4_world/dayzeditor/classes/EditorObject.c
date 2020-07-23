



class EditorObjectLink: Link<EditorObject>
{
	
	
	void EditorObjectLink(EditorObject init)
	{
		Print("EditorObjectLink");
		EditorAction action = new EditorAction("Delete", "Create");
		action.InsertUndoParameter(this, null);
		action.InsertRedoParameter(this, null);
		Editor.InsertAction(action);
		
		
	}
	
	void ~EditorObjectLink()
	{
		Print("~EditorObjectLink");
		
		EditorAction action = new EditorAction("Create", "Delete");
		action.InsertUndoParameter(this, null);
		action.InsertRedoParameter(this, null);
		Editor.InsertAction(action);
	}
	
	void Create()
	{
		Print("EditorObjectLink::Create");
	}
	
	void Delete()
	{
		Print("EditorObjectLink::Delete");
	}
}




class EditorObject : BuildingBase
{
	private bool IsInitialized = false;
	private bool IsSelected = false;
	private string bbox_texture, bbox_material;
	
	protected Object m_WorldObject;
	
	protected Widget 		m_EditorObjectMarkerWidget;
	protected Widget 		m_EditorObjectBrowserWidget;
	protected Widget 		m_EditorMapMarkerWidget;
	
	ref UILinkedObject 		m_EditorObjectMarker = null;
	ref UILinkedObject 		m_EditorObjectBrowser = null;
	ref UILinkedObject		m_EditorMapMarker = null;
	
	protected EntityAI m_BBoxLines[12];	
	
	protected EntityAI m_BBoxBase;
	
	protected EntityAI m_CenterLine;
	
	
	
	void EditorObject()
	{
		Print("EditorObject");
		SetEventMask(EntityEvent.FRAME);
	}
	
	void ~EditorObject()
	{
		Print("~EditorObject");
	}
	

	
	
	static EditorObject CreateFromExistingObject(notnull Object target)	
	{
		// todo
	}
	
	
	private string Type;
	override string GetType()
	{
		//Print("EditorObject::GetType");
		return Type;
	}
	
	override string GetDisplayName()
	{
		//Print("EditorObject::GetDisplayName");
		string name = m_WorldObject.GetDisplayName();
		if (name != string.Empty)
			return name;
		else return GetType();
	}

	
	void CreateBoundingBox()
	{
		Print("EditorObject::CreateBoundingBox");
		vector line_centers[12]; vector line_verticies[8];
		vector clip_info[2];
		vector size = GetSize();
		
		float radius = m_WorldObject.ClippingInfo(clip_info); // idk do something cool w/ radius
		
			
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
				
	
		float line_width = 0.01;
		for (int i = 0; i < 12; i++) {
			
			vector transform[4];			
			transform[3] = line_centers[i];
			
			for (int j = 0; j < 3; j++) {
				transform[j][j] = ((position[j] == line_centers[i][j])*size[j]/2) + line_width;
			}
						
			 
			m_BBoxLines[i] = GetGame().CreateObjectEx("BoundingBoxBase", line_centers[i], ECE_NONE);
			m_BBoxLines[i].SetTransform(transform);			
			
			AddChild(m_BBoxLines[i], -1);
			
		}
		
		
		vector y_axis_mat[4];
		vector bottom_center;
		y_axis_mat[0][0] = line_width;
		y_axis_mat[1][1] = 1000;
		y_axis_mat[2][2] = line_width;
		y_axis_mat[3] = Vector(bottom_center[0], bottom_center[1] - y_axis_mat[1][1] - size[1]/2, bottom_center[2]);
		
		m_CenterLine = GetGame().CreateObjectEx("BoundingBoxBase", bottom_center, ECE_NONE);

		m_CenterLine.SetTransform(y_axis_mat);
		AddChild(m_CenterLine, -1);
		
		
		vector base_mat[4];
		base_mat[3] = GetBottomCenter();
		//m_BBoxBase = GetGame().CreateObjectEx("BoundingBoxBase", vector.Zero, ECE_NONE);
		base_mat[0][0] = size[0]/2;
		base_mat[1][1] = line_width;
		base_mat[2][2] = size[2]/2;
		//m_BBoxBase.SetTransform(base_mat);
		//WorldObject.AddChild(m_BBoxBase, -1);
		
		TStringArray textures = m_BBoxLines[0].GetHiddenSelectionsTextures();
		TStringArray materials = m_BBoxLines[0].GetHiddenSelectionsMaterials();
		bbox_texture = textures[0]; bbox_material = materials[0];
		
		HideBoundingBox();
		Update();
	}
	
	
	override void EEInit()
	{
		super.EEInit();
		IsInitialized = true;
				
		
		m_EditorObjectMarkerWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		m_EditorObjectMarkerWidget.GetScript(m_EditorObjectMarker);
		
		m_EditorMapMarkerWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorMapMarker.layout");
		m_EditorMapMarkerWidget.GetScript(m_EditorMapMarker);
	
		m_EditorObjectBrowserWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorPlacedListItem.layout");
		m_EditorObjectBrowserWidget.GetScript(m_EditorObjectBrowser);
	}
	
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		Deselect();
		
		IsInitialized = false;
		
		GetGame().ObjectDelete(m_WorldObject);
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);
		
	
		delete m_EditorObjectMarker; delete m_EditorObjectBrowser;
		delete m_EditorMapMarker;
		
		for (int i = 0; i < 12; i++)
			GetGame().ObjectDelete(m_BBoxLines[i]);
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
			Editor.ClearSelections();
		
		if (IsSelected) return;
		IsSelected = true;
		ShowBoundingBox();
		
		vector size = GetSize();
		vector position = vector.Zero;
		position[1] = position[1] + size[1]/2;
		
		Editor.GlobalTranslationWidget = GetGame().CreateObjectEx("TranslationWidget", position, ECE_SETUP);
		Editor.GlobalTranslationWidget.SetEditorObject(this, position);

		
		ObjectSelectedEventArgs args(this, IsSelected);
		EditorEvents.ObjectSelectedInvoke(this, args);
		
		Editor.SelectedObjects.Insert(GetID(), this);
	}
	
	void Deselect()
	{
		//Print("EditorObject::Deselect");
		if (!IsSelected) return;
		IsSelected = false;
		HideBoundingBox();
		
		GetGame().ObjectDelete(Editor.GlobalTranslationWidget);
		
		ObjectSelectedEventArgs args(this, IsSelected);
		EditorEvents.ObjectSelectedInvoke(this, args);
		
		Editor.SelectedObjects.Remove(GetID());
	}

	
	vector GetBottomCenter()
	{
		if (!IsInitialized) return vector.Zero;
		vector clip_info[2];
		m_WorldObject.ClippingInfo(clip_info);
		vector result = Vector((clip_info[0][0] + clip_info[1][0]) / 2, clip_info[0][1], (clip_info[0][2] + clip_info[1][2]) / 2);
		result += m_WorldObject.GetPosition();
		
		return result;
	}
	
	

	
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
		vector size[2];
		vector result;

		m_WorldObject.ClippingInfo(size);		
		result[0] = Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]);
		result[1] = Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]);
		result[2] = Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]);
		
		return result;
	}
	
	ref Param4<vector, vector, vector, vector> TransformBeforeDrag;

	vector DragOffset;
		
	bool OnDrag()
	{
		Print("EditorObject::OnDrag");
		
		foreach (ref EditorObject editor_object: Editor.SelectedObjects) {
			vector mat[4];
			editor_object.GetTransform(mat);
			editor_object.TransformBeforeDrag = new Param4<vector, vector, vector, vector>(mat[0], mat[1], mat[2], mat[3]);
		}
		
		vector position = GetPosition();
		//GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);
		return true;
	}
	
	bool OnDrop()
	{
		Print("EditorObject::OnDrop");
		
		//GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		EditorAction action = new EditorAction("SetTransformArray", "SetTransformArray");

		foreach (ref EditorObject editor_object: Editor.SelectedObjects) {
			action.InsertUndoParameter(editor_object, editor_object.TransformBeforeDrag);
			action.InsertRedoParameter(editor_object, editor_object.GetTransformArray());
		}
		
		Editor.InsertAction(action);
		return true;
	}
	
		
	
	

	bool BoundingBoxVisible;
	void ShowBoundingBox()
	{
		if (!IsInitialized) return;
		if (BoundingBoxVisible) return;
		Print("EditorObject::ShowBoundingBox");
		BoundingBoxVisible = true;
		for (int i = 0; i < 12; i++) {
			m_BBoxLines[i].SetObjectTexture(m_BBoxLines[i].GetHiddenSelectionIndex("MainSection1"), bbox_texture);
			m_BBoxLines[i].SetObjectMaterial(m_BBoxLines[i].GetHiddenSelectionIndex("MainSection1"), bbox_material);
			m_BBoxLines[i].Update();
		}
		
		m_CenterLine.SetObjectTexture(m_CenterLine.GetHiddenSelectionIndex("MainSection1"), bbox_texture);
		m_CenterLine.SetObjectMaterial(m_CenterLine.GetHiddenSelectionIndex("MainSection1"), bbox_material);
		
		//m_BBoxBase.SetObjectTexture(m_BBoxBase.GetHiddenSelectionIndex("MainSection1"), bbox_texture);
		//m_BBoxBase.SetObjectMaterial(m_BBoxBase.GetHiddenSelectionIndex("MainSection1"), bbox_material);
		
	}
	
	void HideBoundingBox()
	{
		if (!IsInitialized) return;
		if (!BoundingBoxVisible) return;
		Print("EditorObject::HideBoundingBox");
		BoundingBoxVisible = false;
		for (int i = 0; i < 12; i++) {
			m_BBoxLines[i].SetObjectTexture(m_BBoxLines[i].GetHiddenSelectionIndex("MainSection1"), "");
			m_BBoxLines[i].SetObjectMaterial(m_BBoxLines[i].GetHiddenSelectionIndex("MainSection1"), "");
			m_BBoxLines[i].Update();
		}	
		
		m_CenterLine.SetObjectTexture(m_CenterLine.GetHiddenSelectionIndex("MainSection1"), "");
		m_CenterLine.SetObjectMaterial(m_CenterLine.GetHiddenSelectionIndex("MainSection1"), "");
		
		//m_BBoxBase.SetObjectTexture(m_BBoxBase.GetHiddenSelectionIndex("MainSection1"), "");
		//m_BBoxBase.SetObjectMaterial(m_BBoxBase.GetHiddenSelectionIndex("MainSection1"), "");
	}
	
	
	
	bool IsSelected() {	return IsSelected;	}
	
	void SetObject(string name)
	{
		Print("EditorObject::SetObject");
		Type = name;
		m_WorldObject = GetGame().CreateObjectEx(name, vector.Zero, ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN|ECE_TRACE);
		AddChild(m_WorldObject, -1);
		Update();
		
		m_EditorObjectMarker.SetObject(this);
		m_EditorObjectBrowser.SetObject(this);
		m_EditorMapMarker.SetObject(this);
	}
	
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
	
	static EditorObject GetFromUILinkedRoot(Widget root)
	{
		Print("EditorObject::GetFromObjectRoot");
		foreach (EditorObject editor_object: Editor.EditorObjects) {
			if (editor_object.GetObjectBrowser() == root || editor_object.GetObjectMarker() == root)
				return editor_object;
		}
		
		Print("GetFromObjectRoot: Item Not Found!");
		return null;
	}
	
	static bool CheckIfRootIsSelected(Widget root)
	{
		foreach (EditorObject editor_object: Editor.SelectedObjects) {
			if (editor_object.IsSelected() && (editor_object.GetObjectBrowser() == root || editor_object.GetObjectMarker() == root))
				return true;
		}
		return false;
	}
	
	

}



