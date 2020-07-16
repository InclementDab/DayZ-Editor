


class Editor: Managed
{
	private ref UIManager 					m_UIManager; 	
				
	static ref Hologram 					ObjectInHand;
	static Object							ObjectUnderCursor;
	//static ref Cartesian					ActiveCartesian;
	//static Object							ActiveBoundingBox;
	
	static ref set<ref EditorObject>			SelectedObjects;	
	static ref set<ref EditorObject> 			EditorObjects;
	static ref array<string>					EditorPlaceableObjects;
	
	
	void Editor()
	{
		Print("Editor");		
		m_UIManager = GetGame().GetUIManager();
		
		EditorObjects = new set<ref EditorObject>();
		SelectedObjects	= new set<ref EditorObject>();
		
		LoadPlaceableObjects();
		LoadPlacedObjects();

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
		
	void ~Editor()
	{
		Print("~Editor");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	
	

	void LoadPlaceableObjects()
	{
		Print("Editor::LoadPlaceableObjects");
		EditorPlaceableObjects = new array<string>();
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
			        
				EditorPlaceableObjects.Insert(Config_Name);	
				
		    }
		}
	}
	

	
	
	static EditorObject EditorObjectFromObject(notnull Object target)
	{		
		for (int i = 0; i < EditorObjects.Count(); i++) {
			EditorObject editor_object = EditorObjects[i];
			if (editor_object == null) return null;
			if (editor_object.WorldObject == target) {
				return editor_object;
			}
		}
		return null;
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
				ObjectUnderCursor = target;
				EditorObject editor_object = EditorObjectFromObject(target);
				if (editor_object != null) 
					OnMouseEnterObject(editor_object, x, y);
				
			} else if (target == null && !exit_condition) { 
				ObjectUnderCursor = null;
				exit_condition = OnMouseExitObject(x, y);
				
			}
			
		}
	}
	
	
	static bool CursorAllowedToSelect = true;
	bool OnMouseEnterObject(EditorObject target, int x, int y)
	{
		//Print("OnMouseEnterObject");
		if (!CursorAllowedToSelect) return false;
		if (GetGame().GetInput().LocalHold("UAFire")) return false;
		
		return true;
	}
	
	bool OnMouseExitObject(int x, int y)
	{
		if (GetGame().GetInput().LocalHold("UAFire")) return false;
		return true;
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
	
	static EditorObject PlaceObject(string name, vector position, vector orientation)
	{
		Print("Editor::PlaceObject");
		Object o = GetGame().CreateObjectEx(name, position, ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN|ECE_TRACE);
		EditorObject editor_object = new EditorObject(o);
		editor_object.WorldObject.SetOrientation(orientation);
		
		
		editor_object.OnPlaced(position, orientation);
		EditorObjects.Insert(editor_object);
		CreateSelection(editor_object);
		
		return editor_object;
	}
	
	
	static void CreateSelection(notnull EditorObject target, bool remove_old = true)
	{
		//Print("Editor::CreateSelection");
		if (remove_old) ClearSelections();
		
		int c = SelectedObjects.Count();
		if (SelectedObjects.Insert(target) == c) {
			target.Select();
		}
		

	}
	
	static void ClearSelections()
	{
		Print("Editor.ClearSelections");
		foreach (EditorObject editor_object: EditorObjects) {
			if (editor_object.IsSelected)
				editor_object.Deselect();
		}
		
		SelectedObjects.Clear();
	}
	
	static void RemoveSelection(notnull EditorObject target)
	{
		int index = SelectedObjects.Find(target);
		if (index == -1) return;
		
		SelectedObjects.Remove(index);
		target.Deselect();
	}
	
	
	static bool danger_zone = false;
	bool ui_state = true;
	void OnKeyPress(int key) 
	{
		switch (key)
		{

			
			case KeyCode.KC_DELETE:
				foreach (EditorObject selected_object: SelectedObjects)
					delete selected_object;
				break;
			
			case KeyCode.KC_F4:
				danger_zone = !danger_zone;
				break;
			
						

			
			case KeyCode.KC_RIGHT:
				break;
		}
	}
	
	
	static bool IsPlacing()
	{
		return ObjectInHand != null;
	}
	

	
	static void LoadPlacedObjects()
	{
		// todo: allow people to open their own savefiles
		//PlacedObjects = new array<Object>;
	}
}











