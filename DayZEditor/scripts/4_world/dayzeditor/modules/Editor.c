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
	
	void SetSettings(EditorSettings settings) {
		m_EditorSettings = settings;
		EditorSettings.Save(m_EditorSettings, m_EditorSettingsFile);
		m_EditorSettings.Reload();
	}
	
	EditorObjectManagerModule GetObjectManager()
		return EditorObjectManagerModule.Cast(GetModuleManager().GetModule(EditorObjectManagerModule));
	
	EditorObjectSet GetSelectedObjects() 
		return GetObjectManager().GetSelectedObjects(); 
	
	EditorObjectSet GetPlacedObjects()
		return GetObjectManager().GetPlacedObjects(); 

	EditorObjectDataSet GetSessionCache()
		return m_SessionCache; 
		
	EditorObject GetEditorObject(int id)
		return GetObjectManager().GetEditorObject(id); 
	
	EditorObject GetEditorObject(notnull Object world_object)
		return GetObjectManager().GetEditorObject(world_object);
	
	EditorObject GetPlacedObjectById(int id)
		return GetObjectManager().GetPlacedObjectById(id); 

	
	
	void DeleteSessionData(int id)
		m_SessionCache.Remove(id);	
	
	
	EditorObjectData GetSessionDataById(int id)
		return m_SessionCache.Get(id); 

	void SetBrush(EditorBrush brush)
		m_EditorBrush = brush; 
	
	
	EditorBrush GetBrush()
		return m_EditorBrush; 
	
	bool IsPlacing()
		return m_ObjectInHand != null; 
	
	
	ref EditorObjectSet CreateObjects(ref EditorObjectDataSet data_list, bool create_undo = true)
		return GetObjectManager().CreateObjects(data_list, create_undo);
	
	
	EditorObject CreateObject(ref EditorObjectData editor_object_data, bool create_undo = true)
		return GetObjectManager().CreateObject(editor_object_data, create_undo);
	
	
	EditorObject CreateFromObject(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
		return GetObjectManager().CreateFromObject(target, flags);
	
	
	void DeleteObject(EditorObject target, bool create_undo = true)
		GetObjectManager().DeleteObject(target, create_undo);
	
	
	void DeleteObjects(EditorObjectSet target, bool create_undo = true)
		GetObjectManager().DeleteObjects(target, create_undo);
	
	
	void SelectObject(EditorObject target)
		GetObjectManager().SelectObject(target);
	
	
	void DeselectObject(EditorObject target)
		GetObjectManager().DeselectObject(target);
	
	
	void ToggleSelection(EditorObject target)
		GetObjectManager().ToggleSelection(target);
	
		
	void ClearSelection() 
		GetObjectManager().ClearSelection();
	
	

	private ref EditorSettings 					m_EditorSettings;
	private ref EditorHud						m_EditorHud;
	private EditorHudController 				m_EditorHudController;
	
	private ref EditorHologram 					m_ObjectInHand;
	private ref EditorBrush						m_EditorBrush;

	private ref EditorObjectDataSet			 	m_SessionCache;
	
	
	private bool 								m_Active;
	private string 								m_EditorSettingsFile = "$profile:Editor\\settings.ini";
	private string								m_EditorSaveFile;
	
	
	void Editor(PlayerBase player) 
	{
		EditorLog.Trace("Editor");		
		m_Player = player;
	
		m_SessionCache = new EditorObjectDataSet();
		
		
		// Init Settings
		m_EditorSettings = EditorSettings.Load(m_EditorSettingsFile);
		m_EditorSettings.Reload();
		
		// Init UI
		m_UIManager = GetGame().GetUIManager();
		
		// Init Hud
		m_EditorHud = new EditorHud();
		m_EditorHud.Init(null);
		m_EditorHudController = m_EditorHud.GetController();
		
		m_Mission = GetGame().GetMission();
		
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
		}

		
		SetActive(true);
	}
	
	void ~Editor() {
		EditorLog.Trace("~Editor");
		SetActive(false);
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED, true);
		}
	}
	

	static Editor Create(PlayerBase player)
	{
		EditorLog.Trace("Editor::Create");
		m_EditorInstance = new Editor(player);
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
		avg_timeslice = avg_timeslice + ((ftime - avg_timeslice) / timeslice_count);
		m_EditorHudController.DebugText1 = avg_timeslice.ToString();
		m_EditorHudController.NotifyPropertyChanged("DebugText1");
		EditorLog.CurrentLogLevel = EditorLogLevel.TRACE;
	}
	
	
	bool OnMouseDown(int button)
	{
		EditorLog.Trace("Editor::OnMouseDown");
		
		Widget target = GetWidgetUnderCursor();
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
					
					if (EditorObjectUnderCursor) {
						if (!KeyState(KeyCode.KC_LSHIFT)) {
							ClearSelection();
						}
						SelectObject(EditorObjectUnderCursor);
						return true;
					} else if (!target) {
						GetEditorHud().GetController().DelayedDragBoxCheck();
						
					}
				}

				break;
			}
			
			case MouseState.MIDDLE: {
				
				if (KeyState(KeyCode.KC_LCONTROL))
					EditorLog.Info(GetWidgetUnderCursor().GetName());
				else if (KeyState(KeyCode.KC_LSHIFT)) {

					if (ObjectUnderCursor) {
						Object obj = ObjectUnderCursor;
						while (obj.GetParent())
							obj = obj.GetParent();
						
						CreateFromObject(obj);
					}
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

	bool OnMouseUp(int button)
	{
		return false;
	}
	
	// Only use this to handle hardcoded keys (ctrl + z etc...)
	// Return TRUE if handled.
	bool OnKeyPress(int key)
	{		
		
		//m_EditorHud.OnKeyPress(key);
		
		// LControl Commands
		if (KeyState(KeyCode.KC_LCONTROL)) {
			
			switch (key) {
				case KeyCode.KC_Z: {
					GetObjectManager().Undo();
					return true;
				}
				
				case KeyCode.KC_Y: {
					GetObjectManager().Redo();
					return true;
				}
				
				case KeyCode.KC_A: {
					EditorObjectSet placed_objects = GetPlacedObjects();
					foreach (EditorObject eo: placed_objects)
						SelectObject(eo);
					return true;
				}
				
				case KeyCode.KC_N: {
					New();
					return true;
				}
				
				case KeyCode.KC_S: {
					
					
					EditorWorldData world_data(this);
					
					if (m_EditorSaveFile == string.Empty || KeyState(KeyCode.KC_LSHIFT)) {
						EditorFileSaveDialog save_dialog = new EditorFileSaveDialog(world_data);
						string file = save_dialog.ShowFileDialog();
					} else Save(m_EditorSaveFile, world_data);
					
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
					Cut(GetSelectedObjects());
					return true;
				}

				case KeyCode.KC_C: {
					Copy(GetSelectedObjects());
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
	
	

	
	bool OnKeyRelease(int key)
	{
		return false;
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
			
				
			
			// Registers character as EditorObject
			//CreateFromObject(m_Player, EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.LISTITEM | EditorObjectFlags.MAPMARKER);
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
		}
		
		m_Player.GetInputController().SetDisabled(m_Active);
			
		
	}
	
	bool OnMouseEnterObject(Object target, int x, int y)
	{

	}
	
	bool OnMouseExitObject(Object target, int x, int y)
	{

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
	
	

	
	void New()
	{
		EditorLog.Trace("Editor::New");
		
		MapSelectDialog select_window = new MapSelectDialog("Create New...");
		select_window.ShowDialog();
	}
	
	void Save(string file, EditorWorldData world_data)
	{
		EditorLog.Trace("Editor::Save");
		
		FileDialogResult result = EditorFileManager.Save(world_data, file);
		m_EditorHud.GetController().NotificationCreate("Save " + typename.EnumToString(FileDialogResult, result), COLOR_GREEN); 
		
		if (result == FileDialogResult.SUCCESS) {
			m_EditorSaveFile = file;
		}		
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

