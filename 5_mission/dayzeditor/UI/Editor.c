
class Cartesian
{
	static EntityAI CartesianObject;
	
	void Cartesian(vector position)
	{
		CartesianObject = GetGame().CreateObjectEx("WorldCartesian", position, ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN|ECE_TRACE);
		//CartesianObject = GetGame().CreateObject("WorldCartesian", position, false, false, false);
		CartesianObject.SetOrientation(vector.Up);
		
	}
	
	void ~Cartesian()
	{
		GetGame().ObjectDelete(CartesianObject);
	}
	
	static Cartesian CreateOnObject(Object obj)
	{
		vector min, max, final;
		obj.GetWorldBounds(min, max);
				
		final[0] = (min[0] + max[0])/2;
		final[2] = (min[2] + max[2])/2;
		final[1] = max[1];		
		return new Cartesian(final);
	}
}

class BoundingBox
{	
	int color;
	
	vector b[2];

	void BoundingBox(Object building)
	{
		vector min, max; 
		building.GetWorldBounds(min, max);
		color = COLOR_BLUE_A;
		b[0] = min;
		b[1] = max;
		
		Debug.ClearCanvas();		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~BoundingBox()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	void Update()
	{
		vector b1 = { b[0][0], b[1][1], b[1][2] };
		vector b2 = { b[1][0], b[1][1], b[0][2] };
		vector b3 = { b[0][0], b[1][1], b[0][2] };
		vector b4 = { b[1][0], b[1][1], b[1][2] };
		vector b5 = { b[1][0], b[0][1], b[0][2] };
		vector b6 = { b[0][0], b[0][1], b[1][2] };
		vector b7 = { b[0][0], b[0][1], b[0][2] };
		vector b8 = { b[1][0], b[0][1], b[1][2] };
	
		
		int thickness = 2;
		vector v1 = GetGame().GetScreenPos(b1);
		vector v2 = GetGame().GetScreenPos(b2);
		vector v3 = GetGame().GetScreenPos(b3);
		vector v4 = GetGame().GetScreenPos(b4);
		vector v5 = GetGame().GetScreenPos(b5);
		vector v6 = GetGame().GetScreenPos(b6);
		vector v7 = GetGame().GetScreenPos(b7);
		vector v8 = GetGame().GetScreenPos(b8);
		
		Debug.ClearCanvas();
		Debug.m_CanvasDebug.DrawLine(v6[0], v6[1], v1[0], v1[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v3[0], v3[1], v1[0], v1[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v4[0], v4[1], v1[0], v1[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v4[0], v4[1], v8[0], v8[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v5[0], v5[1], v8[0], v8[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v6[0], v6[1], v8[0], v8[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v7[0], v7[1], v3[0], v3[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v7[0], v7[1], v6[0], v6[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v7[0], v7[1], v5[0], v5[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v4[0], v4[1], v2[0], v2[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v2[0], v2[1], v5[0], v5[1], thickness, color);
		Debug.m_CanvasDebug.DrawLine(v3[0], v3[1], v2[0], v2[1], thickness, color);
	}
}






class Editor: Managed
{
	private ref UIManager 					m_UIManager;
	static ref EditorUI						m_EditorUI;	
	
	
	PlayerBase 								m_PlayerBase;
	EditorCamera							m_Camera;
 	
				
	static ref Hologram 					ObjectInHand;
	static Object						ObjectUnderCursor;
	static Object						CurrentSelectedObject;
	static ref Cartesian						ActiveCartesian;
	static ref BoundingBox						ActiveBoundingBox;
	
	static ref array<ref Object>			PlacedObjects;
	static ref array<string>				EditorListObjects;
	//static ref map<ref Object, ref EditorBrowserListItem> EditorBrowserListItems;
	//static ref map<IEntity, ref EditorObjectMetaData> EditorObjectPlacedObjects;

	
	
	void Editor()
	{
		Print("Editor");
		m_UIManager = GetGame().GetUIManager();

		LoadEditorObjects();
		LoadPlacedObjects();
		
		m_Camera = GetGame().CreateObject("EditorCamera", "7500 500 7500", false);
		m_Camera.SetActive(true);
		
		
		m_PlayerBase = GetGame().GetPlayer();
		if (m_PlayerBase)
			m_PlayerBase.GetInputController().SetDisabled(true);

		
		InitUI();
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		Debug.InitCanvas();
		
	}
	
	void ~Editor()
	{
		Print("~Editor");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	void InitUI()
	{
		m_EditorUI = new EditorUI();
		m_EditorUI.Init();	
	}
	
	bool first_time = true;
	
	void ShowUI(bool state)
	{
		if (first_time)
			m_UIManager.ShowScriptedMenu(m_EditorUI, m_UIManager.GetMenu()); 
		
		m_EditorUI.Show(state);
		m_Camera.LookFreeze = state;
		m_UIManager.ShowUICursor(state);
		if (!state) {
			delete ObjectInHand;
		}
		first_time = false;
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
		    }
		}
	}
	
	void Update() 
	{
		set<Object> obj;
		if (IsPlacing()) {
			vector v = MousePosToRay(obj, ObjectInHand.GetProjectionEntity());
			ObjectInHand.SetProjectionPosition(v);
			ObjectInHand.SetProjectionOrientation(GetGame().SurfaceGetNormal(v[0], v[2]));
			
			
		} else {
			
			v = MousePosToRay(obj);
			if (obj == null) return;
			Object e = obj.Get(0);
			if (e != NULL) {
				int x, y;
				GetMousePos(x, y);
				
				if ((PlacedObjects.Find(e) + 1) && ObjectUnderCursor != e) {
					OnMouseEnterObject(e, x, y);
				}
				SetObjectUnderCursor(e);
				
			}
		}
	}
	
	static void OnMouseEnterObject(Object obj, int x, int y)
	{
		Print("OnMouseEnterObject");
		
		delete ActiveBoundingBox;
		ActiveBoundingBox = new BoundingBox(obj);
	}
	
	static void CreateObjectInHand(string name)
	{
		EntityAI e = GetGame().CreateObject(name, "0 0 0");
		set<Object> o;
		vector v =  MousePosToRay(o);
		ObjectInHand = new Hologram(null, v, e);
	}
	
	static bool IsPlacing()
	{
		return ObjectInHand != null;
	}
	
		
	static vector MousePosToRay(out set<Object> collisions = null, Object ignore = null)
	{
		vector rayStart = GetGame().GetCurrentCameraPosition();
		vector rayEnd = rayStart + GetGame().GetPointerDirection() * 1000;
		vector hitPos;
		vector hitNormal;
		int hitComponentIndex;		
		collisions = new set<Object>;

		DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, collisions, NULL, ignore);

		
		
		return hitPos;
	}
	
	static void SetObjectUnderCursor(notnull Object obj)
	{
		ObjectUnderCursor = obj;
	}
	
	
	
	static Object PlaceObject(string name, vector position, vector orientation)
	{
		Print("Editor::PlaceObject");
		Object b = GetGame().CreateObjectEx(name, position, ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN|ECE_TRACE);
		b.SetOrientation(orientation);			
		
		PlacedObjects.Insert(b);
		
		
		Editor.SetActiveObject(b);
		
		m_EditorUI.OnObjectPlaced(b, position, orientation);
		
		return b;
	}
	
	void LoadPlacedObjects()
	{
		// todo: allow people to open their own savefiles
		PlacedObjects = new array<ref Object>;
	}
	
	static void SetActiveObject(Object obj)
	{
		Print("Editor::SetActiveObject: " + obj);
		CurrentSelectedObject = obj;
		Widget browser_item = m_EditorUI.GetBrowserObjectFromEntity(obj);
		if (browser_item)
			SetFocus(browser_item);
		
		delete ActiveCartesian;
		ActiveCartesian = Cartesian.CreateOnObject(CurrentSelectedObject);
		ActiveBoundingBox.color = COLOR_BLUE;
		
	}
	
	static void ClearActiveObject()
	{
		Print("Editor::ClearActiveObject");
		delete ActiveCartesian;
		CurrentSelectedObject = null;
	}
	
	void DeleteObject(Object obj)
	{
		Widget browser_item = m_EditorUI.GetBrowserObjectFromEntity(obj);
		GetGame().ObjectDelete(obj);
		if (browser_item)
			delete browser_item;
	}
	
	bool ui_state = false;
	void OnKeyPress(int key) 
	{
		switch (key)
		{
			case KeyCode.KC_SPACE:
				ShowUI(ui_state);
				ui_state = !ui_state;
				break;
			
			case KeyCode.KC_DELETE:
				DeleteObject(CurrentSelectedObject);
				break;
			
						
			case KeyCode.KC_LEFT:
				m_EditorUI.Show(false);
				break;
			
			case KeyCode.KC_RIGHT:
				m_EditorUI.Show(true);
				break;
		}
	}
	
}
