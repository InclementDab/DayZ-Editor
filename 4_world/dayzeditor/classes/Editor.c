

typedef ref array<ref vector>> UndeleteableVectorArray;
typedef ref array<string>> UndeleteableStringArray;

typedef ref map<int, ref EditorObject> EditorObjectSet;


// FlattenGrassEllipse!!!!! sim city mode

class Editor: Managed
{
	private ref UIManager 					m_UIManager; 	
		
	static ref EditorHologram 				ObjectInHand;
	static Object							ObjectUnderCursor = null;
	static EditorObject 					EditorObjectUnderCursor = null;
	
	static ref set<ref EditorObjectLink>	SessionCache;
	static ref EditorObjectSet 				CopyCache;
	
	static ref set<string> 	EditorPlaceableObjects;
	static vector 			CurrentMousePosition;
	static bool 			IsDragging = false;
		
	static ref EditorObjectSet				SelectedObjects;
	static ref EditorObjectSet 				EditorObjects;
	
	static ref set<ref EditorAction> 		EditorActionStack;
	
	static ref Widget 						EditorUIWidget;
	static ref EditorUI						ActiveEditorUI;	
	static EditorCamera						ActiveCamera;
	
	static TranslationWidget				GlobalTranslationWidget;
	
	static Object DebugObject0;
	static Object DebugObject1;
	static Object DebugObject2;
	static Object DebugObject3;
	

	
	void Editor()
	{
		Print("Editor");
		EditorObjects 				= new EditorObjectSet();
		SelectedObjects				= new EditorObjectSet();
		SessionCache 				= new set<ref EditorObjectLink>();
		EditorActionStack 			= new set<ref EditorAction>();
		
		
		m_UIManager = GetGame().GetUIManager();
		
		// Init UI
		ActiveEditorUI = new EditorUI();
		EditorUIWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "Editor.layout");
		EditorUIWidget.GetScript(ActiveEditorUI);

		// Init Spawn Position
		TIntArray center_pos = new TIntArray();		
		string world_name;
		GetGame().GetWorldName(world_name);
		GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", world_name), center_pos);
		
		// Init Camera
		float y_level = 200 + GetGame().SurfaceY(center_pos[0], center_pos[1]);
		ActiveCamera = GetGame().CreateObject("EditorCamera", Vector(center_pos[0], y_level, center_pos[1]), false);
		ActiveCamera.SetActive(true);
		
		
		
		LoadPlaceableObjects();
		EditorSettings.Load();
		
		// Event subscriptions
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
		EditorEvents.OnObjectSelectionChanged.Insert(OnObjectSelected);
		EditorEvents.OnObjectDrag.Insert(HandleObjectDrag);
		EditorEvents.OnObjectDrop.Insert(HandleObjectDrop);
		
		Editor.GlobalTranslationWidget = GetGame().CreateObject("TranslationWidget", vector.Zero);

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
		
