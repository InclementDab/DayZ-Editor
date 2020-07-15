


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

	DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, collisions, NULL, ignore, false, false, 1, 5, CollisionFlags.FIRSTCONTACT);
	return hitPos;
}


static EditorObject GetBrowserObjectFromEntity(Object obj)
{
	foreach (EditorObject list_item: Editor.EditorPlacedObjects) {
		if (obj == list_item.GetWorldObject()) {
			return list_item;
		}
	}
	return null;
}	


class Editor: Managed
{
	private ref UIManager 					m_UIManager;
	static ref EditorUI						m_EditorUI;	
	
	
	PlayerBase 								m_PlayerBase;
	EditorCamera							m_Camera;
 	
				
	static ref Hologram 					ObjectInHand;
	static Object							ObjectUnderCursor;
	static Object							SelectedObject; // todo: boink
	static ref Cartesian					ActiveCartesian;
	static Object							ActiveBoundingBox;
	
	static ref array<ref Object>			SelectedObjects;
	static ref array<ref BoundingBox>		ActiveBoundingBoxes;
	
	static ref array<ref Object>			PlacedObjects;
	static ref array<string>				EditorListObjects;
	
	
	static ref array<ref EditorObject> 		EditorPlacedObjects;
	static ref array<ref EditorListItem>	EditorListItems;
	
	void Editor()
	{
		Print("Editor");		
		m_UIManager = GetGame().GetUIManager();
		
		EditorPlacedObjects 		= new array<ref EditorObject>;
		PlacedObjects 				= new array<ref Object>;
		EditorListObjects 			= new array<string>;
		EditorListItems 			= new array<ref EditorListItem>;
		SelectedObjects				= new array<ref Object>;
		ActiveBoundingBoxes			= new array<ref BoundingBox>;
		
		m_Camera = GetGame().CreateObject("EditorCamera", "7500 500 7500", false);
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
		CreateHighlight(target);
		return true;
	}
	
	bool OnMouseExitObject(int x, int y)
	{
		//Print("OnMouseExitObject");
		if (GetGame().GetInput().LocalHold("UAFire")) return false;
		//ActiveBoundingBoxes = new array<ref BoundingBox>;
		
		return true;
	}
	
	void OnMouseButtonPress(int button)
	{
		Input input = GetGame().GetInput();
		if (ObjectUnderCursor != null) {
			if (input.LocalValue("UATurbo")) {
				CreateSelection(ObjectUnderCursor, false);
			} else {
				CreateSelection(ObjectUnderCursor);
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
		SelectObject(target);
		m_EditorUI.OnObjectPlaced(target, position, orientation);
		
		return target;
	}
	
	static void ClearActiveObject()
	{
		Print("Editor::ClearActiveObject");
		delete ActiveCartesian;
		SelectedObject = null;
	}
	
	static void SelectObject(notnull Object obj)
	{
		Print("Editor::SelectObject: " + obj);
		
		EditorUI editor_ui = EditorUI.GetInstance(); // scuff
		SelectedObject = obj;
		EditorObject browser_item = GetBrowserObjectFromEntity(obj);
		if (browser_item)
			SetFocus(browser_item.GetLayoutRoot());
		else 
			editor_ui.CreateEditorObjectFromExisting(obj);
	}
	
	
	static void CreateHighlight(ref Object target)
	{
		CreateSelection(target);
	}
	
	static void CreateSelection(ref Object target, bool remove_old = true)
	{
		if (remove_old) {
 			delete SelectedObjects; delete ActiveBoundingBoxes;
			SelectedObjects = new array<ref Object>;
			ActiveBoundingBoxes = new array<ref BoundingBox>;
		}
		
		SelectedObjects.Insert(target);
		ActiveBoundingBoxes.Insert(new BoundingBox(target));
	}
	
	static void CreateSelections(ref array<ref Object> target, bool remove_old = true)
	{
		if (remove_old) {
 			delete SelectedObjects; delete ActiveBoundingBoxes;
			SelectedObjects = new array<ref Object>;
			ActiveBoundingBoxes = new array<ref BoundingBox>;	
		}
		
		foreach (ref Object obj: target) {
			SelectedObjects.Insert(obj);
		}
		
		ref array<ref BoundingBox> bboxes = CreateBoundingBoxes(target);
		
		foreach (ref BoundingBox bbox: bboxes) {
			ActiveBoundingBoxes.Insert(bbox);
		}
		

		
	}
	
	static void ClearSelections()
	{
		delete SelectedObjects; delete ActiveBoundingBoxes;
	}
	
	static void DeleteObject(Object obj)
	{
		EditorObject browser_item = GetBrowserObjectFromEntity(obj);
		GetGame().ObjectDelete(obj);
		
		if (browser_item)
			delete browser_item;		
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
				DeleteObject(SelectedObject);
				//RemoveBoundingBox();
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
		PlacedObjects = new array<ref Object>;
	}
}


static ref array<ref BoundingBox> CreateBoundingBoxes(ref array<ref Object> targets)
{
	ref array<ref BoundingBox> bounding_boxes = new array<ref BoundingBox>;
	BoundingBox b;
	foreach (ref Object obj: targets) {
		b = new BoundingBox(obj);
		bounding_boxes.Insert(b);
	}
	return bounding_boxes;
}

class BoundingBox
{
	private Object m_Object;
	
	void BoundingBox(ref notnull Object target)
	{		
		EntityAI bounding_box = GetGame().CreateObjectEx("BoundingBoxBase", vector.Zero, ECE_CREATEPHYSICS);
		
		target.AddChild(bounding_box, -1);
		
		vector position = vector.Zero;
		vector size = GetObjectSize(target);
		vector transform[4] =
		{ 
            "1 0 0 0"
            "0 1 0 0" 
            "0 0 1 0"
            "0 0 0 1"
		};
		
		transform[0][0] = size[0]/2;
		transform[1][1] = size[1]/2;
		transform[2][2] = size[2]/2;

        transform[3][0] = position[0];
        transform[3][1] = position[1];
        transform[3][2] = position[2];
        transform[3][3] = 1.0;
		
        bounding_box.SetTransform(transform);
		
		m_Object = bounding_box;
		target.Update();
		
	}
	
	void ~BoundingBox()
	{
		GetGame().ObjectDelete(m_Object);
	}
	
}



