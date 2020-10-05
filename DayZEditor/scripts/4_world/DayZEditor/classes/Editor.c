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


// One day, someone important (likely Adam) will look over this codebase with a sny look of shame on their face
// if today is that day. fix it.
// and message me your feedback on discord :)


ref Editor g_Editor;
Editor GetEditor() {
	return g_Editor;
}


class Editor
{
	/* Private Members */
	private Mission m_Mission;
	private PlayerBase m_Player;
	
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
	
	EditorObjectManagerModule GetObjectManager() {
		return m_ObjectManager;
	}
	
	EditorObjectSet GetSelectedObjects() 
		return m_ObjectManager.GetSelectedObjects(); 
	
	EditorObjectSet GetPlacedObjects()
		return m_ObjectManager.GetPlacedObjects(); 

	EditorObjectDataSet GetSessionCache()
		return m_SessionCache; 
		
	EditorObject GetEditorObject(int id)
		return m_ObjectManager.GetEditorObject(id); 
	
	EditorObject GetEditorObject(notnull Object world_object)
		return m_ObjectManager.GetEditorObject(world_object);
	
	EditorObject GetPlacedObjectById(int id)
		return m_ObjectManager.GetPlacedObjectById(id); 

	void DeleteSessionData(int id)
		m_SessionCache.Remove(id);	
	
	EditorObjectData GetSessionDataById(int id)
		return m_SessionCache.Get(id); 

	void SetBrush(EditorBrush brush)
		m_EditorBrush = brush; 
	
	EditorBrush GetBrush()
		return m_EditorBrush; 
	
	bool IsPlacing()
		return ObjectInHand != null; 
	
	
	EditorObjectSet CreateObjects(EditorObjectDataSet data_list, bool create_undo = true)
		return m_ObjectManager.CreateObjects(data_list, create_undo);
	
	
	EditorObject CreateObject(EditorObjectData editor_object_data, bool create_undo = true)
		return m_ObjectManager.CreateObject(editor_object_data, create_undo);
	
	
	EditorObject CreateFromObject(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
		return m_ObjectManager.CreateFromObject(target, flags);
	
	
	void DeleteObject(EditorObject target, bool create_undo = true)
		m_ObjectManager.DeleteObject(target, create_undo);
	
	
	void DeleteObjects(EditorObjectSet target, bool create_undo = true)
		m_ObjectManager.DeleteObjects(target, create_undo);
	
	
	void SelectObject(EditorObject target)
		m_ObjectManager.SelectObject(target);
	
	
	void DeselectObject(EditorObject target)
		m_ObjectManager.DeselectObject(target);
	
	
	void ToggleSelection(EditorObject target)
		m_ObjectManager.ToggleSelection(target);
	
		
	void ClearSelection() 
		m_ObjectManager.ClearSelection();
	
	
	
	
	// statics (updated in Update())
	static Object								ObjectUnderCursor;
	static vector 								CurrentMousePosition;
	
	// public properties
	ref EditorHologram 							ObjectInHand;
	ref EditorCommandManager 					CommandManager;
	
	// private Editor Members
	private ref EditorSettings 					m_EditorSettings;
	private ref EditorHud						m_EditorHud;
	private ref EditorBrush						m_EditorBrush;
	private ref EditorObjectDataSet			 	m_SessionCache;
	private EditorCamera 						m_EditorCamera;
	
	// private references
	private EditorHudController 				m_EditorHudController;
	private EditorObjectManagerModule 			m_ObjectManager;	
	
	private bool 								m_Active;
	private string 								m_EditorSettingsFile = "$profile:Editor/settings.ini";
	private string								m_EditorSaveFile;
	// todo move to settings
	string										EditorBrushFile = "$profile:Editor/EditorBrushes.xml";
	
	// modes
	bool 										MagnetMode;
	bool 										GroundMode;
	bool 										SnappingMode;
	bool 										CollisionMode;
	
	private void Editor(PlayerBase player) 
	{
		EditorLog.Trace("Editor");		
		g_Editor = this;
		m_Player = player;
		
		// Object Manager
		m_ObjectManager = EditorObjectManagerModule.Cast(GetModuleManager().GetModule(EditorObjectManagerModule));
		
		// Command Manager
		CommandManager = new EditorCommandManager();
		
		// Idk delete me?
		m_SessionCache = new EditorObjectDataSet();
		
		// Init Settings
		m_EditorSettings = EditorSettings.Load(m_EditorSettingsFile);
		m_EditorSettings.Reload();
		
		// Init Hud
		m_EditorHud = new EditorHud();
		m_EditorHudController = m_EditorHud.GetTemplateController();		
		
		m_Mission = GetGame().GetMission();
		
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
		}
		
		SetActive(true);
	}
	
