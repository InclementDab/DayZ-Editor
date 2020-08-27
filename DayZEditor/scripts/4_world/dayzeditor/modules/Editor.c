
private ref EditorClientModule m_EditorInstance;
EditorClientModule GetEditor() 
{ 	
	//if (m_EditorInstance == null)
	//	m_EditorInstance = new Editor();
	return m_EditorInstance;
}

bool IsEditor() { return m_EditorInstance != null; }


enum EditorClientModuleRPC
{
	INVALID = 36114,
	COUNT
};


enum BetterMouseState {
	LEFT = 1,
	RIGHT = 2,
	MIDDLE = 4
};

class EditorClientModule: JMModuleBase
{
	/* Private Members */
	private Mission m_Mission;
	private UIManager m_UIManager;
	private EditorCamera m_Camera;
	private PlayerBase m_Player;

	
	static Object							ObjectUnderCursor = null;
	static EditorObject 					EditorObjectUnderCursor = null;
	static vector 							CurrentMousePosition;
	
	private bool m_Active = false;
	bool IsActive() { return m_Active; }
	
	private int ActiveMouseStates;

	/* UI Stuff */
	EditorHud GetEditorHud() 
		return m_EditorHud;
	
	EditorCamera GetCamera() 
		return m_Camera;
	
	EditorSettings GetSettings()
		return GetModuleManager().GetModule(EditorSettings);
	
	EditorObjectSet GetSelectedObjects() 
		return m_SelectedObjects; 
	
	EditorObjectSet GetPlacedObjects()
		return m_PlacedObjects; 
	
	EditorObjectDataSet	 GetSessionCache()
		return m_SessionCache; 
	
	bool IsPlacing() { return m_ObjectInHand != null; }

	private ref EditorHud						m_EditorHud;
	
	private ref EditorHologram 					m_ObjectInHand;
	private ref EditorBrush						m_EditorBrush;
	
	private ref EditorObjectSet 				m_PlacedObjects;
	private ref EditorObjectSet					m_SelectedObjects;
	private ref EditorObjectDataSet			 	m_SessionCache;
	private ref set<ref EditorAction> 			m_ActionStack;
			
	// Public Members

	void EditorClientModule()
	{
		EditorLog.Info("Editor");
		m_EditorInstance = this;
	}
	
	void ~EditorClientModule()
	{
		EditorLog.Info("~Editor");
	}
	
	
	// JMModuleBase Overrides
	override void OnInit()
	{
		EditorLog.Trace("Editor::OnInit");
		
		m_PlacedObjects = new EditorObjectSet();
		m_SelectedObjects = new EditorObjectSet();
		m_SessionCache = new EditorObjectDataSet();
		m_ActionStack = new set<ref EditorAction>();
		
		// Init UI
		m_UIManager = GetGame().GetUIManager();
		m_EditorHud = new EditorHud();
		m_EditorHud.Init(null);
		
		// Events
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
		EditorEvents.OnObjectDeleted.Insert(OnObjectDeleted);
		
		// Keybinds
		RegisterBinding(new JMModuleBinding("OnEditorToggleActive", "EditorToggleActive"));
		RegisterBinding(new JMModuleBinding("OnEditorToggleCursor", "EditorToggleCursor"));
		RegisterBinding(new JMModuleBinding("OnEditorToggleUI", "EditorToggleUI"));
		
		RegisterBinding(new JMModuleBinding("OnEditorToggleMap", "EditorToggleMap"));
	}
	
	private bool exit_condition = false;
	private float avg_timeslice;
	private int timeslice_count;
	
	override void OnUpdate(float timeslice)
	{
		if (m_Camera != null && !IsMissionOffline()) {
			ScriptRPC update_rpc = new ScriptRPC();
			update_rpc.Write(m_Camera.GetPosition());
			update_rpc.Write(m_Camera.GetOrientation());
			update_rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_UPDATE, true);
		}
		
		
		set<Object> obj = new set<Object>();
		int x, y;
		GetMousePos(x, y);
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
		
		
		
