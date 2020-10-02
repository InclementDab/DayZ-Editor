//Print(__LINE__);
//Print(__FILE__); // useful shit // tools.pak?????
//Print(__void);
//local string test;
//Print(FLT_MAX);
//Print(FLT_MIN);
//__NULL_FUNCT


/*



<rootclass name="DefaultWeapon" /> <!-- weapons -->
<rootclass name="DefaultMagazine" /> <!-- magazines -->
<rootclass name="Inventory_Base" /> <!-- inventory items -->
<rootclass name="HouseNoDestruct" reportMemoryLOD="no" /> <!-- houses, wrecks -->
<rootclass name="SurvivorBase" act="character" reportMemoryLOD="no" /> <!-- player characters -->
<rootclass name="DZ_LightAI" act="character" reportMemoryLOD="no" /> <!-- infected, animals -->
<rootclass name="CarScript" act="car" reportMemoryLOD="no" /> <!-- cars (sedan, hatchback, transitBus, V3S, ...) -->

*/


private ref Editor m_EditorInstance;
Editor GetEditor() {
	return m_EditorInstance;
}


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
	
	string GetSaveFile()
		return m_EditorSaveFile;
	
	void SetSaveFile(string file)
		m_EditorSaveFile = file;
	
	
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
	
	// Editor Commands
	ref EditorNewCommand NewCommand = new EditorNewCommand();
	ref EditorOpenCommand OpenCommand = new EditorOpenCommand();
	ref EditorSaveCommand SaveCommand = new EditorSaveCommand();
	ref EditorSaveAsCommand SaveAsCommand = new EditorSaveAsCommand();
	ref EditorCloseCommand CloseCommand = new EditorCloseCommand();
	ref EditorExportCommand ExportCommand = new EditorExportCommand();
	ref EditorImportCommand ImportCommand = new EditorImportCommand();
	ref EditorExitCommand ExitCommand = new EditorExitCommand();
	
	ref EditorUndoCommand UndoCommand = new EditorUndoCommand();
	ref EditorRedoCommand RedoCommand = new EditorRedoCommand();
	ref EditorSelectAllCommand SelectAllCommand = new EditorSelectAllCommand();
	ref EditorDeleteCommand DeleteCommand = new EditorDeleteCommand();
	
	ref EditorCutCommand CutCommand = new EditorCutCommand();
	ref EditorCopyCommand CopyCommand = new EditorCopyCommand();
	ref EditorPasteCommand PasteCommand = new EditorPasteCommand();
	
	ref EditorMagnetCommand MagnetCommand = new EditorMagnetCommand();
	ref EditorGroundCommand GroundCommand = new EditorGroundCommand();
	ref EditorSnapCommand SnapCommand = new EditorSnapCommand();
	
	ref EditorEnvironmentControlCommand EnvironmentControlCommand = new EditorEnvironmentControlCommand();
	ref EditorPreferencesCommand PreferencesCommand = new EditorPreferencesCommand();
	ref EditorCameraControlsCommand CameraControlsCommand = new EditorCameraControlsCommand();
	ref EditorReloadHudCommand ReloadHudCommand = new EditorReloadHudCommand();
	ref EditorReloadBrushesCommand ReloadBrushesCommand = new EditorReloadBrushesCommand();
	ref EditorLootEditorCommand LootEditorCommand = new EditorLootEditorCommand();
	
	// I think this one should be generated on the EditorObject
	ref EditorObjectPropertiesCommand ObjectPropertiesCommand = new EditorObjectPropertiesCommand();

	private ref EditorSettings 					m_EditorSettings;
	private ref EditorHud						m_EditorHud;
	private EditorHudController 				m_EditorHudController;
	
	private ref EditorHologram 					m_ObjectInHand;
	private ref EditorBrush						m_EditorBrush;
	private ref EditorObjectDataSet			 	m_SessionCache;
	
	private bool 								m_Active;
	private string 								m_EditorSettingsFile = "$profile:Editor/settings.ini";
	private string								m_EditorSaveFile;
	// todo move to settings
	string										EditorBrushFile = "$profile:Editor/EditorBrushes.xml";
	
	
	void Editor(PlayerBase player) 
	{
		EditorLog.Trace("Editor");		
		m_EditorInstance = this;
		m_Player = player;
	
		m_SessionCache = new EditorObjectDataSet();
		
		// Init Settings
		m_EditorSettings = EditorSettings.Load(m_EditorSettingsFile);
		m_EditorSettings.Reload();
		
		// Init UI
		m_UIManager = GetGame().GetUIManager();
		
		// Init Hud
		m_EditorHud = new EditorHud();
		m_EditorHudController = m_EditorHud.GetEditorHudController();		
		
		m_Mission = GetGame().GetMission();
		
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
		}
		
		SetActive(true);
	}
	
	void ~Editor() 
	{
		EditorLog.Trace("~Editor");
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
		// Dont think its needed anymore
		//m_EditorHud.Update(timeslice);
		
		set<Object> obj = new set<Object>();
		int x, y;
		GetMousePos(x, y);
		
		if (m_EditorHud && m_EditorHud.EditorMapWidget.IsVisible()) {
			CurrentMousePosition = m_EditorHud.EditorMapWidget.ScreenToMap(Vector(x, y, 0));
		} else {
			if (m_EditorSettings.ObjectDragCollisions) {
				CurrentMousePosition = MousePosToRay(obj, null, m_EditorSettings.ViewDistance);
			} else {
				CurrentMousePosition = MousePosToRay(obj, null, m_EditorSettings.ViewDistance, 0, true);
			}
		}
		
		// Temporary extra raycast since CurrentMousePosition uses groundonly
		MousePosToRay(obj, null, m_EditorSettings.ViewDistance);
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
		
		
		EditorLog.CurrentLogLevel = LogLevel.WARNING;
		if (m_EditorCamera) {
			vector cam_pos = m_EditorCamera.GetPosition();
			
			m_EditorHudController.cam_x = cam_pos[0];
			m_EditorHudController.cam_y = cam_pos[2];
			m_EditorHudController.cam_z = cam_pos[1];
			/*
			m_EditorHudController.NotifyPropertyChanged("cam_x");
			m_EditorHudController.NotifyPropertyChanged("cam_y");
			m_EditorHudController.NotifyPropertyChanged("cam_z");*/
			
		}
		
		EditorObjectSet selected_objects = GetSelectedObjects();
		if (selected_objects.Count() > 0) {
			// Spams errors
			//m_EditorHud.GetController().SetInfoObjectPosition(selected_objects[0].GetPosition());
		}
		
		CutCommand.SetCanExecute(selected_objects.Count() > 0);
		CopyCommand.SetCanExecute(selected_objects.Count() > 0);
		PasteCommand.SetCanExecute(EditorClipboard.IsClipboardValid());
			
		// debug
		timeslice_count++;
		avg_timeslice = avg_timeslice + ((ftime - avg_timeslice) / timeslice_count);
		m_EditorHudController.DebugText1 = avg_timeslice.ToString();
		//m_EditorHudController.NotifyPropertyChanged("DebugText1");
		EditorLog.CurrentLogLevel = LogLevel.TRACE;		
	}
	
	
	bool OnMouseDown(int button)
	{
		EditorLog.Trace("Editor::OnMouseDown");
		
		Widget target = GetWidgetUnderCursor();
		if (!target) {
			SetFocus(null);
			if (EditorUIManager.CurrentMenu) {
				delete EditorUIManager.CurrentMenu;
			}
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
						m_EditorHudController.DelayedDragBoxCheck();
						
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
			
			case MouseState.RIGHT: {
				
				
				
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
		EditorCommand command = GetCommandFromHotkeys(key);
		if (command) {
			CommandArgs args = new CommandArgs();
			args.Context = m_EditorHud;
			command.Execute(this, args);
			return true;
		}
			
		switch (key) {
				
			case KeyCode.KC_ESCAPE: {
				
				if (IsLootEditActive()) {
					FinishEditLootSpawns();
					return true;
				} 
				
				else if (EditorUIManager.CurrentDialog) {	
					EditorUIManager.CurrentDialog.CloseDialog();
					return true;
					
				} 
				
				else if (EditorUIManager.CurrentMenu) {
					delete EditorUIManager.CurrentMenu;
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
		
		m_EditorCamera.LookEnabled = m_Active;
		m_EditorCamera.MoveEnabled = m_Active;
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
	
	void CreateInHand(ref EditorPlaceableListItem data)
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
			EditorEvents.StopPlacing(this);
		}	
	}
	
		
		
	private Object m_LootEditTarget;
	private bool m_LootEditMode;
	private vector m_PositionBeforeLootEditMode;
	void EditLootSpawns(string name)
	{
		EditorLog.Trace("Editor::EditLootSpawns %1", name);
		m_LootEditTarget = GetGame().CreateObjectEx(name, Vector(0, 10, 0), ECE_NONE);
		
		EditorCamera.CAMERA_SPEED = 10;
		m_PositionBeforeLootEditMode = m_EditorCamera.GetPosition();
		m_EditorCamera.SetPosition(Vector(10, 10, 10));
		m_EditorCamera.LookAt(vector.Zero);	
		
		ref EditorMapGroupProto proto_data = new EditorMapGroupProto(m_LootEditTarget); 
		EditorXMLManager.LoadMapGroupProto(proto_data);
		
		m_LootEditMode = true;
		m_EditorSettings.ObjectDragCollisions = true;
	}
	
	void FinishEditLootSpawns()
	{
		EditorLog.Trace("Editor::FinishEditLootSpawns");
		IEntity child = m_LootEditTarget.GetChildren();
		while (child != null) {
			GetGame().ObjectDelete(Object.Cast(child));
			child = child.GetSibling();
		}
		
		GetGame().ObjectDelete(m_LootEditTarget);
		EditorCamera.CAMERA_SPEED = 60;
		m_EditorCamera.SetPosition(m_PositionBeforeLootEditMode);

		m_LootEditMode = false;
		m_EditorSettings.ObjectDragCollisions = false;

	}
	
	bool IsLootEditActive() { 
		return m_LootEditMode; 
	}

	EditorHud ReloadHud() 
	{
		if (m_EditorHud)
			delete m_EditorHud;
		
		m_EditorHud = new EditorHud();
		m_EditorHudController = m_EditorHud.GetEditorHudController();
		return m_EditorHud;
	}
	
	
	EditorCommand GetCommandFromHotkeys(int key)
	{
		//EditorLog.Trace("Editor::GetCommandFromHotkeys");
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			
			// Ctrl + Shift keybinds
			if (KeyState(KeyCode.KC_LSHIFT)) {
				
				switch (key) {
					
					case KeyCode.KC_S: {
						return SaveAsCommand;
					}
					
					case KeyCode.KC_I: {
						return EnvironmentControlCommand;
					}
					
					case KeyCode.KC_T: {
						return CameraControlsCommand;
					}
					
					case KeyCode.KC_U: {
						return ReloadHudCommand;						
					}
				}
			}
			
			
			switch (key) {
				
				case KeyCode.KC_Z: {
					return UndoCommand;
				}
				
				case KeyCode.KC_Y: {
					return RedoCommand;
				}
				
				case KeyCode.KC_A: {
					return SelectAllCommand;
				}
				
				case KeyCode.KC_N: {
					return NewCommand;
				}
				
				case KeyCode.KC_S: {
					//if (m_EditorSaveFile == string.Empty)
					//	return SaveAsCommand;
					
					return SaveCommand;
				}
				
				case KeyCode.KC_O: {
					return OpenCommand;
				}
				
				case KeyCode.KC_E: {
					return ExportCommand;
				}
				
				case KeyCode.KC_I: {					
					return ImportCommand;
				}
				
				case KeyCode.KC_X: {
					return CutCommand;
				}

				case KeyCode.KC_C: {
					return CopyCommand;
				}
				
				case KeyCode.KC_V: {
					return PasteCommand;
				}
				
				case KeyCode.KC_W: {
					return CloseCommand;
				}
			}
		}

		return null;
	}

}

