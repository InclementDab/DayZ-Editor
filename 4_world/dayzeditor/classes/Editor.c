

typedef ref array<ref vector>> UndeleteableVectorArray;
typedef ref array<string>> UndeleteableStringArray;

typedef ref map<int, ref EditorObject> EditorObjectSet;


// FlattenGrassEllipse!!!!! sim city mode

class Editor: Managed
{
	private static ref Editor m_Instance;
	static Editor GetInstance() { return m_Instance; }
	
	private ref UIManager 					m_UIManager; 	
		
	static ref EditorHologram 				ObjectInHand;
	static Object							ObjectUnderCursor = null;
	static EditorObject 					EditorObjectUnderCursor = null;
	
	static ref set<ref EditorObjectLink>	SessionCache;
	static ref EditorObjectSet 				ClipboardCache;
	static ref set<ref EditorAction> 		ActionStack;
	
	static ref set<string> 					PlaceableObjects;
	static vector 							CurrentMousePosition;
	static bool 							IsDragging = false;
		
	static ref EditorObjectSet 				PlacedObjects;
	static ref EditorObjectSet				SelectedObjects;
	
	static ref Widget 						EditorUIWidget;
	static ref EditorUI						ActiveEditorUI;	
	static EditorCamera						ActiveCamera;
	
	static ref EditorBrush					ActiveBrush;
	
	TranslationWidget						GlobalTranslationWidget;
	
	static Object DebugObject0;
	static Object DebugObject1;
	static Object DebugObject2;
	static Object DebugObject3;
	
