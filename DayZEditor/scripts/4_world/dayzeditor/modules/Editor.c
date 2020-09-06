//Print(__LINE__);
//Print(__FILE__); // useful shit // tools.pak?????



private ref Editor m_EditorInstance;
Editor GetEditor() 
	return m_EditorInstance;


class Editor
{
	
	/* Private Members */
	private Mission m_Mission;
	private UIManager m_UIManager;
	private EditorCamera m_EditorCamera;
	private PlayerBase m_Player;
	
	
	static Object							ObjectUnderCursor = null;
	static EditorObject 					EditorObjectUnderCursor = null;
	static vector 							CurrentMousePosition;
	
	
	bool IsActive()
		return m_Active;
	
	EditorHud GetEditorHud() 
		return m_EditorHud;
	
	EditorCamera GetCamera() 
		return m_EditorCamera;
	
	EditorSettings GetSettings()
		return m_EditorSettings;
	
	EditorObjectSet GetSelectedObjects() 
		return m_SelectedObjects; 
	
	EditorObjectSet GetPlacedObjects()
		return m_PlacedObjects; 
	
	EditorObjectDataSet	 GetSessionCache()
		return m_SessionCache; 
	
	bool IsPlacing()
		return m_ObjectInHand != null; 
	
		
	EditorObject GetEditorObject(notnull Object world_object)
		return GetEditorObject(m_PlacedObjectIndex.Get(world_object.GetID())); 
	
	
	EditorObject GetEditorObject(int id)
		return m_PlacedObjects.Get(id); 
	
	
	void DeleteSessionData(int id)
		m_SessionCache.Remove(id);	
	
	
	EditorObjectData GetSessionDataById(int id)
		return m_SessionCache.Get(id); 

	
	EditorObject GetPlacedObjectById(int id)
		return m_PlacedObjects.Get(id); 
	
		
	void SetBrush(EditorBrush brush)
		m_EditorBrush = brush; 
	
	
	EditorBrush GetBrush()
		return m_EditorBrush; 
	

	private ref EditorSettings 					m_EditorSettings;
	private ref EditorHud						m_EditorHud;
	private EditorHudController 				m_EditorHudController;
	
	private ref EditorHologram 					m_ObjectInHand;
	private ref EditorBrush						m_EditorBrush;
	
	private ref EditorObjectSet 				m_PlacedObjects;
	private ref EditorObjectSet					m_SelectedObjects;
	private ref EditorObjectDataSet			 	m_SessionCache;
	private ref EditorActionStack 				m_ActionStack;
	
	private bool 								m_Active;
			
	private ref map<int, int> m_PlacedObjectIndex = new map<int, int>();
	
	
	void Editor() 
	{
		EditorLog.Trace("Editor");		
		
		m_PlacedObjects = new EditorObjectSet();
		m_SelectedObjects = new EditorObjectSet();
		m_SessionCache = new EditorObjectDataSet();
		m_ActionStack = new EditorActionStack();
		
		// Init Settings
		m_EditorSettings = EditorSettings.Load("$profile:Editor\\settings.ini");
		
		// Init UI
		m_UIManager = GetGame().GetUIManager();
		
		// Init Hud
		m_EditorHud = new EditorHud();
		m_EditorHud.Init(null);
		m_EditorHudController = m_EditorHud.GetController();
		
		m_Mission = GetGame().GetMission();
		
		if (IsMissionOffline()) {
			EditorLog.Info("Loading Offline Editor...");
			
			// Random cam position smile :)
			float x = Math.RandomInt(3500, 8500);
			float z = Math.RandomInt(3500, 8500);
			float y = GetGame().SurfaceY(x, z);
			m_Player = CreateDefaultCharacter(Vector(x, y, z));
			SetActive(true);

		} else {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
		}
		
		// Events
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
		EditorEvents.OnObjectDeleted.Insert(OnObjectDeleted);
	}
	
