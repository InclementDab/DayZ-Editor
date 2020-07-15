


static vector GetObjectSize(Object obj)
{
	vector size[2];
	vector result;
	
	float radius = obj.ClippingInfo(size);
	
	result[0] = Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]);
	result[1] = Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]);
	result[2] = Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]);
	return result;
}

static vector MousePosToRay(out set<Object> collisions = null, Object ignore = null, float raycast_distance = 1000)
{
	vector rayStart = GetGame().GetCurrentCameraPosition();
	vector rayEnd = rayStart + GetGame().GetPointerDirection() * raycast_distance;
	vector hitPos, hitNormal;
	int hitComponentIndex;		
	collisions = new set<Object>;

	DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, collisions, NULL, ignore, false, false, 1, 0, CollisionFlags.FIRSTCONTACT);
	return hitPos;
}


static EditorObject GetBrowserObjectFromEntity(Object obj)
{
	foreach (EditorObject list_item: Editor.EditorPlacedObjects) {
		if (obj == list_item.WorldObject) {
			return list_item;
		}
	}
	return null;
}	

static vector AverageVectors(vector v1, vector v2)
{
	vector result;
	
	result[0] = (v1[0] + v2[0])/2;
	result[1] = (v1[1] + v2[1])/2;
	result[2] = (v1[2] + v2[2])/2;
	
	return result;
}

static vector DivideVectorByNumber(vector v1, float v2)
{
	vector result;
	
	result[0] = v1[0]/2;
	result[1] = v1[1]/2;
	result[2] = v1[2]/2;
	
	return result;
}


static vector DivideVector(vector v1, vector v2)
{
	vector result;
	
	result[0] = v1[0]/v2[0];
	result[1] = v1[1]/v2[1];
	result[2] = v1[2]/v2[2];
	
	return result;
}




class EditorObject1
{
	Object WorldObject;
	BoundingBox WorldBoundingBox;
	
	bool IsSelected = false;
	
	void EditorObject1(notnull Object world_object) 
	{
		WorldObject = world_object;
	}
	
	
	void OnSelected()
	{
		// createbbox
	}
	
}


class Editor: Managed
{
	private ref UIManager 					m_UIManager;
	static ref EditorUI						m_EditorUI;	
	
	
	PlayerBase 								m_PlayerBase;
	EditorCamera							m_Camera;
 	
				
	static ref Hologram 					ObjectInHand;
	static Object							ObjectUnderCursor;
	static ref Cartesian					ActiveCartesian;
	static Object							ActiveBoundingBox;
	
	static ref set<Object>					SelectedObjects;
	static ref array<ref BoundingBox>		ActiveBoundingBoxes;
	
	static ref array<Object>				PlacedObjects;
	static ref array<string>				EditorListObjects;
	
	
	static ref set<EditorObject> 		EditorObjects;
	
	static ref array<EditorListItem>	EditorListItems;
	
