/*
                    Lloyd sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/


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
Editor GetEditor() 
{
	return g_Editor;
}

class EditorHandData
{
	vector PositionOffset;
	vector OrientationOffset;
}

typedef map<Object, ref EditorHandData> EditorHandMap;

class Editor: Managed
{
	/* Private Members */
	protected Mission m_Mission;
		
	// statics (updated in Update())
	static Object								ObjectUnderCursor;
	static int 									ComponentUnderCursor;
	static vector 								CurrentMousePosition;
	
	static const ref array<string> DELETION_BLACKLIST = {
		"BrushBase",
		"BoundingBoxBase",
		"Man",
		"EditorCamera"
	};
	
	// public properties
	ref EditorCommandManager 					CommandManager;
	EditorGeneralSettings GeneralSettings = EditorGeneralSettings.Cast(GetDayZGame().GetProfileSetting(EditorGeneralSettings));
	EditorStatistics Statistics = EditorStatistics.Cast(GetDayZGame().GetProfileSetting(EditorStatistics));
	
	// protected Editor Members
	protected ref EditorHud	m_EditorHud;
	protected ref EditorBrush m_EditorBrush;

	protected EditorCamera m_EditorCamera;
	protected ref EditorHandMap m_PlacingObjects = new EditorHandMap();
	protected Entity m_CurrentControl;
	
	// Stack of Undo / Redo Actions
	protected ref EditorActionStack m_ActionStack = new EditorActionStack();
	
	// private references
	protected ref EditorCameraTrackManagerModule m_CameraTrackManager;
	
	protected int 									m_LastMouseDown;
	protected MouseState							m_LastMouseInput = -1;
	
	// todo: change this to some EditorFile struct that manages this better
	// bouncing around strings is a PAIN... i think it also breaks directories... maybe not
	protected string EditorSaveFile;
	static const string ROOT_DIRECTORY = "$saves:\\Editor\\";
	
	// modes
	bool 										MagnetMode;
	bool 										GroundMode;
	bool 										SnappingMode;
	bool 										CollisionMode;
	
	bool 										CameraLight;

	static const string 						Version = "1.30." + GetBuildNumber();
	
	protected ref TStringArray					m_RecentlyOpenedFiles = {};
	
	// Loot Editing
	protected Object 							m_LootEditTarget;
	protected bool 								m_LootEditMode;
	protected vector 							m_PositionBeforeLootEditMode;
	protected ref EditorMapGroupProto 			m_EditorMapGroupProto;
	static float 								LootYOffset;
	
	// Inventory Editor
	protected ref EditorInventoryEditorHud 		m_EditorInventoryEditorHud;
	
	protected ref Timer	m_StatisticsSaveTimer 	= new Timer(CALL_CATEGORY_GAMEPLAY);
	protected ref Timer	m_AutoSaveTimer			= new Timer(CALL_CATEGORY_GAMEPLAY);
	
	protected ScriptCaller 						m_ObjectSelectCallback;
				
	// Stored list of all Placed Objects, indexed by their WorldObject ID
	protected ref map<int, EditorObject> m_WorldObjectIndex = new map<int, EditorObject>();
	
	protected ref array<ref EditorHiddenObject> m_DeletedObjects = {};
		
	// Current Selected PlaceableListItem
	EditorPlaceableItem CurrentSelectedItem;
	
	bool KEgg; // oh?
	
	//0: EditorObject
	static ref ScriptInvoker OnObjectCreated = new ScriptInvoker();
	
	//0: EditorObject
	static ref ScriptInvoker OnObjectDeleted = new ScriptInvoker();
			
	// 0: EditorHandData
	static ref ScriptInvoker OnAddInHand = new ScriptInvoker();
	
	// 0: EditorHandData
	static ref ScriptInvoker OnRemoveFromHand = new ScriptInvoker();
	
	// 0: EditorObject
	static ref ScriptInvoker OnObjectPlaced = new ScriptInvoker();
	
	protected EditorOnlineSession m_CurrentOnlineSession;
	
	// Stored list of all Placed Objects
	ref map<string, ref EditorObject> m_PlacedObjects = new map<string, ref EditorObject>();
		
	void Editor() 
	{
		if (GetGame().IsServer()) {
			return;
		}
		
		EditorLog.Trace("Editor");
		g_Game.ReportProgress("Loading Editor");
		g_Editor = this;
				
		// Initialize the profiles/editor directory;		
		MakeDirectory(ROOT_DIRECTORY);
		
		// Init Statistics
		m_StatisticsSaveTimer.Run(10.0, this, "OnStatisticsSave", null, true);
								
		// Camera Init
		EditorLog.Info("Initializing Camera");
		g_Game.ReportProgress("Initializing Camera");
		m_EditorCamera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", GetGame().GetPlayer().GetPosition(), ECE_LOCAL));
		
		// Object Manager
		g_Game.ReportProgress("Initializing Object Manager");
		EditorLog.Info("Initializing Object Manager");
				
		// Camera Track Manager
		g_Game.ReportProgress("Initializing Camera Track Manager");
		EditorLog.Info("Initializing Camera Track Manager");
		m_CameraTrackManager = new EditorCameraTrackManagerModule();
		
		// Command Manager
		g_Game.ReportProgress("Initializing Command Manager");
		EditorLog.Info("Initializing Command Manager");
		CommandManager 		= new EditorCommandManager();
		CommandManager.Init();
		
		// Init Hud
		g_Game.ReportProgress("Initializing Hud");
		m_EditorHud 		= new EditorHud();
		
		EditorLog.Info("Initializing Hud");		
		// Add camera marker to newly created hud
		m_EditorHud.CameraMapMarker = new EditorCameraMapMarker(m_EditorCamera);
		m_EditorHud.GetTemplateController().InsertMapMarker(m_EditorHud.CameraMapMarker);
		
		m_Mission = GetGame().GetMission();
		
		GetGame().GetProfileStringList("EditorRecentFiles", m_RecentlyOpenedFiles);
		
		// Just some stuff to do on load
		GetGame().GetWorld().SetPreferredViewDistance(GeneralSettings.ViewDistance);		
		GetGame().GetWorld().SetViewDistance(GeneralSettings.ViewDistance);
		GetGame().GetWorld().SetObjectViewDistance(GeneralSettings.ObjectViewDistance);
		
		// this is terrible but it didnt work in OnMissionLoaded so im forced to reckon with my demons
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PPEffects.ResetAll, 1000);
		
		ControlCamera(m_EditorCamera);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(Update);
		
		// TODO!!! write a better autosave!!!
		//m_AutoSaveTimer.Run(GeneralSettings.AutoSaveTimer, this, "OnAutoSaveTimer");
	}

	void ~Editor() 
	{
		GeneralSettings.Save();
		Statistics.Save();

		GetGame().ObjectDelete(m_EditorCamera);
	}

	void OnStatisticsSave()
	{
		Statistics.Save();
	}
	
	void Update(float timeslice)
	{
		if (ShouldProcessInput()) {
			ProcessInput(GetGame().GetInput());
		}
	
		Statistics.Playtime += timeslice;
		
		set<Object> obj();
		int x, y;
		GetMousePos(x, y);		
				
		if (m_EditorHud && m_EditorHud.EditorMapWidget.IsVisible()) {
			CurrentMousePosition = m_EditorHud.EditorMapWidget.ScreenToMap(Vector(x, y, 0));
			CurrentMousePosition[1] = GetGame().SurfaceY(CurrentMousePosition[0], CurrentMousePosition[2]);
		} else {
			Object collision_ignore;
			// we need to determine what object is under the cursor so we can ignore it on our next raycast
			if (m_PlacingObjects.Count() > 0) {
				vector _;
				int __;
				vector collision_ray_start = GetGame().GetCurrentCameraPosition();
				vector collision_ray_end = collision_ray_start + GetGame().GetPointerDirection() * GeneralSettings.ViewDistance;
				set<Object> results = new set<Object>();
				if (DayZPhysics.RaycastRV(collision_ray_start, collision_ray_end, _, _, __, results)) {
					//collision_ignore = results[0];
					//Print(collision_ignore.GetType());
					
				}
				
				collision_ignore = m_PlacingObjects.GetKey(0);
			}
			
			// Yeah, enfusions dumb, i know
			bool should_precice = (GeneralSettings.HighPrecisionCollision || m_LootEditMode);																					// High precision is experimental, but we want it on for LootEditor since its only placing the cylinders!
			CurrentMousePosition = MousePosToRay(obj, collision_ignore, GeneralSettings.ViewDistance, 0, !CollisionMode, should_precice);
		}
		
		if (!IsPlacing()) {			
			vector hit_pos, hit_normal;
			int component_index;		
			set<Object> collisions = new set<Object>;
			DayZPhysics.RaycastRV(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * GeneralSettings.ObjectViewDistance, hit_pos, hit_normal, component_index, collisions);
			
			Object target = collisions[0];
			if (target) {
				if (target != ObjectUnderCursor || component_index != ComponentUnderCursor) {
					if (ObjectUnderCursor) { 
						OnMouseExitObject(ObjectUnderCursor, x, y, ComponentUnderCursor);
					}
					OnMouseEnterObject(target, x, y, component_index);
					ObjectUnderCursor = target;
					ComponentUnderCursor = component_index;
				} 
				
			} else if (ObjectUnderCursor) {
				OnMouseExitObject(ObjectUnderCursor, x, y, ComponentUnderCursor);
				ObjectUnderCursor = null;
				ComponentUnderCursor = 0;
			}
		}
		
		array<EditorObject> selected_objects = EditorObject.GetSelectedEditorObjects();
		if (m_EditorHud && selected_objects.Count() > 0 && selected_objects[0]) {
			// Spams errors
			m_EditorHud.GetTemplateController().SetInfoObjectPosition(selected_objects[0].GetWorldObject().GetPosition());
		}

		foreach (Object world_object, EditorHandData hand_data: m_PlacingObjects) {
			if (!world_object) {
				return;
			}
												
			vector position = CurrentMousePosition;
			if (hand_data) {
				position += hand_data.PositionOffset;
			}
			
			position[1] = position[1] + ObjectGetSize(world_object)[1];			
			vector transform[4] = {
				"1 0 0",
				"0 1 0",
				"0 0 1",
				position
			};
						
			vector surface_normal = vector.Up;
			float surface_height = GetGame().SurfaceY(position[0], position[2]);
			if (MagnetMode) {
				surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
			}
			
			vector local_ori = world_object.GetDirection();
			local_ori.Normalize();
			transform[0] = surface_normal * local_ori;
			transform[1] = surface_normal;
			transform[2] = surface_normal * (local_ori * vector.Up);
			
			world_object.SetPosition(position);
			world_object.Update();
		}
	}
				
	void PromptForObjectSelection(ScriptCaller callback)
	{
		m_ObjectSelectCallback = callback;
		
		if (m_ObjectSelectCallback) {
			m_EditorHud.CreateNotification("Select a world object");
		}
	}
	
	void SatisfyObjectSelectionPrompt(Object object)
	{
		m_ObjectSelectCallback.Invoke(object);
		if (object) {
			EditorBoundingBox.Destroy(object);
		}
	}
	
	bool IsPromptedForObjectSelection()
	{
		return m_ObjectSelectCallback != null;
	}
	
	// Leave null to use the default camera
	void ControlCamera(ScriptedCamera camera = null)
	{
		if (!camera) {
			camera = m_EditorCamera;
			if (!camera) {
				Error("Camera not found / initialized");
				return;
			}
		}
		
		if (m_CurrentControl) {			
			m_CurrentControl.DisableSimulation(true);
		}
		
		m_CurrentControl = camera;
		camera.SetActive(true);
		camera.OnSelectCamera();
		
		EditorObject.ClearSelections();
		m_CurrentControl.DisableSimulation(false);
	}
	
	void ControlPlayer(notnull Man player)
	{
		if (m_CurrentControl) {
			m_CurrentControl.DisableSimulation(true);
		}
		
		m_CurrentControl = player;
		GetGame().SelectPlayer(null, m_CurrentControl);
		
		EditorObject.ClearSelections();
		m_CurrentControl.DisableSimulation(false);
	}
	
	Entity GetCurrentControl()
	{
		return m_CurrentControl;
	}
	
	Man GetCurrentControlPlayer()
	{
		return Man.Cast(m_CurrentControl);
	}
	
	ScriptedCamera GetCurrentControlCamera()
	{
		return ScriptedCamera.Cast(m_CurrentControl);
	}
	
	void ProcessInput(Input input)
	{
		if (IsPlacing()) {
			foreach (Object placing_object, EditorHandData placing_hand_data: m_PlacingObjects) {
				vector hand_ori = placing_object.GetOrientation();
				float factor = 9;
				if (KeyState(KeyCode.KC_LSHIFT)) {
					factor /= 5;
				}
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					factor *= 5;
				}
				
				if (input.LocalValue("UAZoomInOptics")) {				
					hand_ori[0] = hand_ori[0] - factor;
					placing_object.SetOrientation(hand_ori);			
				}
				
				if (input.LocalValue("UAZoomOutOptics")) {
					hand_ori[0] = hand_ori[0] + factor;
					placing_object.SetOrientation(hand_ori);			
				}
			}
		} else {
			if (input.LocalPress("EditorTeleportPlayerToCursor")) {
				Man controlled_player = GetCurrentControlPlayer();
				if (controlled_player) {
					set<Object> _();
					controlled_player.SetPosition(MousePosToRay(_, controlled_player, 3000, 0, false, true));
				}
			}
			
			if (input.LocalPress("EditorToggleActive")) {
				if (!GetCurrentControlCamera()) {
					ControlCamera();
				} else {
					// if player doesnt exist yet, create one
					/*if (!Man.s_LastControlledPlayer) {
						vector player_position;
						if (!DoCursorRaycast(player_position, 100.0)) {
							player_position = GetCurrentControlCamera().GetPosition();
							player_position[1] = GetGame().SurfaceY(player_position[0], player_position[2]);
						}
						
						Man.s_LastControlledPlayer = CreateDefaultCharacter(GetGame().CreateDefaultPlayer(), player_position);
						if (!Man.s_LastControlledPlayer) {
							return;
						}
						
						Man.s_LastControlledPlayer.SetAllowDamage(false);
					}
					
					ControlPlayer(Man.s_LastControlledPlayer);*/
				}
			}
		}
	}
	
	bool ShouldProcessInput()
	{
		return (!GetDayZGame().IsLeftCtrlDown() && GetGame().GetInput().HasGameFocus(INPUT_DEVICE_KEYBOARD) && (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget));
	}
	
	bool DoCursorRaycast(out vector position, float max_distance = 3000, Object ignore_object = null)
	{
		vector raycast_direction;
		if (m_EditorHud.IsCursorVisible()) {
			raycast_direction = GetGame().GetPointerDirection();
		} else {
			raycast_direction = GetGame().GetCurrentCameraDirection();
		}
		
		vector begin_pos = GetGame().GetCurrentCameraPosition();
		vector end_pos = begin_pos + raycast_direction * max_distance;
		int interaction_layers = PhxInteractionLayers.BUILDING | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ITEM_LARGE;
		Object hit_object;
		vector normal;
		float fraction;
		return DayZPhysics.RayCastBullet(begin_pos, end_pos, interaction_layers, ignore_object, hit_object, position, normal, fraction);
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
		if (!target) { //target.GetName() != "HudPanel"
			SetFocus(null);
			if (EditorHud.CurrentMenu) {
				delete EditorHud.CurrentMenu;
			}
		}
		
		switch (button) {			
			case MouseState.MIDDLE: {
				
				// Ctrl + Middle Mouse logic
				if (KeyState(KeyCode.KC_LCONTROL)) {
					if (ObjectUnderCursor) {			
						EditorObject.ClearSelections();
						if (GetEditorObject(ObjectUnderCursor)) {
							DeleteObject(GetEditorObject(ObjectUnderCursor));
						} else {
							GetGame().ObjectDelete(ObjectUnderCursor);
							HideMapObject(ObjectUnderCursor);
						}
					}
					
					return true;
				} 
				
				return true;
			}
			
			case MouseState.RIGHT: {				
				break;	
			}
		}
		
		
		if (GetWorldTime() - m_LastMouseDown < 500) {
			m_LastMouseDown = 0;
			if (OnDoubleClick(button) && m_LastMouseInput == button) {
				m_LastMouseInput = -1;
				return true; // return is so we dont call GetWorldTime again
			}
		}
	
		m_LastMouseInput = button;
		m_LastMouseDown = GetWorldTime();
		return false;
	}
	
	bool IsSurfaceWater(vector position)
	{
		return GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]);
	}
			
	// also called when component index changes
	bool OnMouseEnterObject(Object target, int x, int y, int component_index)
	{
		if (IsPromptedForObjectSelection()) {
			EditorBoundingBox.Create(target);
			return true;
		}
		
		return m_EditorHud.OnMouseEnterObject(target, x, y, component_index);
	}
	
	// also called when component index changes
	bool OnMouseExitObject(Object target, int x, int y, int component_index)
	{		
		if (IsPromptedForObjectSelection()) {
			EditorBoundingBox.Destroy(target);
			return true;
		}
		
		return m_EditorHud.OnMouseExitObject(target, x, y, component_index);
	}	
	
	void RemoveFromHand(Object world_object)
	{
		m_PlacingObjects.Remove(world_object);
	}
	
	void ClearHand()
	{
		foreach (Object world_object, EditorHandData hand_data: m_PlacingObjects) {
			RemoveFromHand(world_object);
		}
	}
	
	array<Object> GetPlacingObjects()
	{
		return m_PlacingObjects.GetKeyArray();
	}
	
	EditorHandData GetObjectInHandData(Object world_object)
	{
		return m_PlacingObjects[world_object];
	}
	
	EditorHandMap AddInHand(Object world_object, EditorHandData hand_data = null)
	{
		// Turn Brush off when you start to place
		if (m_EditorBrush) {
			SetBrush(null);
		}
		
		if (hand_data) {
			m_PlacingObjects[world_object] = hand_data;
		} else {
			m_PlacingObjects[world_object] = new EditorHandData();
		}
		return m_PlacingObjects;
	}
	
	EditorHandMap AddInHand(EditorPlaceableItem item, EditorHandData hand_data = null)
	{
		return AddInHand(item.CreateObject(Editor.CurrentMousePosition, vector.Zero, 1.0), hand_data);				
	}
			
	void EditLootSpawns(EditorPlaceableItem placeable_item)
	{
		EditorLog.Info("Launching Loot Editor...");
		m_LootEditTarget = GetGame().CreateObjectEx(placeable_item.GetName(), Vector(0, 0, 0), ECE_CREATEPHYSICS | ECE_SETUP | ECE_UPDATEPATHGRAPH);
		vector size = ObjectGetSize(m_LootEditTarget);
		LootYOffset = size[1] / 2;
		m_LootEditTarget.SetPosition(Vector(0, LootYOffset, 0));
		m_LootEditTarget.SetOrientation(Vector(90, 0, 0));
		
		GetCamera().Speed = 10;
		m_PositionBeforeLootEditMode = m_EditorCamera.GetPosition();
		m_EditorCamera.SetPosition(Vector(10, LootYOffset, 10));
		m_EditorCamera.LookAt(Vector(0, LootYOffset, 0));	
		
		if (!FileExist(GeneralSettings.EditorProtoFile)) {
			EditorLog.Info("EditorProtoFile not found! Copying...");
			CopyFile("DayZEditor/scripts/data/Defaults/MapGroupProto.xml", GeneralSettings.EditorProtoFile);
		}
		
		m_EditorMapGroupProto = new EditorMapGroupProto(m_LootEditTarget); 
		EditorXMLManager.LoadMapGroupProto(m_EditorMapGroupProto, GeneralSettings.EditorProtoFile);
		
		m_LootEditMode = true;
		CollisionMode = true;
		GetEditorHud().GetTemplateController().NotifyPropertyChanged("CollisionMode");
		
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
	
	void StartInventoryEditor(EntityAI entity)
	{
		if (m_EditorInventoryEditorHud) {
			m_EditorInventoryEditorHud.Close();
		}
		
		m_EditorInventoryEditorHud = new EditorInventoryEditorHud(entity);
	}
	
	void StopInventoryEditor()
	{
		if (m_EditorInventoryEditorHud) {
			m_EditorInventoryEditorHud.Close();
		}
	}
	
	bool IsInventoryEditorActive()
	{
		return (m_EditorInventoryEditorHud != null);	
	}
	
	// Kinda very jank i think
	void InsertLootPosition(vector position)
	{
		m_EditorMapGroupProto.InsertLootPoint(new EditorLootPointData(position, 1, 1, 32));
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
			EditorLootPoint loot_point = EditorLootPoint.Cast(loot_spawn.GetWorldObject());
			if (!loot_point) {
				continue;
			}
			
			vector loot_pos = loot_spawn.GetWorldObject().GetPosition();
			loot_pos[1] = loot_pos[1] - LootYOffset;
			loot_position_data += string.Format("		<point pos=\"%1\" range=\"%2\" height=\"%3\" /> \n", loot_pos.ToString(false), loot_point.Range, loot_point.Height);
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
		m_EditorHud = new EditorHud();
		return m_EditorHud;
	}
	
	void Undo()
	{
		EditorLog.Trace("EditorObjectManager::Undo");
		foreach (EditorAction action: m_ActionStack) {
			if (!action.IsUndone()) {
				action.CallUndo();
				EditorLog.Info("Undo complete");
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
				EditorLog.Info("Redo complete");
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
		
	protected void OnAutoSaveTimer()
	{		
		if (EditorSaveFile != string.Empty && GeneralSettings.AutoSaveEnabled) {
			CommandManager[EditorSaveCommand].Execute(this, null);
		}
		
		//m_AutoSaveTimer.Run(Math.Max(GeneralSettings.AutoSaveTimer, 60), this, "OnAutoSaveTimer");
	}
	
	EditorObject CreateObject(notnull Object target, EditorObjectFlags flags = EFE_DEFAULT, bool create_undo = true) 
	{	
		EditorObject editor_object = new EditorObject(UUID.Generate(), target.GetType(), target.GetPosition(), target.GetOrientation(), flags);
		m_WorldObjectIndex[editor_object.GetWorldObject().GetID()] = editor_object;
		
		target.Delete();
	
		OnObjectCreated.Invoke(editor_object);
		Statistics.PlacedObjects++;
			
		m_PlacedObjects.Insert(editor_object.GetUUID(), editor_object);
			
		if (m_CurrentOnlineSession) {
			m_CurrentOnlineSession.SetSynchDirty();
		}
		
		return editor_object;
	}

	array<EditorObject> CreateObjects(array<Object> objects, EditorObjectFlags flags = EFE_DEFAULT, bool create_undo = true) 
	{
		array<EditorObject> editor_objects = {};
		foreach (Object object: objects) {			
			string type = object.GetType();
			vector transform[4];
			object.GetTransform(transform);
	
			EditorObject editor_object = new EditorObject(UUID.Generate(), object.GetType(), object.GetPosition(), object.GetOrientation(), flags);
			m_WorldObjectIndex[editor_object.GetWorldObject().GetID()] = editor_object;
			object.Delete();
		
			OnObjectCreated.Invoke(editor_object);
			Statistics.PlacedObjects++;
			
			m_PlacedObjects.Insert(editor_object.GetUUID(), editor_object);
			if (m_CurrentOnlineSession) {
				m_CurrentOnlineSession.SetSynchDirty();
			}
		
			editor_objects.Insert(editor_object);
		}
		
		return editor_objects;
	}
	
	void DeleteObject(notnull EditorObject editor_object, bool create_undo = true) 
	{			
		OnObjectDeleted.Invoke(editor_object);
		delete editor_object;
	}
	
	void DeleteObjects(notnull array<EditorObject> editor_objects, bool create_undo = true)
	{
		foreach (EditorObject editor_object: editor_objects) {		
			OnObjectDeleted.Invoke(editor_object);
		
			m_PlacedObjects.Remove(editor_object.GetUUID());
			delete editor_object;
		}
	}
	
	bool HideMapObject(notnull Object map_object, bool create_undo = true)
	{
		if (!CanHideMapObject(map_object.GetType())) {
			return false;
		}
		
		EditorHiddenObject hidden_object = new EditorHiddenObject(map_object);
		if (create_undo) {
			EditorAction action = new EditorAction("Unhide", "Hide");
			//action.InsertUndoParameter(new Param1<int>(map_object.GetID()));
			//action.InsertRedoParameter(new Param1<int>(map_object.GetID()));
			InsertAction(action);
		}
		
		Statistics.RemovedObjects++;
		m_DeletedObjects.Insert(hidden_object);
		return true;
	}
	
	void HideMapObjects(notnull array<Object> deleted_objects, bool create_undo = true)
	{
		EditorAction action = new EditorAction("Unhide", "Hide");
		
		foreach (Object object: deleted_objects) {			
			if (!CanHideMapObject(object.GetType())) {
				continue;
			}
			
			EditorHiddenObject hidden_object = new EditorHiddenObject(object);
			EditorHiddenObjectData data = hidden_object.CreateSerializedData();
			if (create_undo) {
				action.InsertUndoParameter(new Param1<ref EditorHiddenObjectData>(data));
				action.InsertRedoParameter(new Param1<ref EditorHiddenObjectData>(data));
			}
			
			Statistics.RemovedObjects++;
			m_DeletedObjects.Insert(hidden_object);
		}
		
		if (create_undo) {
			InsertAction(action);
		}
	}
		
	bool UnhideMapObject(notnull EditorHiddenObject hidden_object, bool create_undo = true)
	{				
		EditorAction action = new EditorAction("Hide", "Unhide");
		EditorHiddenObjectData data = hidden_object.CreateSerializedData();
		action.InsertUndoParameter(new Param1<ref EditorHiddenObjectData>(data));
		action.InsertRedoParameter(new Param1<ref EditorHiddenObjectData>(data));
				
		if (create_undo) {
			InsertAction(action);
		}
	
		delete hidden_object;
		return true;
	}
	
	void UnhideMapObjects(notnull array<EditorHiddenObject> hidden_objects, bool create_undo = true)
	{
		if (create_undo) {
			EditorAction action = new EditorAction("Hide", "Unhide");
		}
		
		foreach (EditorHiddenObject hidden_object: hidden_objects) {			
			EditorHiddenObjectData data = hidden_object.CreateSerializedData();			
			if (create_undo) {
				action.InsertUndoParameter(new Param1<ref EditorHiddenObjectData>(data));
				action.InsertRedoParameter(new Param1<ref EditorHiddenObjectData>(data));
			}
			
			Statistics.RemovedObjects++;
			
			delete hidden_object;
		}
		
		if (create_undo) {
			InsertAction(action);
		}
	}
	
	void Clear()
	{
		Statistics.Save();
		EditorSaveFile = string.Empty;	
		m_EditorHud.GetTemplateController().NotifyPropertyChanged("m_Editor.EditorSaveFile");
		m_ActionStack.Clear();
		
		m_PlacedObjects.Clear();	
		m_DeletedObjects.Clear();		
	}
		
	bool CanHideMapObject(string type)
	{
		foreach (string deletion_blacklist: DELETION_BLACKLIST) {
			if (deletion_blacklist == type) {
				return false;
			}
			
			if (GetGame().IsKindOf(type, deletion_blacklist)) {
				return false;
			}
		}
		
		return true;
	}
	
	void LockObject(EditorObject editor_object)
	{
		EditorAction action = new EditorAction("Unlock", "Lock");
		action.InsertUndoParameter(new Param1<EditorObject>(editor_object));
		action.InsertRedoParameter(new Param1<EditorObject>(editor_object));		
		InsertAction(action);
		
		editor_object.SetFlag(EditorObjectFlags.LOCKED);
		editor_object.SetSelected(false);
	}
	
	void UnlockObject(EditorObject editor_object)
	{
		EditorAction action = new EditorAction("Lock", "Unlock");
		action.InsertUndoParameter(new Param1<EditorObject>(editor_object));
		action.InsertRedoParameter(new Param1<EditorObject>(editor_object));		
		InsertAction(action);
		
		editor_object.ClearFlag(EditorObjectFlags.LOCKED);
		editor_object.SetSelected(false);
	}
	
	vector GetCameraProjectPosition(bool ground_only = true, float raycast_distance = 3000)
	{
		vector ray_start = GetGame().GetCurrentCameraPosition();
		vector ray_end = ray_start + GetGame().GetCurrentCameraDirection() * raycast_distance;
		
		vector pos, normal;
		int component;	
		DayZPhysics.RaycastRV(ray_start, ray_end, pos, normal, component, null, null, null, false, ground_only);
		return pos;
	}
		
	static Man CreateDefaultCharacter(string type, vector position)
	{
		EditorLog.Trace("Editor::CreateDefaultCharacter");
		if (GetGame().GetPlayer()) {
			return GetGame().GetPlayer();
		} 

		Man player = Man.Cast(GetGame().CreatePlayer(null, type, position, 0, string.Empty));
		if (!player) {
			EditorLog.Error("Failed to create new player, type %1", type);
			return null;
		}

		player.SetPosition(position);
		player.Update();
		if (GetCurrentHoliday() == EditorHoliday.CHRISTMAS) {
			player.GetInventory().CreateAttachment("SantasHat");
			player.GetInventory().CreateAttachment("SantasBeard");
		} else {
			player.GetInventory().CreateInInventory("ConstructionHelmet_Yellow");
		}
		
    	player.GetInventory().CreateInInventory("Shirt_RedCheck");
    	player.GetInventory().CreateInInventory("Jeans_Blue");
    	player.GetInventory().CreateInInventory("WorkingBoots_Brown");
    	player.GetInventory().CreateInInventory("CivilianBelt");
    	player.GetInventory().CreateInInventory("TaloonBag_Blue");
    	player.GetInventory().CreateInInventory("SledgeHammer");
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
	
	static vector GenerateSafeStartPosition(float radius = 2000.0)
	{
		vector position = GetMapCenterPosition();
		position[0] = Math.RandomFloat(position[0] - radius, position[0] + radius);
		position[2] = Math.RandomFloat(position[2] - radius, position[2] + radius);
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 25.0;		
		return position;
	}
		
	static int GetBuildNumber()
	{
		static const int BUILD_LENGTH = 1;
		if (!FileExist("DayZEditor\\Scripts\\Data\\build")) {
			Print("File doesnt exist");
			return 0;
		}
		
		FileHandle handle = OpenFile("DayZEditor\\Scripts\\Data\\build", FileMode.READ);
		
		int values[1];
		string build_number;
		while (ReadFile(handle, values, 1)) {
			build_number += values[0].AsciiToString();
		}
		
		CloseFile(handle);
		
		return build_number.ToInt();
	}
	
	static Object GetObjectUnderCursor(float raycast_distance = 3000)
	{
		vector ray_start = GetGame().GetCurrentCameraPosition();
		vector ray_end = ray_start + GetGame().GetPointerDirection() * raycast_distance;
		
		vector hitPos, hitNormal;
		int hitComponentIndex;		
		set<Object> collisions = new set<Object>;
		
	
		DayZPhysics.RaycastRV(ray_start, ray_end, hitPos, hitNormal, hitComponentIndex, collisions);
		
		return collisions.Get(0);
	}
	
	static EditorHoliday GetCurrentHoliday()
	{		
		int year, month, day;
		GetYearMonthDayUTC(year, month, day);
		if (month == 12 && day > 12) {
			return EditorHoliday.CHRISTMAS;
		}
		
		if (month == 1 && day < 14) {
			return EditorHoliday.NEWYEARS;
		}
		
		if (month == 10 && day > 0 && day < 14) {
			return EditorHoliday.ANNIVERSARY;
		}
		
		if (month == 10 && day > 14) {
			return EditorHoliday.HALLOWEEN;
		}
		
		return EditorHoliday.NONE;
	}
		
	void LoadSaveData(notnull EditorSaveData save_data, bool clear_before = false)
	{		
		int created_objects, deleted_objects;
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Warning("Different map detected");
			/*if (MessageBox.Show("Different Map Detected", string.Format("Switch map to %1?"), MessageBoxButtons.OKCancel) != DialogResult.OK) {
				return null;
			}
			
			EditorLog.Info("Loading Map %1", save_data.MapName);
			g_Game.ReportProgress(string.Format("Loading Map %1", save_data.MapName));
			
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
			
			m_Editor = GetEditor();
			*/
		}
		
		if (clear_before) {
			GetEditor().Clear();
		}
				
		EditorLog.Debug("Deleting %1 Objects", save_data.EditorHiddenObjects.Count().ToString());		
		foreach (EditorHiddenObjectData id: save_data.EditorHiddenObjects) {
			if (HideMapObject(id.FindObject(), false)) {
				deleted_objects++;
			}
		}
		
		EditorLog.Debug("Creating %1 Objects", save_data.EditorObjects.Count().ToString());
		foreach (EditorObjectData data: save_data.EditorObjects) {
			if (CreateObject(data.CreateObject(), false)) {
				created_objects++;
			}			
		}
		
		if (save_data.CameraPosition != vector.Zero) {
			GetCamera().SetPosition(save_data.CameraPosition);
		}
		
		string error_message;
		if (created_objects < save_data.EditorObjects.Count()) {
			error_message += string.Format("Failed to load %1 objects", save_data.EditorObjects.Count() - created_objects);
		}
		
		if (deleted_objects < save_data.EditorHiddenObjects.Count()) {
			if (error_message != string.Empty) {
				error_message += "	";
			}
			
			error_message += string.Format("Failed to delete %1 objects", save_data.EditorHiddenObjects.Count() - deleted_objects);
		}
		
		if (error_message != string.Empty) {
			EditorLog.Warning(error_message);
			m_EditorHud.CreateNotification(error_message, COLOR_YELLOW);
			
			// Disable auto save since we loaded a shit file
			//Settings.AutoSaveTimer = -1;
		} else {
			m_EditorHud.CreateNotification(string.Format("Loaded %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count()), COLOR_GREEN);
		}
	}
	
	EditorSaveData CreateSaveData(bool selected_only = false)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		// Save world name
		save_data.MapName = GetGame().GetWorldName();
		
		// Save Camera Position
		save_data.CameraPosition = GetCamera().GetPosition();
		
		// Save Objects
		array<EditorObject> placed_objects = GetPlacedObjects();
		if (selected_only) {
			placed_objects.Clear();
			array<EditorObject> selected_objects = EditorObject.GetSelectedEditorObjects();
			foreach (auto selected_object: selected_objects) {
				placed_objects.Insert(selected_object);
			}
		}
		
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				//save_data.EditorObjects.Insert(editor_object.CreateSerializedData());
			}
		}
		
		foreach (EditorHiddenObject hidden_object: m_DeletedObjects) {
			save_data.EditorHiddenObjects.Insert(hidden_object.CreateSerializedData());
		}
		
		return save_data;
	}
	
	EditorPlaceableItem GetReplaceableItem(Object object)
	{
		if (!object) {
			return null;
		}
		
		while (object.GetParent()) {
			object = Object.Cast(object.GetParent());
		}
		
		if (object.GetType() != string.Empty && !object.IsTree() && !object.IsBush() && !object.IsRock()) {			
			return GetDayZGame().GetPlaceableObject(object.GetType());
		}
		
		// 1346854: tank_small_white.p3d
		string debug_name = object.GetDebugNameNative();
		if (debug_name == string.Empty) {
			// lost cause, unlikely
			return null;
		}
		
		array<string> split_string = {};
		debug_name.Split(":", split_string);
		
		// also unlikely
		if (split_string.Count() == 1) {
			return null;
		}
		
		array<EditorPlaceableItem> placeable_items = GetDayZGame().GetReplaceableObjects(split_string[1].Trim());
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {			
			return null;
		}
		
		return placeable_items[0]; // better way to do other than index 0?
	}
		
	void SetSaveFile(string save_file)
	{
		EditorSaveFile = save_file;
		m_EditorHud.GetController().NotifyPropertyChanged("m_Editor.EditorSaveFile");
		
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
				
	void InsertAction(EditorAction action) 
	{
		m_ActionStack.InsertAction(action);
	}
	
	array<string> GetRecentFiles()
	{
		return m_RecentlyOpenedFiles;
	}
		
	EditorHud GetEditorHud() 
	{
		return m_EditorHud;
	}
	
	EditorInventoryEditorHud GetInventoryEditorHud() 
	{
		return m_EditorInventoryEditorHud;
	}
	
	EditorCamera GetCamera() 
	{
		return m_EditorCamera;
	}
		
	EditorCameraTrackManagerModule GetCameraTrackManager() 
	{
		return m_CameraTrackManager;
	}
				
	EditorBrush GetBrush() 
	{
		return m_EditorBrush;
	}
	
	EditorActionStack GetActionStack() 
	{
		return m_ActionStack;
	}	
	
	bool IsObjectHidden(notnull Object object)
	{
		return (CF.ObjectManager.IsMapObjectHidden(object));
	}
		
	array<EditorObject> GetPlacedObjects()
	{
		return m_PlacedObjects.GetValueArray(); 
	}
	
	array<ref EditorHiddenObject> GetDeletedObjects()
	{
		return m_DeletedObjects;
	}
				
	EditorObject GetEditorObject(notnull Object world_object) 
	{
		return m_WorldObjectIndex[world_object.GetID()];
	}
	
	void SetOnlineSession(EditorOnlineSession session)
	{
		m_CurrentOnlineSession = session;
	}

	EditorOnlineSession GetCurrentOnlineSession()
	{
		return m_CurrentOnlineSession;
	}
	
	bool GetHudVisiblity()
	{
#ifdef DIAG_DEVELOPER
		return m_EditorHud != null;
#else
		return m_EditorHud.IsVisible();
#endif	
	}
	
	void SetHudVisibility(bool state)
	{
#ifdef DIAG_DEVELOPER
		if (state) {
			m_EditorHud = new EditorHud();
		} else {
			delete m_EditorHud;
			return;
		}
#endif
		
		m_EditorHud.Show(state);
	}
	
	bool IsPlacing()
	{
		return (m_PlacingObjects && m_PlacingObjects.Count() > 0); 
	}
	
	static void RecursiveGetFiles(string directory, inout array<ref CF_File> files, string pattern = "*")
	{		
		array<ref CF_File> directories = {};
		// first get all directories and recurse them
		if (CF_Directory.GetFiles(directory + "*", directories, FindFileFlags.ARCHIVES)) {
			foreach (CF_File subdirectory: directories) {
				if (subdirectory.IsDirectory()) {
					RecursiveGetFiles(subdirectory.GetFullPath() + "/", files, pattern);
				}
			}
		}
		
		CF_Directory.GetFiles(directory + pattern, files, FindFileFlags.ARCHIVES);
	}
}