	void Editor()
	{
		Print("Editor");
		m_Instance = this;
		PlacedObjects 				= new EditorObjectSet();
		SelectedObjects				= new EditorObjectSet();
		SessionCache 				= new set<ref EditorObjectLink>();
		ActionStack 				= new set<ref EditorAction>();
		
		
		m_UIManager = GetGame().GetUIManager();
		
		// Init UI
		ActiveEditorUI = new EditorUI();
		EditorUIWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "Editor.layout");
		EditorUIWidget.GetScript(ActiveEditorUI);
		EntityAI translate = GetGame().CreateObjectEx("TranslationWidget", vector.Zero, ECE_NONE, RF_FRONT);
		ActiveEditorUI.m_OrientationWidget.SetItem(translate);
		ActiveEditorUI.m_OrientationWidget.SetView(0);

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
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
		EditorEvents.OnObjectDrag.Insert(HandleObjectDrag);
		EditorEvents.OnObjectDrop.Insert(HandleObjectDrop);
		
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		
		// Debug
		DebugObject0 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		DebugObject1 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		DebugObject2 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		DebugObject3 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
	}
		
	void ~Editor()
	{
		Print("~Editor");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	static void InsertAction(EditorAction target)
	{
		int count = ActionStack.Count();
		for (int i = 0; i < count; i++) {
			if (ActionStack[i].IsUndone()) {
				ActionStack.Remove(i);
				i--; count--;
			}
		}
			
		// Adds to bottom of stack
		ActionStack.InsertAt(target, 0);
		
		// debug
		ActiveEditorUI.m_DebugActionStack.ClearItems();
		
		for (int debug_i = 0; debug_i < ActionStack.Count(); debug_i++) {
			ActiveEditorUI.m_DebugActionStack.AddItem(ActionStack[debug_i].GetName(), ActionStack[debug_i], 0);
		}
		
	}

	static void LoadPlaceableObjects(string filter = "")
	{
		Print("EditorUI::LoadPlaceableObjects");
		
		PlaceableObjects = new set<string>();
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
				PlaceableObjects.Insert(Config_Name);	
				ActiveEditorUI.InsertPlaceableObject(Config_Name);
		    }
		}
	}

	
	void Undo()
	{
		Print("Editor::Undo");
		foreach (EditorAction action: ActionStack) {
			if (!action.IsUndone()) {
				action.CallUndo();
				
				//debug 
				for (int i = 0; i < ActiveEditorUI.m_DebugActionStack.GetNumItems(); i++) {
					EditorAction current_action;
					ActiveEditorUI.m_DebugActionStack.GetItemData(i, 0, current_action);
					if (current_action == action) 
						ActiveEditorUI.m_DebugActionStack.SetItemColor(i, 0, COLOR_RED);
					
				}
				
				return;
			}
		}	
	}
	
	void Redo()
	{
		Print("Editor::Redo");
		for (int i = ActionStack.Count() - 1; i >= 0; i--) {
			EditorAction action = ActionStack.Get(i);
			if (action == null) continue;
			if (action.IsUndone()) {
				action.CallRedo();
				
				//debug 
				for (int j = 0; j < ActiveEditorUI.m_DebugActionStack.GetNumItems(); j++) {
					EditorAction current_action;
					ActiveEditorUI.m_DebugActionStack.GetItemData(j, 0, current_action);
					if (current_action == action) 
						ActiveEditorUI.m_DebugActionStack.SetItemColor(j, 0, ARGB(255, 255, 255, 255));
					
				}
				
				return;
			}
		}
	}
	
	
	
	void Cut()
	{
		EditorAction action = new EditorAction("Create", "Delete");
		ClipboardCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: SelectedObjects) {
			ClipboardCache.Insert(id, selected_object);
			GetGame().ObjectDelete(selected_object);
		} 
		
		InsertAction(action);
		
	}
	
	void Copy()
	{
		ClipboardCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: SelectedObjects) {
			ClipboardCache.Insert(id, selected_object);			
		}
	}
	
	void Paste()
	{
		ClearSelections();
		vector avg_position;
		foreach (EditorObject copy_object: ClipboardCache) {
			avg_position += copy_object.GetPosition();
		}
		
		avg_position[0] = avg_position[0] / ClipboardCache.Count();
		avg_position[1] = avg_position[1] / ClipboardCache.Count();
		avg_position[2] = avg_position[2] / ClipboardCache.Count();
		
		foreach (EditorObject editor_object: ClipboardCache) {
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
		
		
		foreach (EditorObject save_object: PlacedObjects) {
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
			
			PlacedObjects.Insert(e_object.GetID(), e_object);
		}
		
	}

	
	
	static EditorObject EditorObjectFromObject(Object target)
	{
		foreach (EditorObject editor_object: PlacedObjects) {
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
		
		
		
		vector cam_orientation = ActiveCamera.GetOrientation();	
		ActiveEditorUI.m_OrientationWidget.SetModelOrientation(Vector(cam_orientation[1], cam_orientation[0], cam_orientation[2]));
		
		// debug
		if (SelectedObjects.Count() == 1) {
			EditorObject eo = SelectedObjects.Get(SelectedObjects.GetIteratorKey(SelectedObjects.Begin()));
			vector debug_pos = eo.GetPosition();
			ActiveEditorUI.m_DebugText1.SetText(string.Format("X: %1 Y: %2 Z: %3", debug_pos[0], debug_pos[1], debug_pos[2]));
			
		}
		
		
		
		string line1;
		if (!EditorObjectUnderCursor) 
			line1 = "NULL";
		else 
			line1 = EditorObjectUnderCursor.GetType();
		ActiveEditorUI.m_DebugText2.SetText(line1);
		ActiveEditorUI.m_DebugText3.SetText(Editor.SessionCache.Count().ToString());
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
	
	

	static void CreateObjectInHand(string name)
	{
		ObjectInHand = new EditorHologram(null, vector.Zero, GetGame().CreateObject(name, "0 0 0"));		
	}
	
	static EditorObjectLink SearchSessionCache(EditorObject target)
	{
		foreach (EditorObjectLink editor_object_link: Editor.SessionCache) {
			if (editor_object_link.Ptr() == target)
				return editor_object_link;
		}
		return null;
	}

	static EditorObject CreateObject(string name, vector position)
	{
		Print("Editor::CreateObject");
		
			
		EditorObject editor_object = GetGame().CreateObjectEx("EditorObject", position, ECE_NONE);
		editor_object.Init(name);


		// maybe move this into createinvoke	jk lol
		SessionCache.Insert(new EditorObjectLink(editor_object));
		PlacedObjects.Insert(editor_object.GetID(), editor_object);
		

		// Create Undo / redo action for creation
		EditorObjectLink link = SearchSessionCache(editor_object);
		EditorAction action = new EditorAction("Delete", "Create");
		action.InsertUndoParameter(link, null);
		action.InsertRedoParameter(link, null);
		Editor.InsertAction(action);
		
		ActiveEditorUI.InsertPlacedObject(editor_object);
		ActiveEditorUI.GetMap().OnObjectCreated(null, editor_object);

		EditorEvents.ObjectCreateInvoke(null, editor_object);
		
		
		return editor_object;
	}
	
	static void PlaceObject()
	{
		Input input = GetGame().GetInput();
		EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
		vector mat[4];
		e.GetTransform(mat);
		EditorObject editor_object = Editor.CreateObject(e.GetType(), e.GetPosition());
		editor_object.SetTransform(mat);
		editor_object.Select();
		if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
	}
	
	

	static void ClearSelections()
	{
		Print("Editor::ClearSelections");		
		foreach (EditorObject editor_object: PlacedObjects) {
			editor_object.Deselect();
		}
		
		
	}
	
	void DeleteObjects(EditorObjectSet target)
	{
		Print("Editor::DeleteObject");
	
		// Create Undo / redo action for deletion		
		EditorAction action = new EditorAction("Create", "Delete");
		foreach (EditorObject selected_object: target) {
			EditorObjectLink link = SearchSessionCache(selected_object);
			action.InsertUndoParameter(link, null);
			action.InsertRedoParameter(link, null);
			GetGame().ObjectDelete(selected_object);
			PlacedObjects.Remove(selected_object.GetID());
		}
		
		InsertAction(action);
		
		GetGame().ObjectDelete(GlobalTranslationWidget);
	}
	
	void DeleteObject(EditorObject target)
	{
		
		EditorAction action = new EditorAction("Create", "Delete");

		EditorObjectLink link = SearchSessionCache(target);
		action.InsertUndoParameter(link, null);
		action.InsertRedoParameter(link, null);
		GetGame().ObjectDelete(target);
		PlacedObjects.Remove(target.GetID());
		
		
		InsertAction(action);
		
		GetGame().ObjectDelete(GlobalTranslationWidget);
		
	}


	
	static bool IsPlacing() { return ObjectInHand != null; }
	static TranslationWidget GetTranslationWidget() { return Editor.GetInstance().GlobalTranslationWidget; }
	
	

	
	void OnObjectSelected(Class context, EditorObject target)
	{
		Print("Editor::OnObjectSelected");		
		
		if (GlobalTranslationWidget != null)
			GetGame().ObjectDelete(GlobalTranslationWidget);
		
		
		GlobalTranslationWidget = GetGame().CreateObjectEx("TranslationWidget", vector.Zero, ECE_SETUP | ECE_CREATEPHYSICS | ECE_LOCAL);
		GlobalTranslationWidget.SetEditorObject(target);	
		
	}
	
	void OnObjectDeselected(Class context, EditorObject target)
	{
		Print("Editor::OnObjectDeselected");
		GetGame().ObjectDelete(GlobalTranslationWidget);
	}
	
	
	
	void HandleObjectDrag(Class context, EditorObject target, ref RaycastRVResult raycast_result = null)
	{
		
		foreach (EditorObject editor_object: Editor.SelectedObjects)
			editor_object.TransformBeforeDrag = editor_object.GetTransformArray();
		
		string name = context.ClassName();		
		switch (name) {
			
			case "EditorMapMarker": {
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ObjectMapDragUpdate, 0, true, target);
				break;
			}
			
			case "EditorObjectMarker": {
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ObjectDragUpdate, 0, true, target);
				break;
			}
			
			case "TranslationWidget": {
				translation_offset = raycast_result.pos - TranslationWidget.Cast(context).GetPosition();
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(TranslationWidgetDragUpdate, 0, true, context, target, raycast_result);
				break;
			}			
			
			case "RotationWidget": {
				translation_offset = raycast_result.pos - RotationWidget.Cast(context).GetPosition();
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(RotationWidgetDragUpdate, 0, true, context, target, raycast_result);
				break;
			}
			
			default: {
				Print("Unhandled case: " + name);
				break;
			}
			
		}
		
		
		// debug
		Editor.DebugObject0 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
	}
	
	void HandleObjectDrop(Class context, EditorObject target)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ObjectMapDragUpdate);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(ObjectDragUpdate);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(TranslationWidgetDragUpdate);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(RotationWidgetDragUpdate);
		
		EditorAction action = new EditorAction("SetTransformArray", "SetTransformArray");
		foreach (EditorObject editor_object: Editor.SelectedObjects) {
			action.InsertUndoParameter(editor_object, editor_object.TransformBeforeDrag);
			action.InsertRedoParameter(editor_object, editor_object.GetTransformArray());
		}
		
		Editor.InsertAction(action);
		
		// debug
		GetGame().ObjectDelete(Editor.DebugObject0);
	}
	
	vector translation_offset;
	void TranslationWidgetDragUpdate(Class context, notnull EditorObject target, ref RaycastRVResult raycast_result)
	{
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			EditorEvents.DropInvoke(this, target);
			return;
		}
		
		TranslationWidget translation_widget = TranslationWidget.Cast(context);
	
		set<Object> o;
		vector cursor_position = MousePosToRay(o, null, vector.Distance(GetGame().GetCurrentCameraPosition(), target.GetPosition()));	
		vector widget_position = translation_widget.GetPosition();
			
		cursor_position = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), widget_position) - translation_offset;
		
		string name = translation_widget.GetActionComponentName(raycast_result.component);
		
		switch (name) {
			
			case "translatex": {
				widget_position[0] = cursor_position[0];
				break;
			}
			
			case "translatey": {
				widget_position[2] = cursor_position[2];
				break;				
			}
			
			case "translatez": {
				widget_position[1] = cursor_position[1];
				break;
			}
		}
				
		translation_widget.SetTranslationPosition(widget_position);
		translation_widget.Update();
		
	}
	
	void RotationWidgetDragUpdate(Class context, notnull EditorObject target, ref RaycastRVResult raycast_result)
	{
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			EditorEvents.DropInvoke(this, target);
			return;
		}
		
		RotationWidget rotation_widget = RotationWidget.Cast(context);
		
		set<Object> o;
		vector cursor_position = MousePosToRay(o, null);	
		vector widget_position = rotation_widget.GetPosition();
		
		
		
		cursor_position = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), widget_position) - translation_offset;
		
		string name = rotation_widget.GetActionComponentName(raycast_result.component);
		vector trans[4];
		vector ori;
		float angle;
		target.GetTransform(trans);
		cursor_position = target.GetPosition() - cursor_position;
		ori = target.GetOrientation();
		switch (name) {
			
			case "rotationx": {

				angle = Math.Atan2(cursor_position[1], cursor_position[2]) * Math.RAD2DEG;
				ori[2] = angle;
				break;
			}
			
			case "rotationy": {
				angle = Math.Atan2(cursor_position[0], cursor_position[1]) * Math.RAD2DEG;
				ori[1] = angle;
				
				//target.PlaceOnSurfaceRotated(trans, trans[3], angle, 0, 0, true);
				
				break;				
			}
			
			case "rotationz": {
				
				angle = Math.Atan2(cursor_position[2], cursor_position[0]) * Math.RAD2DEG;
				ori[0] = -angle;
				break;
			}
		}
		
		//target.SetTransform(trans);
		rotation_widget.SetRotation(ori);
	}
	
	
	// Handles Marker Drag from Map
	void ObjectMapDragUpdate(notnull EditorObject target)
	{
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			EditorEvents.DropInvoke(this, target);
			return;
		}
		
		vector object_position = target.GetPosition();
		int mouse_x, mouse_y;
		GetCursorPos(mouse_x, mouse_y);
		vector cursor_position = ActiveEditorUI.GetMapWidget().ScreenToMap(Vector(mouse_x, mouse_y, 0));
		cursor_position[1] = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]) + GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
		target.SetPosition(cursor_position);
		target.Update();
		
		foreach (EditorObject selected_object: SelectedObjects) {
			if (selected_object == target) continue;
			cursor_position[1] = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]) + GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
			selected_object.SetPosition(cursor_position + selected_object.GetPosition() - object_position); 
			selected_object.Update();
		}
		
		GlobalTranslationWidget.UpdatePosition();
	}
	

	
	
	// Handles Marker Drag from Object Base Marker
	void ObjectDragUpdate(notnull EditorObject target)
	{

		float starttime = TickCount(0);
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			EditorEvents.DropInvoke(this, target);
			return;
		}
		
		// Get all object data 
		vector object_size = target.GetSize();
		vector object_transform[4], start_transform[4];
		// non-updated version of object_transform
		target.GetTransform(start_transform); 
		target.GetTransform(object_transform);
		
		// Raycast ground below object
		set<Object> o;
		vector ground, ground_dir; int component;
		DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, ground, ground_dir, component, o, NULL, target.GetObject(), false, true); // set to ground only

		vector cursor_position = MousePosToRay(o, target.GetObject(), EditorSettings.OBJECT_VIEW_DISTANCE, 0, true);
		vector surface_normal = GetGame().SurfaceGetNormal(ground[0], ground[2]);
		float surface_level = GetGame().SurfaceY(ground[0], ground[2]);
	
		
		// debug
		Editor.DebugObject0.SetPosition(cursor_position);

		// Handle Z only motion
		if (input.LocalValue("UALookAround")) {	
			cursor_position = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), ground);
			cursor_position[1] = cursor_position[1] + object_size[1]/2; // offset building height
			object_transform[3] = ground + object_transform[1] * vector.Distance(ground, cursor_position);
		
		// Handle XY Rotation
		} else if (input.LocalValue("UATurbo")) {
		
			object_transform = { "1 0 0", "0 1 0", "0 0 1", object_transform[3] };
			vector cursor_delta = cursor_position - object_transform[3];
			float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;	
			target.PlaceOnSurfaceRotated(object_transform, object_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, angle * -1, EditorSettings.MAGNET_PLACEMENT);
			target.LocalAngle = angle;
			
		// Handle regular motion
		} else {			
			
			if (EditorSettings.MAINTAIN_HEIGHT) 
				if (EditorSettings.MAGNET_PLACEMENT)
					object_transform[3] = cursor_position + surface_normal * vector.Distance(ground, object_transform[3]);				
				else 
					object_transform[3] = cursor_position + object_transform[1] * vector.Distance(ground, object_transform[3]);
				
			else {
				object_transform[3] = cursor_position;
				object_transform[3][1] = object_transform[3][1] + object_size[1]/2;					
			} 
		
			object_transform[0] = "1 0 0";
			object_transform[1] = "0 1 0";
			object_transform[2] = "0 0 1";
			target.PlaceOnSurfaceRotated(object_transform, object_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, target.LocalAngle * -1, EditorSettings.MAGNET_PLACEMENT);
		}
	
		target.SetTransform(object_transform);
		target.Update();
		
		// This handles all other selected objects
		foreach (EditorObject selected_object: Editor.SelectedObjects) {
			
			if (selected_object == target) continue;
			
			vector selected_transform[4];	
			selected_object.GetTransform(selected_transform);
			
			vector pos_delta = selected_transform[3] - start_transform[3];
			vector cursor_position_delta = object_transform[3] + selected_transform[3] - start_transform[3];
			vector delta2 = start_transform[3] - selected_transform[3];
		
			float angle_delta = Math.Atan2(pos_delta[0], pos_delta[2]) * Math.RAD2DEG;
			surface_normal = GetGame().SurfaceGetNormal(selected_transform[3][0], selected_transform[3][2]);
			DayZPhysics.RaycastRV(selected_transform[3], selected_transform[3] + selected_transform[1] * -1000, ground, ground_dir, component, o, NULL, selected_object.GetObject(), false, true); // set to ground only

			
			// Handle Z only motion for all children				
			if (input.LocalValue("UALookAround")) {
				// Source object position + delta
				//selected_transform[3][1] = object_transform[3][1] + pos_delta[1];
				selected_transform[3] = ground + selected_transform[1] * vector.Distance(cursor_position_delta, ground);
				
			// Handle XY Rotation for all children
			} else if (input.LocalValue("UATurbo")) {
				vector rot_pos;
				angle -= angle_delta;				
				vector new_postion = vector.RotateAroundZero(pos_delta, vector.Up, Math.Cos(angle), Math.Sin(angle));
				selected_object.SetPosition(new_postion + object_transform[3]);
			
			// Handle regular motion for all children
			} else {
				//cursor_position_delta[1] = ground[1];
				if (EditorSettings.MAINTAIN_HEIGHT) {
					if (EditorSettings.MAGNET_PLACEMENT) {
						selected_transform[3] = cursor_position_delta + surface_normal * vector.Distance(ground, selected_transform[3]);
					} else {
					
						selected_transform[3] = cursor_position_delta + selected_transform[1] * vector.Distance(ground, selected_transform[3]);
					}
				} else {
					selected_transform[3] = cursor_position_delta;				
				} 
				
				selected_transform[0] = "1 0 0";
				selected_transform[1] = "0 1 0";
				selected_transform[2] = "0 0 1";
				selected_object.PlaceOnSurfaceRotated(selected_transform, selected_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, selected_object.LocalAngle * -1, EditorSettings.MAGNET_PLACEMENT);
			}	
			
		
			selected_object.SetTransform(selected_transform);
			selected_object.Update();
			
		}
		
		
		GlobalTranslationWidget.UpdatePosition();
		//Print(TickCount(starttime) / 1000);
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
					foreach (EditorObject editor_object: PlacedObjects) 
						editor_object.Select(false);
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
				break;
			}
			
			case KeyCode.KC_O: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Open();
					return true;
				}
				break;
			}
		}
		
		// todo add increment size in ui
		foreach (EditorObject selected_objects: SelectedObjects) {
			
			switch (key) {
				case KeyCode.KC_UP: {
					IncrementMove(selected_objects, 2, -0.5);
					break;
				}
				
				case KeyCode.KC_DOWN: {
					IncrementMove(selected_objects, 2, 0.5);
					break;
				}
			}
			
			switch (key) {
				case KeyCode.KC_LEFT: {
					IncrementMove(selected_objects, 0, -0.5);
					break;
				}			
				
				case KeyCode.KC_RIGHT: {
					IncrementMove(selected_objects, 0, 0.5);
					break;
				}			
			}
			
			switch (key) {
				case KeyCode.KC_PRIOR: {				
					IncrementMove(selected_objects, 1, 0.5);
					break;
				}	
				
				case KeyCode.KC_NEXT: {
					IncrementMove(selected_objects, 1, -0.5);
					break;
				}
			}
		}
		
		return ActiveEditorUI.OnKeyPress(key));
	}
	
	void IncrementMove(EditorObject obj, int axis, float move)
	{
		vector pos = obj.GetPosition();
		pos[axis] = pos[axis] + move;
		obj.SetPosition(pos);
		obj.Update();
		GlobalTranslationWidget.UpdatePosition();
		
	}
}