	void ~Editor() {
		EditorLog.Trace("~Editor");
		return;
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED, true);
		}
	}
	
	// maybe pass PlayerBase into here?
	static Editor Create()
	{
		EditorLog.Trace("Editor::Create");
		m_EditorInstance = new Editor();
		return m_EditorInstance;
	}
	
	static void Destroy()
	{
		EditorLog.Trace("Editor::Destroy");
		delete m_EditorInstance;
	}
	
	private bool exit_condition = false;
	private float avg_timeslice;
	private int timeslice_count;
	
	void Update(float timeslice)
	{
		m_EditorHud.Update(timeslice);
		
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
		
		
		EditorLog.CurrentLogLevel = EditorLogLevel.WARNING;
		if (m_EditorCamera) {
			vector cam_pos = m_EditorCamera.GetPosition();
			
			m_EditorHudController.cam_x = cam_pos[0];
			m_EditorHudController.cam_y = cam_pos[2];
			m_EditorHudController.cam_z = cam_pos[1];
			
			m_EditorHudController.NotifyPropertyChanged("cam_x");
			m_EditorHudController.NotifyPropertyChanged("cam_y");
			m_EditorHudController.NotifyPropertyChanged("cam_z");
			
		}
			
		// debug
		timeslice_count++;
		avg_timeslice = avg_timeslice + ((timeslice - avg_timeslice) / timeslice_count);
		m_EditorHudController.DebugText1 = avg_timeslice.ToString();
		m_EditorHudController.NotifyPropertyChanged("DebugText1");
		EditorLog.CurrentLogLevel = EditorLogLevel.TRACE;
	}
	
	// target CAN BE NULL HERE!!
	bool OnMouseDown(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("Editor::OnMouseDown");
		
		if (!target) {
			SetFocus(null);
		}
		
		switch (button) {
			
			case MouseState.LEFT: {


				if (IsPlacing()) {
					PlaceObject();
					return true;
				}
				
				ClearSelection();
				if (GetBrush() == null) {
					
					if (EditorObjectUnderCursor == null) {
						// delayed dragbox
						m_EditorHud.GetController().DelayedDragBoxCheck();
						return false;
						
						
					} else if (EditorObjectUnderCursor != null) {
						if (!KeyState(KeyCode.KC_LSHIFT)) {
							ClearSelection();
						}
						SelectObject(EditorObjectUnderCursor);
						return true;
					}
				}

				break;
			}
			
			case MouseState.MIDDLE: {
				
				if (KeyState(KeyCode.KC_LCONTROL))
					EditorLog.Info(GetWidgetUnderCursor().GetName());
				else if (KeyState(KeyCode.KC_LSHIFT)) {
					//EditorLog.Info();
					if (ObjectUnderCursor)
						CreateFromObject(ObjectUnderCursor);
				} else {
					
					vector pos = CurrentMousePosition;					
					pos[1] = m_EditorCamera.GetPosition()[1];
					m_EditorCamera.SetPosition(pos);
				}
				
				break;
			}
		}
		
		return false;
	}

	bool OnMouseUp(Widget target, int button, int x, int y)
	{
		return false;
	}
	
	// Only use this to handle hardcoded keys (ctrl + z etc...)
	// Return TRUE if handled.
	bool OnKeyPress(int key)
	{		
		
		m_EditorHud.OnKeyPress(key);
		
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
				
				case KeyCode.KC_A: {
					foreach (EditorObject eo: m_PlacedObjects)
						SelectObject(eo);
					return true;
				}
				
				case KeyCode.KC_N: {
					New();
					return true;
				}
				
				case KeyCode.KC_S: {
					Save();
					return true;
				}
				
				case KeyCode.KC_O: {
					Open();
					return true;
				}
				
				case KeyCode.KC_E: {
					Export();
					return true;
				}
				
				case KeyCode.KC_I: {
					Import();
					return true;
				}
				
				case KeyCode.KC_X: {
					Cut(m_SelectedObjects);
					return true;
				}

				case KeyCode.KC_C: {
					Copy(m_SelectedObjects);
					return true;
				}
				
				case KeyCode.KC_V: {
					Paste(CurrentMousePosition);
					return true;
				} 
				
				

			}
		} else {
			
			switch (key) {
				
				case KeyCode.KC_ESCAPE: {
				
					if (m_EditorHud.IsModalActive()) {
						m_EditorHud.GetModal().CloseDialog();
						return true;
					// jank
					} else if (!GetGame().GetMission().IsPaused()) {
						GetGame().GetMission().Pause();
					} else {
						GetGame().GetMission().Continue();
					}
					
					break;
				}				
			}	
		}
		
		return false;
	}
	
	
	// Object Management
	ref EditorObjectSet CreateObjects(ref EditorObjectDataSet data_list, bool create_undo = true, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		EditorLog.Trace("Editor::CreateObjects");
		EditorObjectSet object_set = new EditorObjectSet();
		EditorAction action = new EditorAction("Delete", "Create");
		foreach (EditorObjectData editor_object_data: data_list) {
			
			EditorObject editor_object = EditorObject.Create(editor_object_data, flags);
			EditorEvents.ObjectCreated(this, editor_object);
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));		
			object_set.InsertEditorObject(editor_object);	
		}
		
		if (create_undo) {
			m_ActionStack.InsertAction(action);
			m_ActionStack.UpdateDebugReadout(GetEditorHud().GetController().DebugActionStackListbox);
		} else {
			delete action;
		}
		
		return object_set;
	}
	
	
	EditorObject CreateObject(ref EditorObjectData editor_object_data, bool create_undo = true, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{		
		EditorLog.Trace("Editor::CreateObject");
		
		EditorObject editor_object = EditorObject.Create(editor_object_data, flags);
		EditorEvents.ObjectCreated(this, editor_object);
		
		if (!create_undo) return editor_object;
		
		EditorAction action = new EditorAction("Delete", "Create");;
		action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		m_ActionStack.InsertAction(action);
		m_ActionStack.UpdateDebugReadout(GetEditorHud().GetController().DebugActionStackListbox);
			
	
		return editor_object;
	}
	
	EditorObject CreateFromObject(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		EditorLog.Trace("Editor::CreateFromObject");
		EditorObject editor_object = EditorObject.Create(target, flags);
		EditorEvents.ObjectCreated(this, editor_object);
		
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
			m_ActionStack.InsertAction(action);
			m_ActionStack.UpdateDebugReadout(GetEditorHud().GetController().DebugActionStackListbox);
		}
		
		delete target;
	}
	
	void DeleteObjects(EditorObjectSet target, bool create_undo = true)
	{
		EditorLog.Trace("Editor::DeleteObjects");
		
		EditorAction action = new EditorAction("Create", "Delete");
		
		foreach (EditorObject editor_object: target) {
			EditorEvents.ObjectDeleted(this, editor_object);
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			delete editor_object;
		}	
			
		if (create_undo) {
			m_ActionStack.InsertAction(action);
			m_ActionStack.UpdateDebugReadout(GetEditorHud().GetController().DebugActionStackListbox);
		}
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
		m_Active = active;
		
		if (m_EditorCamera == null) {

			EditorLog.Info("Initializing Camera");
			
			// Init Spawn Position
			TIntArray center_pos = new TIntArray();		
			string world_name;
			GetGame().GetWorldName(world_name);
			GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", world_name), center_pos);
			
			// Camera Init
			// todo if singleplayer spawn on center of map, otherwise spawn on character in MP
			vector pos = m_Player.GetPosition();
			m_EditorCamera = GetGame().CreateObjectEx("EditorCamera", pos, ECE_LOCAL);
			
			
			// Init Camera Map Marker
			/*
			EditorCameraMapMarker CameraMapMarker = new EditorCameraMapMarker();
			Widget m_MapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorCameraMapMarker.layout");
			m_MapMarkerWidget.GetScript(CameraMapMarker);
			CameraMapMarker.SetCamera(m_Camera, m_EditorUI.GetMapWidget());
			m_EditorUI.InsertMapObject(m_MapMarkerWidget);
			m_EditorUI.GetMapWidget().SetMapPos(Vector(center_pos[0], y_level, center_pos[1]));*/
			
					
			
			GetGame().GetWorld().SetViewDistance(m_EditorSettings.ViewDistance);
			GetGame().GetWorld().SetObjectViewDistance(m_EditorSettings.ObjectViewDistance);
		}
		
		m_EditorCamera.SetLookEnabled(m_Active);
		m_EditorCamera.SetMoveEnabled(m_Active);
		m_EditorCamera.SetActive(m_Active);
		
		m_EditorHud.Show(m_Active);
		m_Mission.GetHud().Show(!m_Active);
		m_Mission.GetHud().ShowHud(!m_Active);
		m_Mission.GetHud().ShowHudUI(!m_Active);
		m_Mission.GetHud().SetPermanentCrossHair(!m_Active);
	
		
		if (!m_Active) {	
			GetGame().SelectPlayer(m_Player.GetIdentity(), m_Player);
			CreateFromObject(m_Player, EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.LISTITEM | EditorObjectFlags.MAPMARKER);
		}
		
		m_Player.GetInputController().SetDisabled(m_Active);
			
		
	}
	
	bool OnMouseEnterObject(Object target, int x, int y)
	{

	}
	
	bool OnMouseExitObject(Object target, int x, int y)
	{

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
		
	
	void CreateInHand(ref EditorPlaceableObjectData data)
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
		
		if (!KeyState(KeyCode.KC_LSHIFT)) { 
			delete m_ObjectInHand;
		}
		
		EditorEvents.StopPlacing(this);
	}
	
	
	void Undo()
	{
		EditorLog.Trace("Editor::Undo");
		foreach (EditorAction action: m_ActionStack) {
			if (!action.IsUndone()) {
				action.CallUndo();
				return;
			}
		}
	}
	
	void Redo()
	{
		EditorLog.Trace("Editor::Redo");
		for (int i = m_ActionStack.Count() - 1; i >= 0; i--) {
			if (m_ActionStack[i] != null && m_ActionStack[i].IsUndone()) {
				m_ActionStack[i].CallRedo();
				return;
			}
		}
	}
	
	void New()
	{
		EditorLog.Trace("Editor::New");
		
		MapSelectDialog select_window = new MapSelectDialog("Create New...");
		select_window.ShowDialog();
	}
	
	void Save()
	{
		EditorLog.Trace("Editor::Save");
		
		EditorWorldData world_data();
		GetGame().GetWorldName(world_data.MapName);
		m_EditorCamera.GetTransform(world_data.CameraPosition);
		
		foreach (EditorObject editor_object: m_PlacedObjects) {
			world_data.EditorObjects.InsertEditorData(editor_object.GetData());
		}
		
		EditorFileSaveDialog save_dialog = new EditorFileSaveDialog();
		string file = save_dialog.ShowFileDialog();
		
		FileDialogResult result = EditorFileManager.Save(world_data, file);
		
		m_EditorHud.GetController().NotificationCreate("Save " + typename.EnumToString(FileDialogResult, result), COLOR_GREEN); 
		
	}
	
	void Open()
	{
		EditorLog.Trace("Editor::Open");
	}
	
	void Import()
	{
		EditorLog.Trace("Editor::Import");
	}
	
	void Export()
	{
		EditorLog.Trace("Editor::Export");
		
		EditorFileExportDialog export_dialog();
		export_dialog.ShowFileDialog();
	}
	
	void Cut(EditorObjectSet target_objects)
	{
		EditorLog.Trace("Editor::Cut");
		EditorClipboard.Cut(target_objects);
	}
	
	void Copy(EditorObjectSet target_objects) 
	{
		EditorLog.Trace("Editor::Copy");
		EditorClipboard.Copy(target_objects);
	}
	
	void Paste(vector position)
	{
		EditorLog.Trace("Editor::Paste");
		EditorClipboard.Paste(position);
	}
	
	
	
	// probably have an EditorMode enum with NORMAL, CHARACTER, LOOTEDITOR or something
	
	private Object m_LootEditTarget;
	private bool m_LootEditMode;
	private vector m_PositionBeforeLootEditMode;
	void PlaceholderForEditLootSpawns(string name)
	{
		m_LootEditTarget = GetGame().CreateObjectEx(name, Vector(0, 1000, 0), ECE_NONE);
		
		m_PositionBeforeLootEditMode = m_EditorCamera.GetPosition();
		m_EditorCamera.SetPosition(Vector(10, 1000, 10));		
		
		ref EditorMapGroupProto proto_data = new EditorMapGroupProto(m_LootEditTarget); 
		EditorXMLManager.LoadMapGroupProto(proto_data);
		m_LootEditMode = true;
		// todo
		//EditorSettings.COLLIDE_ON_DRAG = true;
	}
	
	void PlaceholderRemoveLootMode()
	{
		IEntity child = m_LootEditTarget.GetChildren();
		while (child != null) {
			GetGame().ObjectDelete(Object.Cast(child));
			child = child.GetSibling();
		}
		
		GetGame().ObjectDelete(m_LootEditTarget);
		
		m_EditorCamera.SetPosition(m_PositionBeforeLootEditMode);
		//camera.SelectTarget(null);
		m_LootEditMode = false;
		// todo
		//EditorSettings.COLLIDE_ON_DRAG = false;
	}
	
	bool IsLootEditActive() { 
		return m_LootEditMode; 
	}

			
}