	private void ~Editor() 
	{
		EditorLog.Trace("~Editor");
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED, true);
		}
		
		delete m_EditorHud;
		delete m_EditorSettings;
		delete m_EditorBrush;
		delete m_SessionCache;
		delete ObjectInHand;
	}
	

	static Editor Create(PlayerBase player)
	{
		EditorLog.Trace("Editor::Create");
		g_Editor = new Editor(player);
		return g_Editor;
	}
	
	static void Destroy()
	{
		EditorLog.Trace("Editor::Destroy");
		delete g_Editor;
	}
	
	private bool exit_condition = false;
	
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
			EntityAI collision_ignore;
			if (ObjectInHand) {
				collision_ignore = ObjectInHand.GetProjectionEntity();
			}
			
			if (CollisionMode) {
				CurrentMousePosition = MousePosToRay(obj, collision_ignore, m_EditorSettings.ViewDistance);
			} else {
				CurrentMousePosition = MousePosToRay(obj, collision_ignore, m_EditorSettings.ViewDistance, 0, true);
			}
		}
		
		// Temporary extra raycast since CurrentMousePosition uses groundonly
		MousePosToRay(obj, null, m_EditorSettings.ViewDistance);
		if (!IsPlacing()) {
			Object target = obj.Get(0);
			if (target != null) {
				if (target != ObjectUnderCursor) {
					if (ObjectUnderCursor != null) { 
						OnMouseExitObject(ObjectUnderCursor, x, y);
					}
					OnMouseEnterObject(target, x, y);
					ObjectUnderCursor = target;
				} 
				
			} else if (ObjectUnderCursor != null) {
				exit_condition = OnMouseExitObject(ObjectUnderCursor, x, y);
				ObjectUnderCursor = null;
			}
		}
		
		
		int log_lvl = EditorLog.CurrentLogLevel;
		EditorLog.CurrentLogLevel = LogLevel.WARNING;
		if (m_EditorCamera) {
			vector cam_pos = m_EditorCamera.GetPosition();
			
			m_EditorHudController.cam_x = cam_pos[0];
			m_EditorHudController.cam_y = cam_pos[2];
			m_EditorHudController.cam_z = cam_pos[1];
			
			/*m_EditorHudController.NotifyPropertyChanged("cam_x");
			m_EditorHudController.NotifyPropertyChanged("cam_y");
			m_EditorHudController.NotifyPropertyChanged("cam_z");*/
			
		}
		
		EditorObjectSet selected_objects = GetSelectedObjects();
		if (selected_objects.Count() > 0 && selected_objects[0]) {
			// Spams errors
			m_EditorHud.GetTemplateController().SetInfoObjectPosition(selected_objects[0].GetPosition());
		}
		
		CommandManager.CutCommand.SetCanExecute(selected_objects.Count() > 0);
		CommandManager.CopyCommand.SetCanExecute(selected_objects.Count() > 0);
		//PasteCommand.SetCanExecute(EditorClipboard.IsClipboardValid());

		EditorLog.CurrentLogLevel = log_lvl;		
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
					CommandManager.PlaceObjectCommand.Execute(this, null);
					return true;
				}
				
				if (!target) {
					ClearSelection();
				}
				
				
				if (GetBrush() == null) {
					
					if (ObjectUnderCursor && m_ObjectManager.GetEditorObject(ObjectUnderCursor)) {
						if (!KeyState(KeyCode.KC_LSHIFT)) {
							ClearSelection();
						}
						
						SelectObject(m_ObjectManager.GetEditorObject(ObjectUnderCursor));
						return true;
					} 
					
					else if (!target) {
						m_EditorHud.DelayedDragBoxCheck();
						
					}
				}

				break;
			}
			
			case MouseState.MIDDLE: {
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					EditorLog.Info(GetWidgetUnderCursor().ToString());						
				} 
				
				else if (KeyState(KeyCode.KC_LSHIFT)) {

					if (ObjectUnderCursor) {						
						/* attempt at getting proxies to work. Failed
						while (ObjectUnderCursor.GetParent()) {
							ObjectUnderCursor = Object.Cast(ObjectUnderCursor.GetParent());
						}*/
						
						CreateFromObject(ObjectUnderCursor);
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
	
	private ref ShortcutKeys m_CurrentKeys = new ShortcutKeys();
	
	// Only use this to handle hardcoded keys (ctrl + z etc...)
	// Return TRUE if handled.	
	bool OnKeyPress(int key)
	{
		if (m_CurrentKeys.Find(key) == -1) {
			m_CurrentKeys.Insert(key);
			
			EditorCommand command = CommandManager.CommandShortcutMap[m_CurrentKeys.GetMask()];
			if (command) {
				EditorLog.Debug("Hotkeys Pressed for %1", command.ToString());
				CommandArgs args = new CommandArgs();
				args.Context = m_EditorHud;
				command.Execute(this, args);
				return true;
			}
		}

			
		switch (key) {
				
			case KeyCode.KC_ESCAPE: {
				
				if (GetSelectedObjects().Count() > 0) {
					ClearSelection();
					return true;
				}
				
				if (IsLootEditActive()) {
					FinishEditLootSpawns();
					return true;
				} 
				
				if (EditorUIManager.CurrentDialog) {	
					EditorUIManager.CurrentDialog.CloseDialog();
					return true;
					
				} 
				
				if (EditorUIManager.CurrentMenu) {
					delete EditorUIManager.CurrentMenu;
					return true;
				} 
				
				// jank
				if (!GetGame().GetMission().IsPaused()) {
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
		m_CurrentKeys.Clear();
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
			m_EditorCamera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", pos, ECE_LOCAL));
			
			
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
		
		
		// we are in 4_world and this game is bad :)
		Widget hud_root;
		EnScript.GetClassVar(GetGame().GetMission(), "m_HudRootWidget", 0, hud_root);
		if (hud_root) {
			hud_root.Show(!m_Active);
		}
	
		
		if (!m_Active) {	
			GetGame().SelectPlayer(m_Player.GetIdentity(), m_Player);
		}
		
		m_Player.GetInputController().SetDisabled(m_Active);
			
		
	}
	
	bool OnMouseEnterObject(IEntity target, int x, int y)
	{

	}
	
	bool OnMouseExitObject(IEntity target, int x, int y)
	{

	}	
	
	void CreateInHand(ref EditorPlaceableListItem data)
	{
		EditorLog.Trace("Editor::CreateInHand");
		
		// Turn Brush off when you start to place
		if (m_EditorBrush != null)
			SetBrush(null);
		
		ClearSelection();
		ObjectInHand = new EditorHologram(data.Type, CurrentMousePosition);
		
		EditorEvents.StartPlacing(this, data);		
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
		CollisionMode = true;
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
		CollisionMode = false;

	}
	
	bool IsLootEditActive() { 
		return m_LootEditMode; 
	}

	EditorHud ReloadHud() 
	{
		if (m_EditorHud)
			delete m_EditorHud;
		
		m_EditorHud = new EditorHud();
		m_EditorHudController = m_EditorHud.GetTemplateController();
		return m_EditorHud;
	}
}