	void Editor()
	{
		Print("Editor");		
		m_UIManager = GetGame().GetUIManager();
		
		EditorPlacedObjects 		= new array<EditorObject>;
		PlacedObjects 				= new array<Object>;
		EditorListObjects 			= new array<string>;
		EditorListItems 			= new array<EditorListItem>;
		SelectedObjects				= new set<Object>();
		ActiveBoundingBoxes			= new array<ref BoundingBox>;
		
		float pos1 = Math.RandomFloat(2500, 13000);
		float pos2 = Math.RandomFloat(2500, 13000);
		
		m_Camera = GetGame().CreateObject("EditorCamera", Vector(pos1, 500, pos2), false);
		m_Camera.SetActive(true);
		
		m_EditorUI = new EditorUI();
		m_UIManager.ShowScriptedMenu(m_EditorUI, m_UIManager.GetMenu());
		ShowUI(true);
		
		LoadEditorObjects();
		LoadPlacedObjects();
		
		m_PlayerBase = GetGame().GetPlayer();
		if (m_PlayerBase)
			m_PlayerBase.GetInputController().SetDisabled(true);

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
		
	void ~Editor()
	{
		Print("~Editor");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	
	void ShowUI(bool state)
	{ 
		m_EditorUI.Show(state);
		if (!state) delete ObjectInHand;
		m_Camera.LookFreeze = state;
		m_UIManager.ShowUICursor(state);
	}
	
	void LoadEditorObjects()
	{
		Print("Editor::LoadEditorObjects");
		EditorListObjects = new array<string>;
		
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		
		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < g_Game.ConfigGetChildrenCount(Config_Path); j++) {
		        
				string Config_Name, Base_Name;
		        GetGame().ConfigGetChildName(Config_Path, j, Config_Name);
		        GetGame().ConfigGetBaseName(Config_Path + " " + Config_Name, Base_Name);
		        Base_Name.ToLower();
		
		        if (Base_Name != "housenodestruct")
		            continue;
			        
				EditorListObjects.Insert(Config_Name);	
				
				EditorListItem editor_list_item;
				Widget list_item = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItem.layout", m_EditorUI.m_LeftListPanelSpacer);
				list_item.GetScript(editor_list_item);
				editor_list_item.SetObject(Config_Name);
				EditorListItems.Insert(editor_list_item);
				
		    }
		}
	}
	
	
	bool exit_condition = false;
	
	void Update() 
	{
		set<Object> obj;
		if (IsPlacing()) {
			vector v = MousePosToRay(obj, ObjectInHand.GetProjectionEntity());
			vector size = GetObjectSize(ObjectInHand.GetProjectionEntity());
			v[1] = v[1] + size[1]/2;
			ObjectInHand.SetProjectionPosition(v);
			ObjectInHand.SetProjectionOrientation(GetGame().SurfaceGetNormal(v[0], v[2]));
			
		} else {
			int x, y;
			GetMousePos(x, y);
			v = MousePosToRay(obj);
			Object target = obj.Get(0);
			
			if (target != null && target != ObjectUnderCursor) {
				exit_condition = false;				
				SetObjectUnderCursor(target);
				OnMouseEnterObject(target, x, y);					
				
			} else if (target == null && !exit_condition) { 
				SetObjectUnderCursor(null);
				exit_condition = OnMouseExitObject(x, y);
				
			}
			
		}
	}
	
	
	static bool CursorAllowedToSelect = true;
	bool OnMouseEnterObject(Object target, int x, int y)
	{
		//Print("OnMouseEnterObject");
		if (!CursorAllowedToSelect) return false;
		if (GetGame().GetInput().LocalHold("UAFire")) return false;
		if (GetGame().GetInput().LocalHold("UATurbo")) {
			CreateSelection(target, false);
		}
		
		return true;
	}
	
	bool OnMouseExitObject(int x, int y)
	{
		if (GetGame().GetInput().LocalHold("UAFire")) return false;
		
		return true;
	}
	
	static void OnMouseButtonPress(int button)
	{
		Print("Editor::OnMouseButtonPress");
		Input input = GetGame().GetInput();
		if (button == 0) {
			if (ObjectUnderCursor == null) ClearSelections();
			else if (SelectedObjects.Find(ObjectUnderCursor) == -1) {
				if (input.LocalValue("UATurbo")) {
					CreateSelection(ObjectUnderCursor, false);
				} else {
					CreateSelection(ObjectUnderCursor);
				}
			}
		}
	}
	
	static void CreateObjectInHand(string name)
	{
		EntityAI obj = GetGame().CreateObject(name, "0 0 0");
		set<Object> o;
		vector v =  MousePosToRay(o);
		vector size = GetObjectSize(obj);
		
		
		v[1] = v[1] + size[1]/2;
		ObjectInHand = new Hologram(null, v, obj);
	}
	
