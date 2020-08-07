
// One day, someone important (likely Adam) will look over this codebase with a sny look of shame on their face
// if today is that day. fix it.
// and message me your feedback on discord :)


static PlayerBase CreateDefaultCharacter()
{
	PlayerBase player;
	if (GetGame().GetPlayer() != null) {
		player = GetGame().GetPlayer();
	} else {	
	    player = PlayerBase.Cast(GetGame().CreatePlayer(NULL, GetGame().CreateRandomPlayer(), vector.Zero, 0, "NONE"));
	    player.GetInventory().CreateInInventory("AviatorGlasses");
	    player.GetInventory().CreateInInventory("AliceBag_Black");
	    player.GetInventory().CreateInInventory("TranslationWidget");
	}
	
    return player;
}

private ref Editor m_EditorInstance;
static Editor GetEditor() { return m_EditorInstance; }

class Editor: Managed
{
	// Private Memebers
	private ref EditorObjectManager			m_EditorObjectManager = new EditorObjectManager();
	private ref EditorUIManager 			m_EditorUIManager = new EditorUIManager();
	private ref EditorSettings				m_EditorSettings = new EditorSettings();
	
	private ref EditorBrush					m_EditorBrush;
	
	static ref EditorHologram 				ObjectInHand;
	static Object							ObjectUnderCursor = null;
	static EditorObject 					EditorObjectUnderCursor = null;
	
	static vector 							CurrentMousePosition;
	static bool 							IsDragging = false;
	static bool 							PlayerActive = false;
	static PlayerBase						EditorPlayer;

	TranslationWidget						GlobalTranslationWidget;
	
	// debug
	static Object DebugObject0, DebugObject1, DebugObject2, DebugObject3;
	
	// Getters
	ref EditorObjectManager GetObjectManager() { return m_EditorObjectManager; }
	ref EditorUIManager GetUIManager() { return m_EditorUIManager; }
	ref EditorSettings GetSettings() { return m_EditorSettings; }
	ref EditorBrush GetEditorBrush() { return m_EditorBrush; }
	
	bool IsPlacing() { return ObjectInHand != null; }
	TranslationWidget GetTranslationWidget() { return GetEditor().GlobalTranslationWidget; }