	void ~Editor()
	{
		Print("~Editor");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	static void InsertAction(EditorAction target)
	{

		for (int i = 0; i < EditorActionStack.Count(); i++) 
			if (EditorActionStack[i].was_undone) 
				delete EditorActionStack[i];
		
		EditorActionStack.InsertAt(target, 0);
	}

	static void LoadPlaceableObjects(string filter = "")
	{
		Print("EditorUI::LoadPlaceableObjects");
		
		EditorPlaceableObjects = new set<string>();
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < g_Game.ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        GetGame().ConfigGetChildName(Config_Path, j, Config_Name);
		        GetGame().ConfigGetBaseName(Config_Path + " " + Config_Name, Base_Name);
		        Base_Name.ToLower();
		
		        //if (Base_Name != "housenodestruct")
		        //    continue;
			
				
				EditorPlaceableObjects.Insert(Config_Name);	
				ActiveEditorUI.InsertPlaceableObject(Config_Name);
		    }
		}
	}

	
	void Undo()
	{
		Print("Editor::Undo");
		foreach (EditorAction action: EditorActionStack) {
			if (!action.was_undone) {
				action.CallUndo();
				return;
			}
		}	
	}
	
	void Redo()
	{
		Print("Editor::Redo");

		
		for (int i = EditorActionStack.Count() - 1; i >= 0; i--) {
			EditorAction action = EditorActionStack.Get(i);
			if (action == null) continue;
			if (action.was_undone) {
				action.CallRedo();
				return;
			}
		}
	}
	
	
	
	void Cut()
	{
		EditorAction action = new EditorAction("Create", "Delete");
		CopyCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: SelectedObjects) {
			CopyCache.Insert(id, selected_object);
			GetGame().ObjectDelete(selected_object);
		} 
		
		InsertAction(action);
		
	}
	
	void Copy()
	{
		CopyCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: SelectedObjects) {
			CopyCache.Insert(id, selected_object);			
		}
	}
	
	void Paste()
	{
		ClearSelections();
		vector avg_position;
		foreach (EditorObject copy_object: CopyCache) {
			avg_position += copy_object.GetPosition();
		}
		
		avg_position[0] = avg_position[0] / CopyCache.Count();
		avg_position[1] = avg_position[1] / CopyCache.Count();
		avg_position[2] = avg_position[2] / CopyCache.Count();
		
		foreach (EditorObject editor_object: CopyCache) {
			vector mat[4];
			editor_object.GetTransform(mat);
			mat[3] = avg_position - editor_object.GetPosition() + CurrentMousePosition;
			EditorObject result = CreateObject(editor_object.GetType(), mat);
			result.Select();
		}
	}
	
	void Save()
	{	
		EditorWorldData save_data = new EditorWorldData();
		ActiveCamera.GetTransform(save_data.CameraPosition);
		
		
		foreach (EditorObject save_object: EditorObjects) {
			vector mat[4]; 
			save_object.GetTransform(mat);
			save_data.WorldObjects.Insert(new EditorWorldObject(save_object.GetType(), mat));
		}
		
		EditorFileManager.SaveFile(save_data);
	}
	
	void Open()
	{
		EditorWorldData load_data = EditorFileManager.LoadFile();
		ActiveCamera.SetTransform(load_data.CameraPosition);
		// find a proper way to remove all existing files. maybe restart editor
		
		foreach (EditorWorldObject load_object: load_data.WorldObjects) {
			EditorObject e_object = CreateObject(load_object.Classname, load_object.Transform);
			
			EditorObjects.Insert(e_object.GetID(), e_object);
		}
		
	}

	
	
	static EditorObject EditorObjectFromObject(Object target)
	{
		foreach (EditorObject editor_object: EditorObjects) {
			if (editor_object.GetObject() == target) {
				return editor_object;
			}
		}
		
		return null;
	}
	
	
	bool exit_condition = false;
	void Update() 
	{
		set<Object> obj = new set<Object>();
		int x, y;
		GetCursorPos(x, y);
		CurrentMousePosition = MousePosToRay(obj);
		
	
		if (!IsPlacing()) {
			Object target = obj.Get(0);
			
			if (target != null) {
				if (target != ObjectUnderCursor) {
					if (ObjectUnderCursor != null) OnMouseExitObject(ObjectUnderCursor, x, y);
					OnMouseEnterObject(target, x, y);
					ObjectUnderCursor = target;
				}
				
			} else if (ObjectUnderCursor != null) {
				exit_condition = OnMouseExitObject(ObjectUnderCursor, x, y);
				ObjectUnderCursor = null;
			}			
		}
		
		ActiveEditorUI.m_DebugText1.SetText(string.Format("X: %1 Y: %2 Z: %3", CurrentMousePosition[0], CurrentMousePosition[1], CurrentMousePosition[2]));
		
		string line1;
		if (!EditorObjectUnderCursor) 
			line1 = "NULL";
		else 
			line1 = EditorObjectUnderCursor.GetType();
		ActiveEditorUI.m_DebugText2.SetText(line1);
		ActiveEditorUI.m_DebugText3.SetText(Editor.SelectedObjects.Count().ToString());
	}
	
	
	
	bool OnMouseEnterObject(Object target, int x, int y)
	{
		//Print("Editor::OnMouseEnterObject");
		EditorObjectUnderCursor = EditorObjectFromObject(target);
		if (EditorObjectUnderCursor != null) {
			return EditorObjectUnderCursor.OnMouseEnter(x, y);
		}
		
		
		return true;
		
	}
	
	bool OnMouseExitObject(Object target, int x, int y)
	{
		if (EditorObjectUnderCursor != null)
			EditorObjectUnderCursor.OnMouseLeave(x, y);
		EditorObjectUnderCursor = null;
		
		return true;
		
	}
	
	
	// can we refactor this? .... probably :)
	static void CreateObjectInHand(string name)
	{
			
			
		ObjectInHand = new EditorHologram(null, vector.Zero, GetGame().CreateObject(name, "0 0 0"));
		
		
	}
	
	
	
	static EditorObject CreateObject(string name, vector transform[4])
	{
		Print("Editor::CreateObject");
	
		
		EditorObject editor_object = GetGame().CreateObjectEx("EditorObject", transform[3], ECE_NONE);		
		editor_object.SetTransform(transform);
		editor_object.SetObject(name);
		
		editor_object.CreateBoundingBox();
		editor_object.Update();
		
		EditorObjectLink link = new EditorObjectLink(editor_object);
		SessionCache.Insert(link);
		
		Editor.EditorObjects.Insert(editor_object.GetID(), editor_object);
		
		EditorEvents.ObjectCreateInvoke(null, editor_object);
		
		return editor_object;
	}
	
	static void PlaceObject()
	{
		Input input = GetGame().GetInput();
		EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
		vector mat[4];
		e.GetTransform(mat);
		EditorObject editor_object = Editor.CreateObject(e.GetType(), mat);
		editor_object.Select();
		if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
	}
	
	

	static void ClearSelections()
	{
		Print("Editor::ClearSelections");		
		foreach (EditorObject editor_object: EditorObjects) {
			editor_object.Deselect();
		}
	}
	
	void DeleteObjects(EditorObjectSet target)
	{
		Print("Editor::DeleteObject");
	
		EditorAction action = new EditorAction("Create", "Delete");
		foreach (EditorObject selected_object: target) {
			EditorObjects.Remove(selected_object.GetID());
			GetGame().ObjectDelete(selected_object);
		}
		
		
		InsertAction(action);
	}

	
	
	
	bool OnMouseButtonDown(int x, int y, int button)
	{
		Input input = GetGame().GetInput();
		if (button == 2) {
			
			if (input.LocalValue("UAWalkRunTemp")) {
				set<Object> oop;
				vector v = MousePosToRay(oop);
				LightingBolt.CreateLightning(v, 1);		
			} else {				
				set<Object> o;
				vector pos = MousePosToRay(o);
				vector current = ActiveCamera.GetPosition();
				pos[1] = current[1];
				ActiveCamera.SetPosition(pos);
			}
	
			
		}
		
		return true;
	}
	
	bool OnKeyPress(int key) 
	{
		Input input = GetGame().GetInput();
		switch (key) {

			case KeyCode.KC_DELETE: {
				DeleteObjects(SelectedObjects);
				ClearSelections();
				return true;
			}

			
			case KeyCode.KC_Z: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Undo();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_Y: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Redo();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_A: {
				if (input.LocalValue("UAWalkRunTemp")) {
					ClearSelections();
					foreach (EditorObject editor_object: EditorObjects) 
						editor_object.Select();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_X: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Cut();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_C: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Copy();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_V: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Paste();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_E: {
				if (input.LocalValue("UAWalkRunTemp")) {
					ActiveEditorUI.ShowExportWindow();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_S: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Save();
					
					return true;
				}
			}
			
			case KeyCode.KC_O: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Open();
					return true;
				}
			}
			
		
		}
		
		return ActiveEditorUI.OnKeyPress(key));
	}
	
	
	static bool IsPlacing() { return ObjectInHand != null; }
	
	
	
	
	void OnObjectCreated(Class context, EditorObject target)
	{
		ActiveEditorUI.InsertPlacedObject(target);
		
		ActiveEditorUI.GetMap().OnObjectCreated(context, target);
	}
	
	void OnObjectSelected(Class context, Param2<EditorObject,bool> params)
	{
		Print("EditorUI::OnObjectSelected");	
		if (!params.param2)
			GetGame().ObjectDelete(Editor.GlobalTranslationWidget);
		
		//if (SelectedObjects.Count() > 1) return;
		//Editor.GlobalTranslationWidget = GetGame().CreateObjectEx("TranslationWidget", params.param1.GetPosition(), ECE_NONE);
		//Editor.GlobalTranslationWidget.SetEditorObject(params.param1, params.param1.GetPosition());

		
		
	}
	
	
	vector start_position;
	void HandleObjectDrag(Class context, EditorObject target)
	{
		start_position = target.GetPosition();
		Editor.DebugObject0 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ObjectDragUpdate, 0, true, target);		
	}
	
	void HandleObjectDrop(Class context, EditorObject target)
	{		
		GetGame().ObjectDelete(Editor.DebugObject0);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ObjectDragUpdate);
	}
	
	
	void ObjectDragUpdate(notnull EditorObject target)
	{
		float starttime = TickCount(0);
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			EditorEvents.DropInvoke(this, target);
			return;
		}
		
		// Get all object data 
		Object target_object = target.GetObject();
		vector object_position = target_object.GetPosition();
		vector object_size = target.GetSize();
		vector object_orientation = target_object.GetOrientation();
		vector object_transform[4];
		target_object.GetTransform(object_transform);
		
		// Raycast ground below object
		set<Object> o;
		vector ground, ground_dir; int component;
		DayZPhysics.RaycastRV(object_position, object_position + object_transform[1] * -1000, ground, ground_dir, component, o, NULL, target_object, false, true); // set to ground only
		
		vector cursor_position = MousePosToRay(o, target_object);
		if (GROUND_MODE) {
			cursor_position[1] = object_transform[3][1];
		} else {
			cursor_position[1] = cursor_position[1] + object_size[1] / 2;
		}		
		Editor.DebugObject0.SetPosition(ground);
		
		vector transform_position[4] = { "1 0 0", "0 1 0", "0 0 1", cursor_position };
		vector surface_normal = GetGame().SurfaceGetNormal(cursor_position[0], cursor_position[2]);
		float surface_level = GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
		
		
		// If map is ON	
		if (ActiveEditorUI.IsMapOpen()) {
			
			int mouse_x, mouse_y;
			GetCursorPos(mouse_x, mouse_y);
			cursor_position = ActiveEditorUI.GetMapWidget().ScreenToMap(Vector(mouse_x, mouse_y, 0));
			cursor_position[1] = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]) + GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
			target.SetPosition(cursor_position);
			
		} else {
		
			// Handle Z only motion*
			if (input.LocalValue("UALookAround")) {	
						
				cursor_position = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), ground);
				cursor_position[1] = cursor_position[1] + object_size[1]/2;
				transform_position[0] = object_transform[0];
				transform_position[1] = object_transform[1];
				transform_position[2] = object_transform[2];	
				transform_position[3] = ground + object_transform[1] * vector.Distance(ground, cursor_position);
				
			
			// Handle XY Rotation
			} else if (input.LocalValue("UATurbo")) {
				
				vector cursor_delta = cursor_position - object_position;
				
				float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;				
				transform_position[3][0] = object_position[0];
				transform_position[3][2] = object_position[2];
				target.PlaceOnSurfaceRotated(transform_position, object_position, surface_normal[0] * -1, surface_normal[2] * -1, angle * -1, MAGNET_PLACEMENT);
				//vector.Direction(object_position, cursor_position);
				//Math3D.DirectionAndUpMatrix(, target.GetTransformAxis(1), transform);
				
			} else {
				
				target.PlaceOnSurfaceRotated(transform_position, cursor_position, surface_normal[0] * -1, surface_normal[2] * -1, 0, MAGNET_PLACEMENT);
				
			}
			
			target.SetTransform(transform_position);
			target.Update();
		}
		


					
		// This handles all other selected objects
		foreach (EditorObject selected_object: Editor.SelectedObjects) {
			
			if (selected_object == target) continue;
			vector selected_size = selected_object.GetSize();
			vector selected_position = selected_object.GetPosition();
			vector pos_delta = selected_position - object_position;
			float angle_delta = Math.Atan2(pos_delta[0], pos_delta[2]) * Math.RAD2DEG;
			surface_normal = GetGame().SurfaceGetNormal(selected_position[0], selected_position[2]);
			
			// If map is ON	
			if (ActiveEditorUI.IsMapOpen()) {
			
				cursor_position[1] = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]) + GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
				selected_object.SetPosition(cursor_position + pos_delta); 
				
			} else {
				
				// Handle Z only motion for all child objects
				if (input.LocalValue("UALookAround")) {
					
					pos_delta[0] = selected_position[0];
					pos_delta[1] = transform_position[3][1] + pos_delta[1];
					pos_delta[2] = selected_position[2];
					selected_object.SetPosition(pos_delta);
					
				// Handle XY Rotation for all child 
				} else if (input.LocalValue("UATurbo")) {
					vector rot_pos;
					
					angle -= angle_delta;				
					//Print(angle);
					vector new_postion = vector.RotateAroundZero(pos_delta, vector.Up, Math.Cos(angle), Math.Sin(angle));
					
					
					//Print(new_postion);					
					
					selected_object.SetPosition(new_postion + object_position);
					
				} else {
					transform_position[4] = { "1 0 0", "0 1 0", "0 0 1", cursor_position + pos_delta };
					target.PlaceOnSurfaceRotated(transform_position, cursor_position + pos_delta, surface_normal[0] * -1, surface_normal[2] * -1, 0, MAGNET_PLACEMENT);
					
					selected_object.SetTransform(transform_position);
				}	
				
				
				selected_object.Update();
			}
			
			
		}
		
		//Print(TickCount(starttime) / 1000);
	}
}


enum ExportMode {
	TERRAINBUILDER,
	COMFILE, 
	EXPANSION
}


enum HeightType {
	ASL,
	ATL
}

class ExportSettings
{
	static HeightType ExportHeightType;
	
}