	static Object PlaceObject(string name, vector position, vector orientation)
	{
		Print("Editor::PlaceObject");
		Object target = GetGame().CreateObjectEx(name, position, ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN|ECE_TRACE);
		target.SetOrientation(orientation);		
		PlacedObjects.Insert(target);		
		CreateSelection(target);
		m_EditorUI.OnObjectPlaced(target, position, orientation);
		
		return target;
	}
	

	
	
	static void CreateHighlight(notnull Object target)
	{
		CreateSelection(target);
	}
	
	static void CreateSelection(notnull Object target, bool remove_old = true)
	{
		
		if (remove_old) ClearSelections();
		
		int c = SelectedObjects.Count();
		if (SelectedObjects.Insert(target) == c) {
			BoundingBox bbox = new BoundingBox(target);
			Editor.ActiveBoundingBoxes.Insert(bbox);			
		}
		
		//m_EditorUI.monitor.SetBlood(Editor.SelectedObjects.Count());
	}
	
	static void ClearSelections()
	{
		SelectedObjects.Clear();
		ActiveBoundingBoxes.Clear();
		//m_EditorUI.monitor.SetBlood(Editor.SelectedObjects.Count());
	}
	
	static void RemoveSelection(Object target)
	{
		int index = SelectedObjects.Find(target);
		if (index == -1) return;
			
		Object bounding_box = target.GetChildren();
		if (bounding_box != null) {
			//ActiveBoundingBoxes.RemoveItem(bounding_box); ah shit here we go again
		}
		SelectedObjects.Remove(index);
			
		
	}
	
	static void DeleteObject(EditorObject target)
	{
		GetGame().ObjectDelete(target.WorldObject);
		delete target;		
	}
	
	
	bool ui_state = true;
	void OnKeyPress(int key) 
	{
		switch (key)
		{
			case KeyCode.KC_SPACE:
				ui_state = !ui_state;
				ShowUI(ui_state);
				break;
			
			case KeyCode.KC_DELETE:
				foreach (EditorObject selected_object: EditorPlacedObjects)
					DeleteObject(selected_object);
				break;
			
						
			case KeyCode.KC_ESCAPE:
				if (ui_state) {
					ui_state = false;	
					m_EditorUI.Show(ui_state);
				} else {
					// Pause menu	
				}

				break;
			
			case KeyCode.KC_RIGHT:
				break;
		}
	}
	
	
	static bool IsPlacing()
	{
		return ObjectInHand != null;
	}
	
	static void SetObjectUnderCursor(Object target)
	{
		ObjectUnderCursor = target;
	}
	
	static void LoadPlacedObjects()
	{
		// todo: allow people to open their own savefiles
		PlacedObjects = new array<Object>;
	}
}



class BoundingBox
{
	EntityAI m_BBoxLines[12];	
	EntityAI m_BBoxBase;
	
	void BoundingBox(notnull Object target)
	{
		Print("BoundingBox");
		
		vector line_centers[12]; vector line_verticies[8];
		vector clip_info[2];
		float radius = target.ClippingInfo(clip_info); // idk do something cool w/ radius		
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
				
	
		float line_width = 0.1;
		for (int i = 0; i < 12; i++) {
			
			vector size = GetObjectSize(target);
			vector transform[4];			
			transform[3] = line_centers[i];
			
			for (int j = 0; j < 3; j++) {
				transform[j][j] = ((position[j] == line_centers[i][j])*size[j]/2) + line_width;
			}
						
			 
			m_BBoxLines[i] = GetGame().CreateObjectEx("BoundingBoxBase", line_centers[i], ECE_NONE);
			m_BBoxLines[i].SetTransform(transform);
		
			target.AddChild(m_BBoxLines[i], -1);
		}
		
		//m_BBoxBase = GetGame().CreateObjectEx("BoundingBoxBase", vector.Zero, ECE_NONE);
		
		target.Update();
	}
	
	void ~BoundingBox()
	{
		Print("~BoundingBox");
		
		for (int i = 0; i < 12; i++) {
			GetGame().ObjectDelete(m_BBoxLines[i]);
			m_BBoxLines[i].Update();
		}	
	}	
	
}