	void Editor()
	{
		Print("Editor");
		EditorSettings.Load();
		
		// Event subscriptions
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
		EditorEvents.OnObjectDrag.Insert(HandleObjectDrag);
		EditorEvents.OnObjectDrop.Insert(HandleObjectDrop);
		EditorEvents.OnBrushChanged.Insert(OnBrushChanged);
				
		// Character Creation
		EditorPlayer = CreateDefaultCharacter();
		

		// Debug
		DebugObject0 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		DebugObject1 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		DebugObject2 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		DebugObject3 = GetGame().CreateObject("BoundingBoxBase", vector.Zero);
		
		m_EditorSettings.SetPlaceableObjectCategory(PlaceableObjectCategory.BUILDING);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update); // Last thing always
	}
		
	void ~Editor()
	{
		Print("~Editor");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	


	
	void Undo()
	{
		Print("Editor::Undo");
		foreach (EditorAction action: GetObjectManager().GetActionStack()) {
			if (!action.IsUndone()) {
				action.CallUndo();
				
				//debug 
				for (int i = 0; i < GetUIManager().GetEditorUI().m_DebugActionStack.GetNumItems(); i++) {
					EditorAction current_action;
					GetUIManager().GetEditorUI().m_DebugActionStack.GetItemData(i, 0, current_action);
					if (current_action == action) 
						GetUIManager().GetEditorUI().m_DebugActionStack.SetItemColor(i, 0, COLOR_RED);
				}
				
				return;
			}
		}	
	}
	
	void Redo()
	{
		Print("Editor::Redo");
		for (int i = GetObjectManager().GetActionStack().Count() - 1; i >= 0; i--) {
			EditorAction action = GetObjectManager().GetActionStack().Get(i);
			if (action == null) continue;
			if (action.IsUndone()) {
				action.CallRedo();
				
				//debug 
				for (int j = 0; j < GetUIManager().GetEditorUI().m_DebugActionStack.GetNumItems(); j++) {
					EditorAction current_action;
					GetUIManager().GetEditorUI().m_DebugActionStack.GetItemData(j, 0, current_action);
					if (current_action == action) 
						GetUIManager().GetEditorUI().m_DebugActionStack.SetItemColor(j, 0, ARGB(255, 255, 255, 255));
					
				}
				
				return;
			}
		}
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
		
		
	
		// debug
		EditorObjectSet selected_objects = GetObjectManager().GetSelectedObjects();
		if (selected_objects.Count() == 1) {
			EditorObject eo = selected_objects.Get(selected_objects.GetIteratorKey(selected_objects.Begin()));
			vector debug_pos = eo.GetPosition();
			//GetUIManager().GetEditorUI().m_DebugText1.SetText(string.Format("X: %1 Y: %2 Z: %3", debug_pos[0], debug_pos[1], debug_pos[2]));
			GetUIManager().GetEditorUI().ShowObjPosInfoPanel(true);
			GetUIManager().GetEditorUI().UpdateInfoObjPos(debug_pos);
		} else 
		{ 
			GetUIManager().GetEditorUI().ShowObjPosInfoPanel(false);
		}
	
		string line1;
		if (!EditorObjectUnderCursor) 
			line1 = "NULL";
		else 
			line1 = EditorObjectUnderCursor.GetType();
		GetUIManager().GetEditorUI().m_DebugText2.SetText(line1);
		GetUIManager().GetEditorUI().m_DebugText3.SetText(GetObjectManager().GetSessionCache().Count().ToString());
		
	}
	
	static void SetCameraTarget(Object target)
	{
		GetEditor().GetUIManager().GetEditorCamera().SelectTarget(target);
	}
	
	
	void OnBrushChanged(Class context, EditorBrush brush) { m_EditorBrush = brush; }
	
	bool OnMouseEnterObject(Object target, int x, int y)
	{
		//Print("Editor::OnMouseEnterObject");
		EditorObjectUnderCursor = GetObjectManager().GetEditorObject(target);
		if (EditorObjectUnderCursor != null)
			return EditorObjectUnderCursor.OnMouseEnter(x, y);
	
	
		return true;
		
	}
	
	bool OnMouseExitObject(Object target, int x, int y)
	{
		if (EditorObjectUnderCursor != null)
			EditorObjectUnderCursor.OnMouseLeave(x, y);
		EditorObjectUnderCursor = null;
		
		return true;
	}
	
	
	private Object m_LootEditTarget;
	private bool m_LootEditMode;
	private vector m_PositionBeforeLootEditMode;
	
	// probably have an EditorMode enum with NORMAL, CHARACTER, LOOTEDITOR or something
	void PlaceholderForEditLootSpawns(string name)
	{
		m_LootEditTarget = GetGame().CreateObjectEx(name, Vector(0, 1000, 0), ECE_NONE);
		
		EditorCamera camera = GetUIManager().GetEditorCamera();
		m_PositionBeforeLootEditMode = camera.GetPosition();
		camera.SetPosition(Vector(10, 1000, 10));
		//camera.SelectTarget(m_LootEditTarget);
		
		
		ref EditorMapGroupProto proto_data = new EditorMapGroupProto(m_LootEditTarget); 
		EditorXMLManager.LoadMapGroupProto(proto_data);
		m_LootEditMode = true;
	}
	
	void PlaceholderRemoveLootMode()
	{
		IEntity child = m_LootEditTarget.GetChildren();
		while (child != null) {
			GetGame().ObjectDelete(child);
			child = child.GetSibling();
		}
		
		GetGame().ObjectDelete(m_LootEditTarget);
		
		EditorCamera camera = GetUIManager().GetEditorCamera();
		camera.SetPosition(m_PositionBeforeLootEditMode);
		//camera.SelectTarget(null);
		m_LootEditMode = false;
	}
	
	bool IsLootEditActive() { return m_LootEditMode; }
	
	void CreateObjectInHand(string name)
	{
		// Turn Brush off when you start to place
		EditorEvents.BrushChangedInvoke(this, null);
		
		EditorSettings.SIM_CITY_MODE = false;
		ObjectInHand = new EditorHologram(null, vector.Zero, GetGame().CreateObject(name, vector.Zero));		
	}
	

	void PlaceObject()
	{
		Input input = GetGame().GetInput();
		EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
		vector mat[4];
		
		
		EditorObject editor_object = GetEditor().GetObjectManager().CreateObject(e.GetType(), e.GetPosition());
		editor_object.SetOrientation(e.GetOrientation());
		GetEditor().GetObjectManager().SelectObject(editor_object, !input.LocalValue("UATurbo"));
		
		if (!input.LocalValue("UATurbo")) { 
			delete Editor.ObjectInHand;
		}
	}
	

	void Save()
	{	
		EditorWorldData save_data = new EditorWorldData();
		GetUIManager().GetEditorCamera().GetTransform(save_data.CameraPosition);
		
		EditorObjectSet placed_objects = GetObjectManager().GetPlacedObjects();
		foreach (EditorObject save_object: placed_objects)	
			save_data.WorldObjects.Insert(save_object.GetSaveData());
 
		EditorFileManager.SaveFile(save_data);
		GetEditor().GetUIManager().NotificationCreate("Saved!", COLOR_GREEN); 
	}
	
	void Open()
	{
		delete m_EditorObjectManager;
		m_EditorObjectManager = new EditorObjectManager();
		
		EditorWorldData load_data = new EditorWorldData();
		int loadfile_result = EditorFileManager.LoadFile(load_data);
		Print("LoadFile Result " + loadfile_result);
		GetUIManager().GetEditorCamera().SetTransform(load_data.CameraPosition);
		
		foreach (EditorWorldObject load_object: load_data.WorldObjects) {
			EditorObject e_object =  GetObjectManager().CreateObject(load_object.m_Typename, load_object.m_Transform[3]);
			GetObjectManager().GetPlacedObjects().Insert(e_object.GetID(), e_object);
		}
		
		GetEditor().GetUIManager().NotificationCreate("Loaded!"); 
	}
	
	

	
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
		ref EditorObjectSet selected_objects = GetObjectManager().GetSelectedObjects();
		
		foreach (EditorObject editor_object: selected_objects)
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
		EditorObjectSet eo_set = GetObjectManager().GetSelectedObjects();
		foreach (EditorObject editor_object: eo_set) {
			action.InsertUndoParameter(editor_object, editor_object.TransformBeforeDrag);
			action.InsertRedoParameter(editor_object, editor_object.GetTransformArray());
		}
		
		GetObjectManager().InsertAction(action);
		
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
		
		// Disabled cause its very buggy
		//rotation_widget.SetRotation(ori);
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
		vector cursor_position = GetUIManager().GetEditorUI().GetMapWidget().ScreenToMap(Vector(mouse_x, mouse_y, 0));
		cursor_position[1] = object_position[1] - GetGame().SurfaceY(object_position[0], object_position[2]) + GetGame().SurfaceY(cursor_position[0], cursor_position[2]);
		target.SetPosition(cursor_position);
		target.Update();
		
		foreach (EditorObject selected_object: GetObjectManager().GetSelectedObjects()) {
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
		//Editor.DebugObject0.SetPosition(cursor_position);

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
	
		target.SetTransformWithSnapping(object_transform);
		target.Update();
		
		// This handles all other selected objects
		EditorObjectSet selected_objects = GetObjectManager().GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			
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
	
	
	
	bool OnKeyPress(int key) 
	{
		Input input = GetGame().GetInput();
		switch (key) {
			
			case KeyCode.KC_F1: {
				EditorEvents.PlaceableCategoryChangedInvoke(this, PlaceableObjectCategory.BUILDING);
				return true;
			}
			
			case KeyCode.KC_F2: {
				EditorEvents.PlaceableCategoryChangedInvoke(this, PlaceableObjectCategory.VEHICLE);
				return true;
			}
			
			case KeyCode.KC_F3: {
				EditorEvents.PlaceableCategoryChangedInvoke(this, PlaceableObjectCategory.ENTITY);
				return true;
			}
			
			case KeyCode.KC_F4: {
				EditorEvents.PlaceableCategoryChangedInvoke(this, PlaceableObjectCategory.HUMAN);
				return true;
			}
						
			case KeyCode.KC_F5: {
				// Create Character on cursor and select them
				set<Object> o;
				vector v = MousePosToRay(o);
				GetUIManager().SetEditorCameraActive(false);
				GetGame().SelectPlayer(null, EditorPlayer);
				EditorPlayer.SetPosition(v);
				PlayerActive = true;
				GetUIManager().GetEditorUI().GetRoot().Show(false);
				break;
			}
			
			case KeyCode.KC_F6: {
				// Deselect character
				GetUIManager().SetEditorCameraActive(true);
				PlayerActive = false;
				GetUIManager().GetEditorUI().GetRoot().Show(true);
				break;
			}

			
			case KeyCode.KC_ESCAPE: {
				
				if (GetFocus()) {
					SetFocus(null);
					//return true;
				} else if (m_LootEditMode) {
					PlaceholderRemoveLootMode();
				} else {
					
					//m_UIManager.GetMenu().GetVisibleMenu() != "PauseMenu"
					// maybe something like this idk just add better escape func
				}
				break;
			}
			
			case KeyCode.KC_DELETE: {
				GetObjectManager().DeleteSelection();				
				return true;
			}
			
			case KeyCode.KC_M: {
								
				GetUIManager().GetEditorUI().ShowMap(!GetUIManager().GetEditorUI().IsMapOpen());
				return true;
			}
		
			case KeyCode.KC_SPACE: {
				if (GetGame().GetUIManager().IsCursorVisible() && !GetUIManager().GetEditorUI().IsMapOpen()) {
					GetUIManager().GetEditorUI().HideCursor();
					if (Editor.IsPlayerActive()) {
						//GetGame().GetPlayer().GetInputController().SetDisabled(false);
						Editor.SetPlayerAimLock(false);
					}
				} else { 
					GetUIManager().GetEditorUI().ShowCursor();
					if (Editor.IsPlayerActive()) {
						//GetGame().GetPlayer().GetInputController().SetDisabled(true);
						Editor.SetPlayerAimLock(true);
					}
				}
				return true;
			}
			
			case KeyCode.KC_U: {
				EditorSettings.MAGNET_PLACEMENT = !EditorSettings.MAGNET_PLACEMENT;
				SetFocus(null);
				return true;
			}
			
			case KeyCode.KC_Y: {
				
				if (input.LocalValue("UAWalkRunTemp")) {
					Redo();
				} else {
					GetEditor().GetUIManager().SetVisibility(!GetEditor().GetUIManager().GetVisibility());
				}
				return true;
			}
			
			case KeyCode.KC_G: {
				EditorSettings.MAINTAIN_HEIGHT = !EditorSettings.MAINTAIN_HEIGHT;
				SetFocus(null);
				return true;
			}

			
			case KeyCode.KC_Z: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Undo();
					return true;
				}
				break;
			}
			
			
			case KeyCode.KC_A: {
				if (input.LocalValue("UAWalkRunTemp")) {
					
					GetObjectManager().SelectObjects(GetObjectManager().GetPlacedObjects());
					return true;
				}
				break;
			}
			
			case KeyCode.KC_X: {
				if (input.LocalValue("UAWalkRunTemp")) {
					int r = GetObjectManager().CutSelection();
					Print(string.Format("Cut %1 Objects", r));
					return true;
				}
				break;
			}
			
			case KeyCode.KC_C: {
				if (input.LocalValue("UAWalkRunTemp")) {
					r = GetObjectManager().CopySelection();
					Print(string.Format("Copied %1 Objects", r));
					return true;
				}
				break;
			}
			
			case KeyCode.KC_V: {
				if (input.LocalValue("UAWalkRunTemp")) {
					r = GetObjectManager().PasteSelection();
					Print(string.Format("Pasted %1 Objects", r));
					return true;
				}
				break;
			}
			
			case KeyCode.KC_E: {
				if (input.LocalValue("UAWalkRunTemp")) {
					//GetUIManager().GetEditorUI().ShowExportWindow();
					
					// todo once UI is created, add "Export Selected Only"
					ref EditorObjectSet export_objects = GetEditor().GetObjectManager().GetPlacedObjects();
					EditorFileManager.ExportToFile(export_objects, ExportMode.COMFILE, "export_server");
					EditorFileManager.ExportToFile(export_objects, ExportMode.EXPANSION, "export_expansion");
					EditorFileManager.ExportToFile(export_objects, ExportMode.TERRAINBUILDER, "export_terrainbuilder", HeightType.ABSOLUTE);
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
			
			case KeyCode.KC_R: {
				if (input.LocalValue("UAWalkRunTemp")) {
					EditorEventManager.ImportEventPositions();
					return true;
				}
				break;
			}
			
			default: {
				Print("DEFAULT CASE: " + key);
				break;
			}
			
			
			

		}
		
		// todo add increment size in ui
		EditorObjectSet selectedobjects = GetObjectManager().GetSelectedObjects();
		foreach (EditorObject selected_objects: selectedobjects) {
			
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
		
		return false;
	}
	
	void IncrementMove(EditorObject obj, int axis, float move)
	{
		vector pos = obj.GetPosition();
		pos[axis] = pos[axis] + move;
		obj.SetPosition(pos);
		obj.Update();
		GlobalTranslationWidget.UpdatePosition();
		
	}
	
	
	static bool IsPlayerActive()
	{
		return PlayerActive;
	}
	
	// Remove this once you find an actual solution kekw
	static void SetPlayerAimLock(bool state)
	{
		GetGame().GetPlayer().GetInputController().OverrideAimChangeX(state, 0);
		GetGame().GetPlayer().GetInputController().OverrideAimChangeY(state, 0);
	}
}










