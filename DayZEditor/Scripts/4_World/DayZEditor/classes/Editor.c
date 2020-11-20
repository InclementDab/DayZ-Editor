//Print(__LINE__);
//Print(__FILE__); // useful shit // tools.pak?????
//Print(__void);
//local string test;
//Print(FLT_MAX);
//Print(FLT_MIN);
//__NULL_FUNCT


///
///
/// SYNC JUNCTURES?!?!?!?
///
///

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
		
	// statics (updated in Update())
	static Object								ObjectUnderCursor;
	static vector 								CurrentMousePosition;
	
	// public properties
	ref EditorWorldObject 						ObjectInHand;
	ref EditorCommandManager 					CommandManager;
	ref EditorSettings 							Settings;
	
	// private Editor Members
	private ref EditorHud						m_EditorHud;
	private ref EditorBrush						m_EditorBrush;
	private ref EditorObjectDataMap			 	m_SessionCache;
	private EditorCamera 						m_EditorCamera;
	
	// Stack of Undo / Redo Actions
	private ref EditorActionStack 				m_ActionStack;
	
	// private references
	private EditorHudController 				m_EditorHudController;
	private EditorObjectManagerModule 			m_ObjectManager;	
	
	private bool 								m_Active;
	string 										EditorSettingsFile = "$profile:/Editor/Settings.json";
	string										EditorSaveFile;
	string										EditorDirectory = "$profile:/Editor/";
	
	// modes
	bool 										MagnetMode;
	bool 										GroundMode;
	bool 										SnappingMode;
	bool 										CollisionMode;

	string 										Version = "v" + GetVersionNumber();
	
	private void Editor(PlayerBase player) 
	{
		EditorLog.Trace("Editor");
		g_Editor = this;
		m_Player = player;

		// Initialize the profiles/editor directory;		
		MakeDirectory(EditorDirectory);
		
		// Init Settings
		//Settings 			= LoadSettings(EditorSettingsFile);
		Settings = new EditorSettings();
		
		// Object Manager
		m_ObjectManager 	= EditorObjectManagerModule.Cast(GetModuleManager().GetModule(EditorObjectManagerModule));
		
		// Command Manager
		CommandManager 		= new EditorCommandManager();
		
		// Needs to exist on clients for Undo / Redo syncing
		m_SessionCache 		= new EditorObjectDataMap();
		m_ActionStack 		= new EditorActionStack();
		
		// Init Hud
		m_EditorHud 		= new EditorHud();
		m_EditorHudController = m_EditorHud.GetTemplateController();		
		
		m_Mission = GetGame().GetMission();
		
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
		}
		
		SetActive(true);
		
		thread AutoSaveThread();
	}
	
	private void ~Editor() 
	{
		EditorLog.Trace("~Editor");
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED, true); 
		}
		
		SaveSettings(Settings, EditorSettingsFile);
		
		delete Settings;
		delete m_EditorHud;
		delete m_EditorBrush;
		delete m_SessionCache;
		delete ObjectInHand;
	}

	
	static Editor Create(PlayerBase player)
	{
		EditorLog.Trace("Editor::Create");
		g_Editor = new Editor(player);
		g_Editor.SetActive(false);
		g_Editor.SetActive(true);
		return g_Editor;
	}
	
	static void Destroy()
	{
		EditorLog.Trace("Editor::Destroy");
		delete g_Editor;
	}
		
	void Update(float timeslice)
	{		
		ProcessInput(GetGame().GetInput());
		
		ref set<Object> obj = new set<Object>();
		int x, y;
		GetMousePos(x, y);
		
		if (m_EditorHud && m_EditorHud.EditorMapWidget.IsVisible()) {
			CurrentMousePosition = m_EditorHud.EditorMapWidget.ScreenToMap(Vector(x, y, 0));
			CurrentMousePosition[1] = GetGame().SurfaceY(CurrentMousePosition[0], CurrentMousePosition[2]);
		} else {
			Object collision_ignore;
			if (ObjectInHand) {
				collision_ignore = ObjectInHand.GetWorldObject();
			}
			
			CurrentMousePosition = MousePosToRay(obj, collision_ignore, Settings.ViewDistance, 0, !CollisionMode);
		}
		
		if (Settings.DebugMode) {
			Debug.DestroyAllShapes();
			Debug.DrawSphere(CurrentMousePosition, 0.25, COLOR_GREEN_A);
		}
	
		if (!IsPlacing()) {
			Object target = GetObjectUnderCursor(Settings.ViewDistance);
			if (target) {
				if (target != ObjectUnderCursor) {
					if (ObjectUnderCursor) { 
						OnMouseExitObject(ObjectUnderCursor, x, y);
					}
					OnMouseEnterObject(target, x, y);
					ObjectUnderCursor = target;
				} 
				
			} else if (ObjectUnderCursor) {
				OnMouseExitObject(ObjectUnderCursor, x, y);
				ObjectUnderCursor = null;
			}
		}
		
		
		int log_lvl = EditorLog.CurrentLogLevel;
		EditorLog.CurrentLogLevel = LogLevel.WARNING;
		if (m_EditorCamera) {
			vector cam_pos = m_EditorCamera.GetPosition();
			
			m_EditorHudController.cam_x = cam_pos[0];
			m_EditorHudController.cam_y = cam_pos[1];
			m_EditorHudController.cam_z = cam_pos[2];
			
			m_EditorHudController.NotifyPropertyChanged("cam_x");
			m_EditorHudController.NotifyPropertyChanged("cam_y");
			m_EditorHudController.NotifyPropertyChanged("cam_z");
			
		}
		
		EditorObjectMap selected_objects = GetSelectedObjects();
		if (selected_objects.Count() > 0 && selected_objects[0]) {
			// Spams errors
			m_EditorHud.GetTemplateController().SetInfoObjectPosition(selected_objects[0].GetPosition());
		}
		
		CommandManager.CutCommand.SetCanExecute(selected_objects.Count() > 0);
		CommandManager.CopyCommand.SetCanExecute(selected_objects.Count() > 0);
		//PasteCommand.SetCanExecute(EditorClipboard.IsClipboardValid());

		CommandManager.SnapCommand.SetCanExecute(false); // not implemented
		
		// Shit code. Theres better ways to do this CanUndo and CanRedo are slow
		CommandManager.UndoCommand.SetCanExecute(CanUndo());
		CommandManager.RedoCommand.SetCanExecute(CanRedo());
				
		EditorLog.CurrentLogLevel = log_lvl;		
		
		// God Mode
		if (m_Player) {
			m_Player.SetHealth("", "Health", 100);
			m_Player.SetHealth("", "Shock", 100);
		}
	}
	
	
	void ProcessInput(Input input)
	{
		if (IsPlacing()) {
			vector hand_ori = ObjectInHand.GetWorldObject().GetOrientation();
			float factor = 9;
			if (KeyState(KeyCode.KC_LSHIFT)) {
				factor /= 5;
			}
			
			if (KeyState(KeyCode.KC_LCONTROL)) {
				factor *= 5;
			}
			
			if (input.LocalValue("UAZoomInOptics")) {				
				hand_ori[0] = hand_ori[0] - factor;
				ObjectInHand.GetWorldObject().SetOrientation(hand_ori);			
			}
			
			if (input.LocalValue("UAZoomOutOptics")) {
				hand_ori[0] = hand_ori[0] + factor;
				ObjectInHand.GetWorldObject().SetOrientation(hand_ori);			
			}
		}
	}
	
	bool OnDoubleClick(int button)
	{
		EditorLog.Trace("Editor::OnDoubleClick");
		Widget target = GetWidgetUnderCursor();
		switch (button) {
			
			case MouseState.LEFT: {
				
				if (m_LootEditMode && !target) {
					InsertLootPosition(CurrentMousePosition);
				}
				
				return true;
			}
			
		}
		
		return false;
	}
	
	bool OnMouseDown(int button)
	{
		EditorLog.Trace("Editor::OnMouseDown");
		
		Widget target = GetWidgetUnderCursor();
		if (!target) {
			SetFocus(null);
			if (EditorHud.CurrentMenu) {
				delete EditorHud.CurrentMenu;
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
				
				
				if (!GetBrush() && GetSelectedObjects().Count() == 0) {
					
					if (ObjectUnderCursor) {
						EditorObject editor_object = m_ObjectManager.GetEditorObject(ObjectUnderCursor);
						if (editor_object) {
							
							// Removed due to bug with inside selection being weird
							// Allows multiple objects to be dragged with selection
							/*if (editor_object.IsSelected()) {
								return true;
							}
							
							if (!KeyState(KeyCode.KC_LSHIFT)) {
								ClearSelection();
							}
							
							SelectObject(editor_object);*/
							return true;
						} 
					} 
					
					if (!target) {
						m_EditorHud.DelayedDragBoxCheck();
					}
				}

				break;
			}
			
			case MouseState.MIDDLE: {
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					if (GetWidgetUnderCursor())
						EditorLog.Info(GetWidgetUnderCursor().GetName());						
					
					if (ObjectUnderCursor) {
						int low, high;
						ObjectUnderCursor.GetNetworkID(low, high);
						EditorLog.Info(low.ToString());
						EditorLog.Info(high.ToString());
					}
				} 
				
				else if (KeyState(KeyCode.KC_LSHIFT)) {


					if (ObjectUnderCursor) {						
						/* attempt at getting proxies to work. Failed
						while (ObjectUnderCursor.GetParent()) {
							ObjectUnderCursor = Object.Cast(ObjectUnderCursor.GetParent());
						}*/
						ClearSelection();
						SelectObject(CreateObject(ObjectUnderCursor));
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
	
	// Return TRUE if handled.	
	bool OnKeyPress(int key)
	{
		// Dont process hotkeys if dialog is open
		if (m_EditorHud.CurrentDialog) return false;
		
		if (m_CurrentKeys.Find(key) != -1) return false;
		
		m_CurrentKeys.Insert(key);
		EditorCommand command = CommandManager.CommandShortcutMap[m_CurrentKeys.GetMask()];
		if (command) {
			EditorLog.Debug("Hotkeys Pressed for %1", command.ToString());
			CommandArgs args = new CommandArgs();
			args.Context = m_EditorHud;
			command.Execute(this, args);
			return true;
		}
			
		return false;
	}
		
	bool OnKeyRelease(int key)
	{
		m_CurrentKeys.Remove(m_CurrentKeys.Find(key));
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
			m_EditorCamera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + Vector(0, 5, 0), ECE_LOCAL));
			m_EditorHud.CameraMapMarker = new EditorCameraMapMarker(m_EditorCamera);
			m_EditorHud.GetTemplateController().InsertMapMarker(m_EditorHud.CameraMapMarker);
			
			// Registers character as EditorObject
			//CreateObject(m_Player);
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

		EditorObjectMap placed_objects = GetEditor().GetPlacedObjects();
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				editor_object.GetMarker().Show(m_Active);
			}
		}	
		
		if (!m_Active) {	
			GetGame().SelectPlayer(m_Player.GetIdentity(), m_Player);
		}
		
		m_Player.GetInputController().SetDisabled(m_Active);
			
		EditorHud.SetCurrentTooltip(null);
	}
	
	bool OnMouseEnterObject(IEntity target, int x, int y)
	{
		
	}
	
	bool OnMouseExitObject(IEntity target, int x, int y)
	{

	}	
	
	void CreateInHand(EditorPlaceableItem item)
	{
		EditorLog.Trace("Editor::CreateInHand");
		
		// Turn Brush off when you start to place
		if (m_EditorBrush != null)
			SetBrush(null);
		
		ClearSelection();
		ObjectInHand = new EditorHologram(item);
		
		EditorEvents.StartPlacing(this, item);		
	}
	
	EditorObject PlaceObject()
	{
		EditorLog.Trace("Editor::PlaceObject");
		if (!ObjectInHand) return null;	
		
		EditorHologram editor_hologram;
		if (!Class.CastTo(editor_hologram, ObjectInHand)) {
			return null;
		}
		
		EntityAI entity = editor_hologram.GetWorldObject();
		if (!entity) {
			return null;
		}
		
		EditorObjectData editor_object_data = EditorObjectData.Create(entity.GetType(), entity.GetPosition(), entity.GetOrientation());
		if (editor_object_data) {
			EditorObject editor_object = CreateObject(editor_object_data);
		}
		
		EditorEvents.ObjectPlaced(this, editor_object);
		
		if (!KeyState(KeyCode.KC_LSHIFT)) { 
			StopPlacing(); 
		}
		
		if (editor_object) {
			SelectObject(editor_object);
		}
		
		return editor_object;
	}
	
	void StopPlacing()
	{
		EditorLog.Trace("Editor::StopPlacing");
		delete ObjectInHand;
		EditorEvents.StopPlacing(this);
	}
	
		
	private Object m_LootEditTarget;
	private bool m_LootEditMode;
	private vector m_PositionBeforeLootEditMode;
	private ref EditorMapGroupProto m_EditorMapGroupProto;
	
	static float LootYOffset;
	
	void EditLootSpawns(EditorPlaceableItem placeable_item)
	{
		EditorLog.Trace("Editor::EditLootSpawns %1", placeable_item.Type);
		 
		m_LootEditTarget = GetGame().CreateObjectEx(placeable_item.Type, Vector(0, 0, 0), ECE_CREATEPHYSICS | ECE_SETUP | ECE_UPDATEPATHGRAPH);
		vector size = ObjectGetSize(m_LootEditTarget);
		LootYOffset = size[1] / 2;
		m_LootEditTarget.SetPosition(Vector(0, LootYOffset, 0));
		m_LootEditTarget.SetOrientation(Vector(90, 0, 0));
		
		GetCamera().Speed = 10;
		m_PositionBeforeLootEditMode = m_EditorCamera.GetPosition();
		m_EditorCamera.SetPosition(Vector(10, LootYOffset, 10));
		m_EditorCamera.LookAt(Vector(0, LootYOffset, 0));	
		
		if (!FileExist(Settings.EditorProtoFile)) {
			EditorLog.Info("EditorProtoFile not found! Copying...");
			CopyFile("DayZEditor/scripts/data/Defaults/MapGroupProto.xml", Settings.EditorProtoFile);
		}
		
		m_EditorMapGroupProto = new EditorMapGroupProto(m_LootEditTarget); 
		EditorXMLManager.LoadMapGroupProto(m_EditorMapGroupProto, Settings.EditorProtoFile);
		
		m_LootEditMode = true;
		CollisionMode = true;
		
		thread EditLootSpawnsDialog();
	}
	
	private void EditLootSpawnsDialog()
	{
		MessageBox.Show("Beta!", "Please know that Edit Loot spawns is just a demo and has NO WAY of saving / Exporting your changes (yet)\n\nDouble Click: Add new Loot Position\nEscape: Exit Loot Editor (Copies loot positions to clipboard)", MessageBoxButtons.OK);
	}
	
	// Kinda very jank i think
	void InsertLootPosition(vector position)
	{
		m_EditorMapGroupProto.InsertLootPoint(new EditorLootPoint(position, 1, 1, 32));
	}
	
	void FinishEditLootSpawns()
	{
		EditorLog.Trace("Editor::FinishEditLootSpawns");
		
		array<EditorObject> loot_spawns = m_EditorMapGroupProto.GetLootSpawns();
		Object building = m_EditorMapGroupProto.GetBuilding();
		string loot_position_data;
		
		loot_position_data += string.Format("<group name=\"%1\" lootmax=\"4\">\n", building.GetType());
		// this shits a mess
		loot_position_data += "	<usage name=\"Industrial\" />\n";
		loot_position_data += "	<usage name=\"Farm\" />\n";
		loot_position_data += "	<usage name=\"Military\" />\n";
		loot_position_data += "	<container name=\"lootFloor\" lootmax=\"4\">\n";
		
		foreach (EditorObject loot_spawn: loot_spawns) {
			vector loot_pos = loot_spawn.GetPosition();
			loot_pos[1] = loot_pos[1] - LootYOffset;
			loot_position_data += string.Format("		<point pos=\"%1\" range=\"0.5\" height=\"1.5\" /> \n", loot_pos.ToString(false));
		}
		
		loot_position_data += "	</container>\n";
		loot_position_data += "</group>\n";
		
		GetGame().CopyToClipboard(loot_position_data);
		
		delete m_EditorMapGroupProto;
		
		GetGame().ObjectDelete(m_LootEditTarget);
		GetCamera().Speed = 60;
		m_EditorCamera.SetPosition(m_PositionBeforeLootEditMode);

		m_LootEditMode = false;
		CollisionMode = false;
	}
	
	bool IsLootEditActive() { 
		return m_LootEditMode; 
	}

	EditorHud ReloadHud() 
	{
		EditorLog.Trace("Editor::ReloadHud");
		delete m_EditorHud;
		
		m_EditorHud = new EditorHud();
		m_EditorHudController = m_EditorHud.GetTemplateController();
		return m_EditorHud;
	}
	
	void Undo()
	{
		EditorLog.Trace("EditorObjectManager::Undo");
		foreach (EditorAction action: m_ActionStack) {
			if (!action.IsUndone()) {
				action.CallUndo();
				EditorLog.Debug("Undo complete");
				return;
			}
		}
	}
	
	void Redo()
	{
		EditorLog.Trace("EditorObjectManager::Redo");
		for (int i = m_ActionStack.Count() - 1; i >= 0; i--) {
			if (m_ActionStack[i] != null && m_ActionStack[i].IsUndone()) {
				m_ActionStack[i].CallRedo();
				EditorLog.Debug("Redo complete");
				return;
			}
		}
	}
	
	bool CanUndo() 
	{
		foreach (EditorAction action: m_ActionStack) {
			if (!action.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool CanRedo() 
	{
		for (int i = m_ActionStack.Count() - 1; i >= 0; i--) {
			if (m_ActionStack[i] && m_ActionStack[i].IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	void TeleportPlayerToCursor()
	{
		if (!m_Player) return;
		m_Player.SetPosition(CurrentMousePosition);
	}
	
	private void AutoSaveThread()
	{
		while (g_Editor) {
			if (!Settings) continue;
			
			Settings.AutoSaveTimer = Math.Clamp(Settings.AutoSaveTimer, 10, FLT_MAX);
			Sleep(Settings.AutoSaveTimer * 1000);
			if (EditorSaveFile != string.Empty) {
				CommandManager.SaveCommand.Execute(this, null);
			}
		}
	}
	
		

	bool IsActive()
		return m_Active;
	
	EditorHud GetEditorHud() 
		return m_EditorHud;
	
	EditorCamera GetCamera() 
		return m_EditorCamera;
	
	EditorObjectManagerModule GetObjectManager() {
		return m_ObjectManager;
	}
	
	EditorObjectMap GetSelectedObjects() {
		return m_ObjectManager.GetSelectedObjects(); 
	}
	
	EditorObjectMap GetPlacedObjects() {
		return m_ObjectManager.GetPlacedObjects(); 
	}

	EditorObjectDataMap GetSessionCache() {
		return m_SessionCache; 
	}
		
	EditorObject GetEditorObject(int id) {
		return m_ObjectManager.GetEditorObject(id); 
	}
	
	EditorObject GetEditorObject(notnull Object world_object) {
		return m_ObjectManager.GetEditorObject(world_object);
	}
	
	EditorObject GetPlacedObjectById(int id) {
		return m_ObjectManager.GetPlacedObjectById(id); 
	}

	void DeleteSessionData(int id) {
		m_SessionCache.Remove(id);	
	}
	
	EditorObjectData GetSessionDataById(int id) {
		return m_SessionCache.Get(id); 
	}

	void SetBrush(EditorBrush brush) {
		m_EditorBrush = brush; 
	}
	
	EditorBrush GetBrush() {
		return m_EditorBrush; 
	}
	
	bool IsPlacing() {
		return ObjectInHand != null; 
	}
	
	
	EditorObject CreateObject(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL, bool create_undo = true) 
	{
		EditorLog.Trace("Editor::CreateObject " + target);	
		return CreateObject(EditorObjectData.Create(target, flags), create_undo);
	}
	
	EditorObject CreateObject(notnull EditorObjectData editor_object_data, bool create_undo = true) 
	{

		EditorLog.Trace("Editor::CreateObject " + editor_object_data);
		EditorAction action = new EditorAction("Delete", "Create");
		
		// Cache Data (for undo / redo)
		m_SessionCache.Insert(editor_object_data.GetID(), editor_object_data);
		
		// Create Object
		EditorObject editor_object = m_ObjectManager.CreateObject(m_SessionCache[editor_object_data.GetID()]);
		if (!editor_object) return null;
		
		action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
		
		if (create_undo) {
			InsertAction(action);
		}

		
		return editor_object;
	}
	
	EditorObjectMap CreateObjects(notnull EditorObjectDataMap data_list, bool create_undo = true) 
	{
		EditorLog.Trace("Editor::CreateObject");
		
		EditorObjectMap object_set = new EditorObjectMap();
		EditorAction action = new EditorAction("Delete", "Create");
		
		foreach (int id, ref EditorObjectData editor_object_data: data_list) {
			
			// Cache Data (for undo / redo)
			m_SessionCache.Insert(editor_object_data.GetID(), editor_object_data);
			
			// Create Object
			EditorObject editor_object = m_ObjectManager.CreateObject(m_SessionCache[editor_object_data.GetID()]);
			if (!editor_object) continue;
			
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			
			object_set.Insert(editor_object.GetID(), editor_object);

		}
		
		if (create_undo) {
			InsertAction(action);
		}

		
		return object_set;
	}
	
	void DeleteObject(EditorObject editor_object, bool create_undo = true) 
	{
		EditorAction action = new EditorAction("Create", "Delete");
		if (!editor_object.Locked) {
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			m_ObjectManager.DeleteObject(editor_object);
		}
		
		if (create_undo) {
			InsertAction(action);
		}
	}
	
	
	void DeleteObjects(EditorObjectMap editor_object_map, bool create_undo = true)
	{
		EditorAction action = new EditorAction("Create", "Delete");

		foreach (int id, EditorObject editor_object: editor_object_map) {
			if (!editor_object.Locked) {
				action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
				action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
				m_ObjectManager.DeleteObject(editor_object);
			}
		}
		
		if (create_undo) {
			InsertAction(action);
		}
	}
	
	void DeleteMapObject(int id)
	{
		Object obj = m_ObjectManager.GetWorldObject(id);
		if (obj) {
			CF.ObjectManager.HideMapObject(obj);
		}
	}
	
	void SelectObject(EditorObject target) {
		m_ObjectManager.SelectObject(target);
	}
	
	
	void DeselectObject(EditorObject target) {
		m_ObjectManager.DeselectObject(target);
	}
	
	
	void ToggleSelection(EditorObject target) {
		m_ObjectManager.ToggleSelection(target);
	}
		
	void ClearSelection() {
		m_ObjectManager.ClearSelection();
	}
	
	EditorActionStack GetActionStack() {
		return m_ActionStack;
	}
	
	void Clear()
	{
		EditorSaveFile = string.Empty;	
		m_ActionStack.Clear();
		m_SessionCache.Clear();
		m_ObjectManager.Clear();
	}
		
	void InsertAction(EditorAction action) 
	{
		m_ActionStack.InsertAction(action);
		// this crashes smile :)
		//m_ActionStack.UpdateDebugReadout(GetEditor().GetEditorHud().GetEditorHudController().DebugActionStackListbox);
	}
	
	static string GetVersionNumber()
    {
        ref array<ref ModInfo> mods = new array<ref ModInfo>;
        GetDayZGame().GetModInfos(mods);
        for (int i = 0; i < mods.Count(); i++) {
            if (mods[i].GetName().IndexOf("DayZ") == 0 && mods[i].GetName().IndexOf("Editor") == 5) {
                return mods[i].GetVersion();
            }
        }

        return string.Empty;
    }
	
	static vector GetRandomTeleportPosition()
	{		
		float x, z, radius;
		
		x = 5467;
		z = 8660;
		radius = 500;
		
		vector position;
		position[0] = Math.RandomInt(x - radius, x + radius);
		position[2] = Math.RandomInt(z - radius, z + radius);
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 1;
		
		if (GetGame().SurfaceIsSea(position[0], position[2])) {
			// try again
			EditorLog.Debug("Landed in water, trying again");
			return GetRandomTeleportPosition(); 
		}
		
		
		array<Object> position_objects = {};
		array<CargoBase> position_cargos = {};
		GetGame().GetObjectsAtPosition(position, 2, position_objects, position_cargos);
		if (position_objects.Count() > 0) {
			// try again
			EditorLog.Debug("Landed in building, trying again");
			return GetRandomTeleportPosition();
		}
		
		return position;
	}
}

