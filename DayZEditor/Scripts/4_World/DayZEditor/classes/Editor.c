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
	EditorStatistics							Statistics;
	
	// private Editor Members
	private ref EditorHud						m_EditorHud;
	private ref EditorBrush						m_EditorBrush;
	private ref EditorObjectDataMap			 	m_SessionCache;
	private EditorCamera 						m_EditorCamera;
	
	// Stack of Undo / Redo Actions
	private ref EditorActionStack 				m_ActionStack;
	private ref ShortcutKeys 					m_CurrentKeys = new ShortcutKeys();
	
	// private references
	private EditorHudController 				m_EditorHudController;
	private EditorObjectManagerModule 			m_ObjectManager;	
	private EditorCameraTrackManagerModule		m_CameraTrackManager;
	
	private bool 								m_Active;
	// todo: change this to some EditorFile struct that manages this better
	// bouncing around strings is a PAIN... i think it also breaks directories... maybe not
	protected string							EditorSaveFile;
	string										EditorDirectory = "$profile:/Editor/";
	
	// modes
	bool 										MagnetMode;
	bool 										GroundMode;
	bool 										SnappingMode;
	bool 										CollisionMode;

	string 										BanReason = "null";
	static const string 						Version = "1.11." + GetBuildNumber() + "pre";
	
	protected ref TStringArray					m_RecentlyOpenedFiles = {};
	
	// Loot Editing
	private Object 								m_LootEditTarget;
	private bool 								m_LootEditMode;
	private vector 								m_PositionBeforeLootEditMode;
	private ref EditorMapGroupProto 			m_EditorMapGroupProto;
	static float 								LootYOffset;
	
	private void Editor(PlayerBase player) 
	{
		EditorLog.Trace("Editor");
		g_Game.ReportProgress("Loading Editor");

		g_Editor = this;
		m_Player = player;
		
		// Load ban data
		LoadBanData();
		
		// Player god mode
		m_Player.SetAllowDamage(false);

		// Initialize the profiles/editor directory;		
		MakeDirectory(EditorDirectory);
		
		// Init Settings
		Settings 			= new EditorSettings();
		Settings.Load();
		
		// Init Statistics
		Statistics			= EditorStatistics.GetInstance();
		
		// Object Manager
		g_Game.ReportProgress("Initializing Object Manager");
		EditorLog.Info("Initializing Object Manager");
		m_ObjectManager 	= EditorObjectManagerModule.Cast(GetModuleManager().GetModule(EditorObjectManagerModule));
		
		// Camera Track Manager
		g_Game.ReportProgress("Initializing Camera Track Manager");
		EditorLog.Info("Initializing Camera Track Manager");
		m_CameraTrackManager = EditorCameraTrackManagerModule.Cast(GetModuleManager().GetModule(EditorCameraTrackManagerModule));
		
		// Command Manager
		g_Game.ReportProgress("Initializing Command Manager");
		EditorLog.Info("Initializing Command Manager");
		CommandManager 		= new EditorCommandManager();
		CommandManager.Init();
		
		// Needs to exist on clients for Undo / Redo syncing
		m_SessionCache 		= new EditorObjectDataMap();
		m_ActionStack 		= new EditorActionStack();
		
		// Init Hud
		g_Game.ReportProgress("Initializing Hud");
		m_EditorHud 		= new EditorHud();
		EditorLog.Info("Initializing Hud");
		m_EditorHudController = m_EditorHud.GetTemplateController();		
		
		m_Mission = GetGame().GetMission();
		
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
		}
		
		GetGame().GetProfileStringList("EditorRecentFiles", m_RecentlyOpenedFiles);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateStatTime, 10000, true, 10);
		
		thread AutoSaveThread();
	}
	
	private void ~Editor() 
	{
		EditorLog.Trace("~Editor");
		if (!IsMissionOffline()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED, true); 
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(UpdateStatTime);
		
		Settings.Save();
		Statistics.Save();
		
		delete Settings;
		delete m_EditorHud;
		delete m_EditorBrush;
		delete m_SessionCache;
		delete ObjectInHand;
		delete m_RecentlyOpenedFiles;
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
		
	void Update(float timeslice)
	{		
		ProcessInput(GetGame().GetInput());
		
		set<Object> obj();
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
		
		// Just shutting the logger up for a minute
		int log_lvl = EditorLog.CurrentLogLevel;
		EditorLog.CurrentLogLevel = LogLevel.WARNING;
		
		if (ObjectUnderCursor) {
			m_EditorHudController.ObjectReadoutName = ObjectUnderCursor.GetType();
		} else {
			m_EditorHudController.ObjectReadoutName = string.Empty;
		}
		
		m_EditorHudController.NotifyPropertyChanged("ObjectReadoutName");
		
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
		
		CommandManager[EditorCutCommand].SetCanExecute(selected_objects.Count() > 0);
		CommandManager[EditorCopyCommand].SetCanExecute(selected_objects.Count() > 0);
		//PasteCommand.SetCanExecute(EditorClipboard.IsClipboardValid());

		CommandManager[EditorSnapCommand].SetCanExecute(false); // not implemented
		
		// Shit code. Theres better ways to do this CanUndo and CanRedo are slow
		CommandManager[EditorUndoCommand].SetCanExecute(CanUndo());
		CommandManager[EditorRedoCommand].SetCanExecute(CanRedo());
		
		CommandManager[EditorOpenRecentCommand].SetCanExecute(m_RecentlyOpenedFiles.Count() > 0);
		
		CommandManager[EditorCameraTrackRun].SetCanExecute(m_CameraTrackManager.GetCameraTracks().Count() > 0);
		
		EditorLog.CurrentLogLevel = log_lvl;
	}
	
	// Get Selected player in Editor
	PlayerBase GetPlayer()
	{
		return m_Player;
	}
	
	void SetPlayer(PlayerBase player)
	{
		// You can only control one player, this is how
		EditorObjectMap placed_objects = GetPlacedObjects();
		foreach (int id, EditorObject placed_object: placed_objects) {
			PlayerBase loop_player = PlayerBase.Cast(placed_object.GetWorldObject());
			if (loop_player && loop_player != player) {
				placed_object.Control = false;
			}
		}
		
		m_Player = player;
	}
	
	void ProcessInput(Input input)
	{
		if (ObjectInHand && ObjectInHand.GetWorldObject()) {
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
		EditorLog.Trace("Editor::OnMouseDown " + button);
		
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
					PlaceObject();
					return true;
				}
				
				if (!target || target == m_EditorHud.EditorMapWidget) {
					ClearSelection();
					GetCameraTrackManager().ClearSelection();
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
					if (GetWidgetUnderCursor()) {
						EditorLog.Info(GetWidgetUnderCursor().GetName());						
					}
				} 
				
				else if (KeyState(KeyCode.KC_LSHIFT)) {
					if (ObjectUnderCursor) {			
						ClearSelection();
						HideMapObject(ObjectUnderCursor);
					}
				} else {
					vector pos = Vector(CurrentMousePosition[0], GetGame().SurfaceY(CurrentMousePosition[0], CurrentMousePosition[2]), CurrentMousePosition[2]);
					vector current_pos = m_EditorCamera.GetPosition();
					float distance_to_ground = GetGame().SurfaceY(current_pos[0], current_pos[2]);
					pos[1] = pos[1] + distance_to_ground;
					m_EditorCamera.LerpCameraPosition(pos, 0.1);
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
		
	// Return TRUE if handled.	
	bool OnKeyPress(int key)
	{
		// Dont process hotkeys if dialog is open
												// HACK
		if (m_EditorHud.CurrentDialog && key != KeyCode.KC_ESCAPE) {
			return false;
		}
		
		Widget focus = GetFocus();
		if (focus && focus.IsInherited(EditBoxWidget)) {
			return false;
		}
		
		if (m_CurrentKeys.Find(key) != -1) {
			return false;
		}
		
		m_CurrentKeys.Insert(key);
		EditorCommand command = CommandManager.GetCommandFromShortcut(m_CurrentKeys.GetMask());
		if (!command) {
			return false;
		}
			
		EditorLog.Debug("Hotkeys Pressed for %1", command.ToString());
		CommandArgs args = new CommandArgs();
		args.Context = m_EditorHud;
		command.Execute(this, args);
		return true;
	}
		
	bool OnKeyRelease(int key)
	{
		m_CurrentKeys.Remove(m_CurrentKeys.Find(key));
		return false;
	}
	
	// Call to enable / disable editor
	void SetActive(bool active)
	{	
		EditorLog.Info("Set Active %1", active.ToString());
		string ban_reason;
		if (IsBannedClient(ban_reason) && active) {
			ShowBanDialog(ban_reason);
			return;
		}
		
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
		
		m_Player.DisableSimulation(m_Active);
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
		if (GetWidgetUnderCursor()) {
			return null;
		}
		
		if (!ObjectInHand) return null;	
		
		EditorHologram editor_hologram;
		if (!Class.CastTo(editor_hologram, ObjectInHand)) {
			return null;
		}
		
		Object entity = editor_hologram.GetWorldObject();
		if (!entity) {
			EditorLog.Warning("Invalid Entity from %1", editor_hologram.GetPlaceableItem().Type);
			return null;
		}
		
		EditorObjectData editor_object_data = EditorObjectData.Create(entity.GetType(), entity.GetPosition(), entity.GetOrientation(), entity.GetScale(), EditorObjectFlags.ALL);
		if (!editor_object_data) {
			EditorLog.Warning("Invalid Object data from %1", entity.GetType());
			return null;
		}
		
		EditorObject editor_object = CreateObject(editor_object_data);
		if (!editor_object) { 
			EditorLog.Warning("Invalid Editor Object from %1", entity.GetType());
			return null;
		}
		
		if (editor_object.GetWorldObject() && editor_object.GetWorldObject().IsInherited(ItemBase)) {
			EditorLog.Warning("%1 has persistence! If you place this it may cause duplications in your server!", editor_object.GetWorldObject().GetType());
		}
		
		Statistics.PlacedObjects++;
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
		
	void EditLootSpawns(EditorPlaceableItem placeable_item)
	{
		EditorLog.Trace("Editor::EditLootSpawns %1", placeable_item.Type);
		 
		EditorLog.Info("Launching Loot Editor...");
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
		GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("CollisionMode");
		
		thread EditLootSpawnsDialog();
	}
	
	private void EditLootSpawnsDialog()
	{
		MessageBox.Show("Attention!", "Double Click: Add new Loot Position\nEscape: Exit Loot Editor (Copies loot positions to clipboard)", MessageBoxButtons.OK);
	}

	private void EditLootSpawnFinishedDialog()
	{
		EditorLootEditorDialog loot_editor_dialog("Attention!");
		loot_editor_dialog.ShowDialog();
	}
	
	// Kinda very jank i think
	void InsertLootPosition(vector position)
	{
		m_EditorMapGroupProto.InsertLootPoint(new EditorLootPoint(position, 1, 1, 32));
	}
	
	void FinishEditLootSpawns()
	{
		EditorLog.Trace("Editor::FinishEditLootSpawns");
		
		EditorLog.Info("Closing Loot Editor");
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
		
		thread EditLootSpawnFinishedDialog();
	}
	
	bool IsLootEditActive() 
	{ 
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
			if (m_ActionStack[i] && m_ActionStack[i].IsUndone()) {
				m_ActionStack[i].CallRedo();
				EditorLog.Debug("Redo complete");
				return;
			}
		}
	}
	
	bool CanUndo() 
	{
		foreach (EditorAction action: m_ActionStack) {
			if (action && !action.IsUndone()) {
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
			
			Settings.AutoSaveTimer = Math.Clamp(Settings.AutoSaveTimer, 10, int.MAX);
			Sleep(Settings.AutoSaveTimer * 1000);
			if (EditorSaveFile != string.Empty) {
				CommandManager[EditorSaveCommand].Execute(this, null);
			}
		}
	}

	void SetSaveFile(string save_file)
	{
		EditorSaveFile = save_file;
		GetEditorHud().GetController().NotifyPropertyChanged("m_Editor.EditorSaveFile");
		
		if (m_RecentlyOpenedFiles.Find(EditorSaveFile) != -1) {
			m_RecentlyOpenedFiles.RemoveOrdered(m_RecentlyOpenedFiles.Find(EditorSaveFile));
		}
		
		m_RecentlyOpenedFiles.Insert(EditorSaveFile);
		if (m_RecentlyOpenedFiles.Count() > 3) {
			m_RecentlyOpenedFiles.RemoveOrdered(0);
		}
		
		GetGame().SetProfileStringList("EditorRecentFiles", m_RecentlyOpenedFiles);
		GetGame().SaveProfile();
	}
		
	string GetSaveFile()
	{
		return EditorSaveFile;
	}
	
	void SetBrush(EditorBrush brush) 
	{
		m_EditorBrush = brush; 
	}
	
	void DeleteSessionData(int id) 
	{
		m_SessionCache.Remove(id);	
	}
	
	bool IsPlacing() 
	{
		return ObjectInHand != null; 
	}
	
	EditorObject CreateObject(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL, bool create_undo = true) 
	{
		EditorLog.Trace("Editor::CreateObject " + target);	
		return CreateObject(EditorObjectData.Create(target, flags), create_undo);
	}
	
	EditorObject CreateObject(EditorObjectData editor_object_data, bool create_undo = true) 
	{
		EditorLog.Trace("Editor::CreateObject " + editor_object_data);
		
		// Cache Data (for undo / redo)
		if (!editor_object_data) return null;
		m_SessionCache.Insert(editor_object_data.GetID(), editor_object_data);
		
		// Create Object
		EditorObject editor_object = m_ObjectManager.CreateObject(m_SessionCache[editor_object_data.GetID()]);
		if (!editor_object) return null;
		
		EditorAction action = new EditorAction("Delete", "Create");
		action.InsertUndoParameter(new Param1<int>(editor_object.GetID()));
		action.InsertRedoParameter(new Param1<int>(editor_object.GetID()));
		
		if (create_undo) {
			InsertAction(action);
		}
		
		return editor_object;
	}
		
	EditorObjectMap CreateObjects(EditorObjectDataMap data_list, bool create_undo = true) 
	{
		EditorLog.Trace("Editor::CreateObject");
		
		EditorObjectMap object_set = new EditorObjectMap();
		EditorAction action = new EditorAction("Delete", "Create");
		
		foreach (int id, EditorObjectData editor_object_data: data_list) {
			
			// Cache Data (for undo / redo)
			if (!editor_object_data) continue;
			m_SessionCache.Insert(editor_object_data.GetID(), editor_object_data);
			
			// Create Object
			EditorObject editor_object = m_ObjectManager.CreateObject(m_SessionCache[editor_object_data.GetID()]);
			if (!editor_object) continue;
			
			action.InsertUndoParameter(new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(new Param1<int>(editor_object.GetID()));
			
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
		if (!editor_object.Locked && editor_object.Show) {
			action.InsertUndoParameter(new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(new Param1<int>(editor_object.GetID()));
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
			if (!editor_object.Locked && editor_object.Show) {
				action.InsertUndoParameter(new Param1<int>(editor_object.GetID()));
				action.InsertRedoParameter(new Param1<int>(editor_object.GetID()));
				m_ObjectManager.DeleteObject(editor_object);
			}
		}
		
		if (create_undo) {
			InsertAction(action);
		}
	}
	
	bool HideMapObject(int id, bool create_undo = true)
	{
		return HideMapObject(m_ObjectManager.GetWorldObject(id), create_undo);
	}
	
	bool HideMapObject(Object object, bool create_undo = true)
	{
		return HideMapObject(new EditorDeletedObject(object), create_undo);
	}
	
	bool HideMapObject(EditorDeletedObject map_object, bool create_undo = true)
	{		
		if (m_ObjectManager.IsObjectHidden(map_object)) { 
			return false;
		}
		
		EditorAction action = new EditorAction("Unhide", "Hide");
		// todo refactor
		action.InsertUndoParameter(new Param1<int>(map_object.GetID()));
		action.InsertRedoParameter(new Param1<int>(map_object.GetID()));
		
		Statistics.RemovedObjects++;
		
		m_ObjectManager.HideMapObject(map_object);

		if (create_undo) {
			InsertAction(action);
		}
		
		return true;
	}
	
	void HideMapObjects(EditorDeletedObjectMap deleted_objects, bool create_undo = true)
	{
		EditorAction action = new EditorAction("Unhide", "Hide");

		foreach (int id, EditorDeletedObject deleted_object: deleted_objects) {						
			action.InsertUndoParameter(new Param1<int>(deleted_object.GetID()));
			action.InsertRedoParameter(new Param1<int>(deleted_object.GetID()));
			
			Statistics.RemovedObjects++;
			m_ObjectManager.HideMapObject(deleted_object);
		}
		
		if (create_undo) {
			InsertAction(action);
		}
	}
	
	bool UnhideMapObject(int id, bool create_undo = true)
	{		
		if (!m_ObjectManager.IsObjectHidden(id)) { 
			return false;
		}
		
		EditorAction action = new EditorAction("Hide", "Unhide");
		// todo refactor
		action.InsertUndoParameter(new Param1<int>(id));
		action.InsertRedoParameter(new Param1<int>(id));
				
		m_ObjectManager.UnhideMapObject(id);

		if (create_undo) {
			InsertAction(action);
		}
		
		return true;
	}
	
	void UnhideMapObjects(EditorDeletedObjectMap deleted_objects, bool create_undo = true)
	{
		EditorAction action = new EditorAction("Hide", "Unhide");

		foreach (int id, EditorDeletedObject deleted_object: deleted_objects) {						
			action.InsertUndoParameter(new Param1<int>(deleted_object.GetID()));
			action.InsertRedoParameter(new Param1<int>(deleted_object.GetID()));
			
			Statistics.RemovedObjects++;
			m_ObjectManager.UnhideMapObject(deleted_object);
		}
		
		if (create_undo) {
			InsertAction(action);
		}
	}
	
	/*
	bool UnhideMapObject(EditorDeletedObject map_object, bool create_undo = true)
	{
		if (!map_object) { 
			return false;
		}
		
		if (!m_ObjectManager.IsObjectHidden(map_object)) { 
			return false;
		}
		
		EditorAction action = new EditorAction("Hide", "Unhide");
		// todo refactor
		action.InsertUndoParameter(new Param1<int>(map_object.GetID()));
		action.InsertRedoParameter(new Param1<int>(map_object.GetID()));
				
		m_ObjectManager.UnhideMapObject(map_object);

		if (create_undo) {
			InsertAction(action);
		}
		
		return true;
	}
	*/
	
	void LockObject(EditorObject editor_object)
	{
		EditorAction action = new EditorAction("Unlock", "Lock");
		action.InsertUndoParameter(new Param1<EditorObject>(editor_object));
		action.InsertRedoParameter(new Param1<EditorObject>(editor_object));
		
		editor_object.Lock(true);
		
		InsertAction(action);
	}
	
	void UnlockObject(EditorObject editor_object)
	{
		EditorAction action = new EditorAction("Lock", "Unlock");
		action.InsertUndoParameter(new Param1<EditorObject>(editor_object));
		action.InsertRedoParameter(new Param1<EditorObject>(editor_object));
		
		editor_object.Lock(false);
		
		InsertAction(action);
	}
	
	void SelectObject(EditorObject target) 
	{
		m_ObjectManager.SelectObject(target);
	}
	
	void DeselectObject(EditorObject target) 
	{
		m_ObjectManager.DeselectObject(target);
	}
	
	void ToggleSelection(EditorObject target) 
	{
		m_ObjectManager.ToggleSelection(target);
	}
		
	void ClearSelection() 
	{
		m_ObjectManager.ClearSelection();
	}
	
	void SelectHiddenObject(EditorDeletedObject target)
	{
		m_ObjectManager.SelectHiddenObject(target);
	}
	
	void DeselectHiddenObject(EditorDeletedObject target)
	{
		m_ObjectManager.DeselectHiddenObject(target);
	}
	
	void ToggleHiddenObjectSelection(EditorDeletedObject target)
	{
		m_ObjectManager.ToggleHiddenObjectSelection(target);
	}
	
	void Clear()
	{
		EditorSaveFile = string.Empty;	
		m_EditorHud.GetTemplateController().NotifyPropertyChanged("m_Editor.EditorSaveFile");
		m_ActionStack.Clear();
		m_SessionCache.Clear();
		m_ObjectManager.Clear();
	}
		
	void InsertAction(EditorAction action) 
	{
		m_ActionStack.InsertAction(action);
	}
	
	array<string> GetRecentFiles()
	{
		return m_RecentlyOpenedFiles;
	}
	
	static PlayerBase CreateDefaultCharacter(vector position = "0 0 0")
	{
		EditorLog.Trace("Editor::CreateDefaultCharacter");
		PlayerBase player;
		if (GetWorkbenchGame().GetPlayer()) {
			return PlayerBase.Cast(GetWorkbenchGame().GetPlayer());
		} 
		
		if (Class.CastTo(player, GetWorkbenchGame().CreatePlayer(null, GetWorkbenchGame().CreateRandomPlayer(), position, 0, "NONE"))) {
			player.GetInventory().CreateInInventory("AviatorGlasses");
	    	player.GetInventory().CreateInInventory("Shirt_RedCheck");
	    	player.GetInventory().CreateInInventory("Jeans_Blue");
	    	player.GetInventory().CreateInInventory("WorkingBoots_Brown");
	    	player.GetInventory().CreateInInventory("ConstructionHelmet_Yellow");
	    	player.GetInventory().CreateInInventory("CivilianBelt");
	    	player.GetInventory().CreateInInventory("TaloonBag_Blue");
			player.GetHumanInventory().CreateInHands("SledgeHammer");
		}
	
	    return player;
	}
			
	// Just annoying
	static string GetWorldName()
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		return world_name;
	}
		
	static vector GetMapCenterPosition()
	{
		TIntArray values();
		GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", GetWorldName()), values);
				
		// they were playing the wrong game when they thought of this one
		return Vector(values[0], values[2], values[1]);
	}
	
	static vector GetSafeStartPosition(float x, float z, float radius)
	{
		vector position;
		position[0] = Math.RandomFloat(x - radius, x + radius);
		position[2] = Math.RandomFloat(z - radius, z + radius);
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 1;
		
		//if (GetGame().SurfaceIsSea(position[0], position[2])) {
			// try again
			//EditorLog.Debug("Landed in water, trying again");
			//return GetSafeStartPosition(x, z, radius + 50); 
		//}
		
		array<Object> position_objects = {};
		array<CargoBase> position_cargos = {};
		GetGame().GetObjectsAtPosition(position, 2, position_objects, position_cargos);
		if (position_objects.Count() > 0) {
			// try again
			EditorLog.Debug("Landed in building, trying again");
			return GetSafeStartPosition(x, z, radius + 50);
		}
		
		return position;
	}
		
	bool IsBannedClient(out string reason)
	{
		reason = BanReason;
		return reason != "null";
	}
	
	// This is a bit of a dodgy / hacky solution
	void LoadBanData()
	{
		array<ref BiosUser> users = {};
		// Weird bug
		if (!GetGame() || !GetGame().GetUserManager()) return;
		
		GetGame().GetUserManager().GetUserList(users);
		foreach (BiosUser user: users) {
			Print(user.GetName());
			Print(user.GetUid());
		}
		
		RestContext rest = GetRestApi().GetRestContext("https:\/\/dayz-editor-default-rtdb.firebaseio.com\/");
		BanReason = rest.GET_now(string.Format("bans/%1.json", GetGame().GetUserManager().GetSelectedUser().GetUid()));
		
		// Temporary hotfix until we can re-implement the old system
		// dont really like it but it will have to do
		if (BanReason == "App Error") {
			BanReason = "null";
		}
		
		if (BanReason == "Timeout") {
			BanReason = "null";
		}
	}
	
	static void ShowBanDialog(string reason)
	{
		EditorLog.Warning("Banned Client Detected! Exiting...");
		if (reason == "App Error") {
			GetGame().GetUIManager().ShowDialog("Network Error", "Could not load network data, please check your internet connection and restart the DayZ Editor", 76, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu());
		} else {
			GetGame().GetUIManager().ShowDialog("Banned from DayZ Editor", string.Format("You have been banned from using the DayZ Editor.\n%1\n If you believe this was in error, please contact InclementDab \# 0001 on Discord", reason), 76, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu());
		}		
	}
	
	void UpdateStatTime(int passed_time)
	{
		Statistics.PlayTime += passed_time;
	}
	
	static int GetBuildNumber()
	{
		static const int BUILD_LENGTH = 1;
		if (!FileExist("DayZEditor\\Scripts\\Data\\build.txt")) {
			return 0;
		}
		
		FileHandle handle = OpenFile("DayZEditor\\Scripts\\Data\\build.txt", FileMode.READ);
		
		int values[1];
		string build_number;
		while (ReadFile(handle, values, 1)) {
			build_number += values[0].AsciiToString();
		}
		
		CloseFile(handle);
		
		return build_number.ToInt();
	}
	
	bool IsActive() return m_Active;
	EditorHud GetEditorHud() return m_EditorHud;
	EditorCamera GetCamera() return m_EditorCamera;
	EditorObjectManagerModule GetObjectManager() return m_ObjectManager;
	EditorCameraTrackManagerModule GetCameraTrackManager() return m_CameraTrackManager;
	EditorObjectMap GetSelectedObjects() return m_ObjectManager.GetSelectedObjects(); 
	EditorDeletedObjectMap GetSelectedHiddenObjects() return m_ObjectManager.GetSelectedHiddenObjects();
	EditorObjectMap GetPlacedObjects() return m_ObjectManager.GetPlacedObjects(); 
	EditorDeletedObjectMap GetDeletedObjects() return m_ObjectManager.GetDeletedObjects();
	EditorObjectDataMap GetSessionCache() return m_SessionCache; 		
	EditorObject GetEditorObject(int id) return m_ObjectManager.GetEditorObject(id); 	
	EditorObject GetEditorObject(notnull Object world_object) return m_ObjectManager.GetEditorObject(world_object);	
	EditorObject GetPlacedObjectById(int id) return m_ObjectManager.GetPlacedObjectById(id); 	
	EditorObjectData GetSessionDataById(int id) return m_SessionCache.Get(id); 
	EditorBrush GetBrush() return m_EditorBrush;
	EditorActionStack GetActionStack() return m_ActionStack;
}