		typename mouse_state_type = BetterMouseState;
		// Handles OnMouseDown and OnMouseUp events
		for (int i = 0; i < 3; i++) {
			int mouse_state;
			mouse_state_type.GetVariableValue(null, i, mouse_state);
			if (GetMouseState(i) & MB_PRESSED_MASK) {
				if ((ActiveMouseStates & mouse_state) == 0) {
					ActiveMouseStates |= mouse_state;
					OnMouseDown(mouse_state);
				}
			} else { 
				if ((ActiveMouseStates & mouse_state) == mouse_state) {
					ActiveMouseStates &= ~mouse_state;
					OnMouseUp(mouse_state);
				}
			}
		}
		
	
		
		
		
		
		// debug
		timeslice_count++;
		avg_timeslice = avg_timeslice + ((timeslice - avg_timeslice) / timeslice_count);
		EditorLog.CurrentLogLevel = EditorLogLevel.WARNING;
		m_EditorHud.GetController().DebugText1 = avg_timeslice.ToString();
		m_EditorHud.GetController().NotifyPropertyChanged("DebugText1");
		EditorLog.CurrentLogLevel = EditorLogLevel.TRACE;
	}
	
	override bool IsServer() { return false; }	

	override void OnInvokeConnect(PlayerBase player, PlayerIdentity identity)
	{
		EditorLog.Trace("Editor::OnInvokeConnect");		
		m_Player = player;
		EditorLog.Debug(m_Player);
	}
		
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");
		m_Mission = GetGame().GetMission();
		
		// Only runs in Singleplayer
		if (IsMissionOffline()) {
			EditorLog.Info("Loading Offline Editor...");
			
			// Random cam position smile :)
			float x = Math.RandomInt(3500, 8500);
			float z = Math.RandomInt(3500, 8500);
			float y = GetGame().SurfaceY(x, z);
			m_Player = CreateDefaultCharacter(Vector(x, y, z));
			m_Active = true;
			SetActive(m_Active);
		} else {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
		}
	}
	
	override void OnMissionFinish()
	{
		EditorLog.Trace("Editor::OnMissionFinish");
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED, true);
		}
	}
	
	override void OnMissionLoaded()
	{
		EditorLog.Trace("Editor::OnMissionLoaded");
	}
	
	
	// Inputs
	
	private bool ShouldProcessInput(UAInput input)
	{
		// Check if LocalPress, Check if LControl is pressed, Check if game is focused
		return (input.LocalPress() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus());
	}
	
	private void OnEditorToggleActive(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleActive");
		
		m_Active = !m_Active;
		SetActive(m_Active);
	}	
	
	private void OnEditorToggleCursor(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleCursor");
		
		// Dont want to toggle cursor on map
		if (m_EditorHud.IsMapVisible()) return;
		m_EditorHud.ToggleCursor();
	}	
	

	private void OnEditorToggleUI(UAInput input)
	{		
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleUI");
		
		m_EditorHud.Show(!m_EditorHud.IsVisible());
	}	
	
	private void OnEditorToggleMap(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleMap");
		
		m_EditorHud.ShowMap(!m_EditorHud.IsMapVisible());
		m_EditorHud.ShowCursor();
		
		EditorEvents.MapToggled(this, m_EditorHud.GetMap(), m_EditorHud.IsMapVisible());
	}
	
	
	
	
	void OnMouseDown(int button)
	{
		EditorLog.Trace("Editor::OnMouseDown %1", typename.EnumToString(BetterMouseState, button));
		
		switch (button) {
			
			case BetterMouseState.LEFT: {
				

				
				break;
			}
			
			case BetterMouseState.MIDDLE: {
				
				if (KeyState(KeyCode.KC_LCONTROL))
					EditorLog.Info(GetWidgetUnderCursor().GetName());
				
				break;
			}
		}
	}
	
	void OnMouseUp(int button)
	{
		EditorLog.Trace("Editor::OnMouseUp %1", typename.EnumToString(BetterMouseState, button));
	}
	
	
	// Only use this to handle hardcoded keys (ctrl + z etc...)
	// Return TRUE if handled.
	bool OnKeyPress(int key)
	{		
		// LControl Commands
		if (KeyState(KeyCode.KC_LCONTROL)) {
			
			switch (key) {
				case KeyCode.KC_Z: {
					Undo();
					return true;
				}
				
				case KeyCode.KC_Y: {
					Redo();
					return true;
				}				
			}
		} else {
			
			switch (key) {
				
				case KeyCode.KC_ESCAPE: {
					ActiveMouseStates = 0;
					if (m_EditorHud.IsModalActive()) {
						m_EditorHud.ModalClose();
						return true;
					}
					
					break;
				}
				
				
			}
			
		}
		
		return false;
	}
			
	
	// RPC stuff
	override int GetRPCMin() 
		return EditorClientModuleRPC.INVALID;
	

	override int GetRPCMax()
		return EditorClientModuleRPC.COUNT;
	
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	{
		switch (rpc_type) {
			
		
		}
	}
	
	// Object Management
	void CreateObjects(ref EditorObjectDataSet data_list, bool create_undo = true)
	{
		EditorLog.Trace("Editor::CreateObjects");

		EditorAction action = new EditorAction("Delete", "Create");
		foreach (EditorObjectData editor_object_data: data_list) {
			
			EditorObject editor_object = new EditorObject(editor_object_data);
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));			
		}
		
		if (create_undo) {
			InsertAction(action);
		} else {
			delete action;
		}
		
		EditorEvents.ObjectCreated(this, editor_object);
	}
	
	
	EditorObject CreateObject(ref EditorObjectData editor_object_data, bool create_undo = true)
	{		
		EditorLog.Trace("Editor::CreateObject");
		
		EditorObject editor_object = new EditorObject(editor_object_data);
		EditorEvents.ObjectCreated(this, editor_object);
		
		if (!create_undo) return editor_object;
		
		EditorAction action = new EditorAction("Delete", "Create");;
		action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		InsertAction(action);
			
	
		return editor_object;
	}

	void DeleteObject(EditorObject target, bool create_undo = true)
	{
		EditorLog.Trace("Editor::DeleteObject");
		EditorEvents.ObjectDeleted(this, target);
		
		if (create_undo) {
			EditorAction action = new EditorAction("Create", "Delete");
			action.InsertUndoParameter(target, new Param1<int>(target.GetID()));
			action.InsertRedoParameter(target, new Param1<int>(target.GetID()));
			InsertAction(action);
		}
		
		delete target;
	}
	
	void DeleteObjects(EditorObjectSet target, bool create_undo = true)
	{
		EditorLog.Trace("Editor::DeleteObjects");
		
		if (create_undo) EditorAction action = new EditorAction("Create", "Delete");
		
		foreach (EditorObject editor_object: target) {
			EditorEvents.ObjectDeleted(this, editor_object);
			if (create_undo) {
				action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
				action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			}
			
			delete editor_object;
		}	
			
		if (create_undo) InsertAction(action);
	}
	
	// Call to select an object
	void SelectObject(EditorObject target)
	{
		EditorLog.Trace("Editor::SelectObject");
		m_SelectedObjects.InsertEditorObject(target);
		EditorEvents.ObjectSelected(this, target);
		target.OnSelected();
	}
	
	// Call to deselect an object
	void DeselectObject(EditorObject target)
	{
		EditorLog.Trace("Editor::DeselectObject");
		m_SelectedObjects.RemoveEditorObject(target);
		EditorEvents.ObjectDeselected(this, target);
		target.OnDeselected();
	}	
	
	// Call to toggle selection
	void ToggleSelection(EditorObject target)
	{
		EditorLog.Trace("Editor::ToggleSelection");
		if (target.IsSelected())
			DeselectObject(target);
		else
			SelectObject(target);
		
	}
	
	// Call to clear selection
	void ClearSelection()
	{
		EditorLog.Trace("Editor::ClearSelection");		
		foreach (EditorObject editor_object: m_SelectedObjects)
			DeselectObject(editor_object);
	}
	
	
	// Call to enable / disable editor
	void SetActive(bool active)
	{	
		if (m_Camera == null) {

			EditorLog.Info("Initializing Camera");
			
			// Init Spawn Position
			TIntArray center_pos = new TIntArray();		
			string world_name;
			GetGame().GetWorldName(world_name);
			GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", world_name), center_pos);
			
			// Camera Init
			// todo if singleplayer spawn on center of map, otherwise spawn on character in MP
			vector pos = m_Player.GetPosition();
			m_Camera = GetGame().CreateObjectEx("EditorCamera", pos, ECE_LOCAL);
			
			
			// Init Camera Map Marker
			/*
			EditorCameraMapMarker CameraMapMarker = new EditorCameraMapMarker();
			Widget m_MapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorCameraMapMarker.layout");
			m_MapMarkerWidget.GetScript(CameraMapMarker);
			CameraMapMarker.SetCamera(m_Camera, m_EditorUI.GetMapWidget());
			m_EditorUI.InsertMapObject(m_MapMarkerWidget);
			m_EditorUI.GetMapWidget().SetMapPos(Vector(center_pos[0], y_level, center_pos[1]));*/
		}
		
		m_Camera.SetLookEnabled(active);
		m_Camera.SetMoveEnabled(active);
		m_Camera.SetActive(active);
		
		m_EditorHud.Show(active);
		m_Mission.GetHud().Show(!active);
		m_Mission.GetHud().ShowHud(!active);
		m_Mission.GetHud().ShowHudUI(!active);
		m_Mission.GetHud().SetPermanentCrossHair(!active);
	
		
		if (!active) {	
			GetGame().SelectPlayer(m_Player.GetIdentity(), m_Player);
		}
		
		m_Player.GetInputController().SetDisabled(active);
			
		
	}
	
	bool OnMouseEnterObject(Object target, int x, int y)
	{

	}
	
	bool OnMouseExitObject(Object target, int x, int y)
	{

	}
	
	
	void InsertAction(EditorAction target)
	{
		int count = m_ActionStack.Count();
		for (int i = 0; i < count; i++) {
			if (m_ActionStack[i].IsUndone()) {
				m_ActionStack.Remove(i);
				i--; count--;
			}
		}
			
		// Adds to bottom of stack
		m_ActionStack.InsertAt(target, 0);
		
		// debug
//		GetEditorHudController().DebugActionStackListbox.Insert(new TStringClassPair(target.GetName(), target));
	}
	

	

	
	void OnObjectCreated(Class context, EditorObject target)
	{
		//m_SelectedObjects.InsertEditorObject(target);
		m_PlacedObjects.InsertEditorObject(target);
		m_PlacedObjectIndex.Insert(target.GetWorldObject().GetID(), target.GetID());
	}	
	
	void OnObjectDeleted(Class context, EditorObject target)
	{
		m_SelectedObjects.RemoveEditorObject(target);
		m_PlacedObjects.RemoveEditorObject(target);
	}
	
	// This is for hashmap lookup of placed objects. very fast
	private ref map<int, int> m_PlacedObjectIndex = new map<int, int>();
	
	EditorObject GetEditorObject(notnull Object world_object) { 
		return GetEditorObject(m_PlacedObjectIndex.Get(world_object.GetID())); 
	}
	
	EditorObject GetEditorObject(int id) { 
		return m_PlacedObjects.Get(id); 
	}
	
	void DeleteSessionData(int id) { 
		m_SessionCache.Remove(id);	
	}
	
	EditorObjectData GetSessionDataById(int id) { 
		return m_SessionCache.Get(id); 
	}
	
	EditorObject GetPlacedObjectById(int id) { 
		return m_PlacedObjects.Get(id); 
	}
		
	void SetBrush(EditorBrush brush) { 
		m_EditorBrush = brush; 
	}
	
	EditorBrush GetBrush() { 
		return m_EditorBrush; 
	}
	
	
	void CreateInHand(EditorPlaceableObjectData data)
	{
		EditorLog.Trace("Editor::CreateInHand");
		
		// Turn Brush off when you start to place
		if (m_EditorBrush != null)
			SetBrush(null);
		
		ClearSelection();
		m_ObjectInHand = new EditorHologram(data.Type, CurrentMousePosition);
		
		EditorEvents.StartPlacing(this, data);
		
	}
	
	void PlaceObject()
	{
		Input input = GetGame().GetInput();
		EntityAI e = m_ObjectInHand.GetProjectionEntity();
		vector mat[4];
	
		EditorObject editor_object = CreateObject(EditorObjectData.Create(e.GetType(), e.GetPosition(), e.GetOrientation()));
		SelectObject(editor_object);
		
		if (!input.LocalValue("UATurbo")) { 
			delete m_ObjectInHand;
		}
		
		EditorEvents.StopPlacing(this);
	}
	
	
	void Undo()
	{
		EditorLog.Trace("Editor::Undo");
		foreach (EditorAction action: m_ActionStack)
			if (!action.IsUndone())
				action.CallUndo();
	}
	
	void Redo()
	{
		EditorLog.Trace("Editor::Redo");
		for (int i = m_ActionStack.Count() - 1; i >= 0; i--)
			if (m_ActionStack[i] != null && m_ActionStack[i].IsUndone())
				m_ActionStack[i].CallRedo();
		
	}
	
	
	
	
	// probably have an EditorMode enum with NORMAL, CHARACTER, LOOTEDITOR or something
	
	private Object m_LootEditTarget;
	private bool m_LootEditMode;
	private vector m_PositionBeforeLootEditMode;
	void PlaceholderForEditLootSpawns(string name)
	{
		m_LootEditTarget = GetGame().CreateObjectEx(name, Vector(0, 1000, 0), ECE_NONE);
		
		m_PositionBeforeLootEditMode = m_Camera.GetPosition();
		m_Camera.SetPosition(Vector(10, 1000, 10));		
		
		ref EditorMapGroupProto proto_data = new EditorMapGroupProto(m_LootEditTarget); 
		EditorXMLManager.LoadMapGroupProto(proto_data);
		m_LootEditMode = true;
		EditorSettings.COLLIDE_ON_DRAG = true;
	}
	
	void PlaceholderRemoveLootMode()
	{
		IEntity child = m_LootEditTarget.GetChildren();
		while (child != null) {
			GetGame().ObjectDelete(Object.Cast(child));
			child = child.GetSibling();
		}
		
		GetGame().ObjectDelete(m_LootEditTarget);
		
		m_Camera.SetPosition(m_PositionBeforeLootEditMode);
		//camera.SelectTarget(null);
		m_LootEditMode = false;
		EditorSettings.COLLIDE_ON_DRAG = false;
	}
	
	bool IsLootEditActive() { return m_LootEditMode; }
	
	
	/*
	// Handles Marker Drag from Object Base Marker
	void ObjectDragUpdate(notnull EditorObject target)
	{

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
		DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, ground, ground_dir, component, o, NULL, target.GetWorldObject(), false, true); // set to ground only

		vector cursor_position = MousePosToRay(o, target.GetWorldObject(), EditorSettings.OBJECT_VIEW_DISTANCE, 0, !EditorSettings.COLLIDE_ON_DRAG);
		vector surface_normal = GetGame().SurfaceGetNormal(ground[0], ground[2]);
		float surface_level = GetGame().SurfaceY(ground[0], ground[2]);
	
		// debug
		Editor.DebugObject0.SetPosition(cursor_position);

		// Handle Z only motion
		
		if (input.LocalValue("UALookAround")) {	
			
		
		// Handle XY Rotation
		} else if (input.LocalValue("UATurbo")) {

		// Handle regular motion
		} else {			
			
			object_transform[0] = "1 0 0";
			object_transform[1] = "0 1 0";
			object_transform[2] = "0 0 1";
			target.PlaceOnSurfaceRotated(object_transform, object_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, target.LocalAngle * -1, EditorSettings.MAGNET_PLACEMENT);
			
			
			cursor_position[1] = cursor_position[1] - object_size[1]/2;
			if (EditorSettings.MAINTAIN_HEIGHT) 
				if (EditorSettings.MAGNET_PLACEMENT)
					object_transform[3] = cursor_position + surface_normal * vector.Distance(ground, object_transform[3]);				
				else 
					object_transform[3] = cursor_position + object_transform[1] * vector.Distance(ground, object_transform[3]);
				
			else {
				object_transform[3] = cursor_position;
				object_transform[3][1] = object_transform[3][1] + object_size[1];					
			} 
		
			
		}
		
	
		target.SetTransform(object_transform);
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
			DayZPhysics.RaycastRV(selected_transform[3], selected_transform[3] + selected_transform[1] * -1000, ground, ground_dir, component, o, NULL, selected_object.GetWorldObject(), false, true); // set to ground only

			
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
		
		
		//GlobalTranslationWidget.UpdatePosition();
		//Print(TickCount(starttime) / 1000);
	}
*/
}
