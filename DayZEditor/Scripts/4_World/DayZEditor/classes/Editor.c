/*
                    Euler sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/

#define GIZMOS_ENABLED


enum eEditorMode
{
	None = 0,
	Translation,
	Rotation,
	Scale
};

ref Editor g_Editor;
Editor GetEditor()
{
    if (!g_Editor || g_Editor.m_IsDestroying) {
        return null;
    }
    return g_Editor;
}

class EditorHandData
{
	vector PositionOffset;
	vector OrientationOffset;
}

typedef map<ref EditorWorldObject, ref EditorHandData> EditorHandMap;

class EditorColors
{
	static const LinearColor BLUE = 0xFF007ACC;
	static const LinearColor BLACK = 0xFF24282E;
	static const LinearColor YELLOW = 0xFFEFCB68;
	static const LinearColor HONEYDEW = 0xFFE1EFE6;
	static const LinearColor GRAY = 0xFFAEB7B3;
	static const LinearColor RED = 0xFFD64045;
	static const LinearColor PINK = 0xFF9E768F;
}

class EditorWebApi: WebApiBase
{
	override string GetBaseUrl()
	{
		return Editor.WEB_API_ENDPOINT;
	}
}

class Editor: Managed
{
	/* Private Members */
	protected Mission m_Mission;
	protected PlayerBase m_Player, m_ControllingPlayer;

	const int STATISTICS_SAVE_INTERVAL = 40;
	
//#ifdef DIAG_DEVELOPER
//	static const string WEB_API_ENDPOINT = "http:\/\/127.0.0.1:5000\/";
//#else
	static const string WEB_API_ENDPOINT = "http:\/\/45.61.162.80:8612\/";
//#endif
	
	static const ref array<string> DELETION_BLACKLIST = {
		"BrushBase",
		"BoundingBoxBase",
		"Man",
		"EditorCamera",
		"EditorCamera_V2",
		"EditorCameraClassic"
	};
		
    bool m_IsDestroying = false;
	// public properties
	ref EditorCommandManager 					CommandManager;
	
	// protected Editor Members
	protected ref EditorHud							m_EditorHud;
	ref EditorBrush	Brush;
	protected ref map<int, ref EditorObjectData>			m_SessionCache; // strong ref of EditorObjectData
	protected ref map<int, ref EditorDeletedObjectData>		m_DeletedSessionCache;
	protected ref map<string, EditorObject> m_EditorObjectsByUuid = new map<string, EditorObject>();
	protected ref map<string, EditorDeletedObject> m_HiddenObjectsByUuid = new map<string, EditorDeletedObject>();
	protected EditorCamera 												m_EditorCamera;
	protected ref EditorHandMap						m_PlacingObjects = new EditorHandMap();
	protected EditorWorldObject m_MainHandObject; // Always the first object added in hand
	protected typename m_CurrentGizmoType = EditorTranslationGizmo;
	protected ref EditorGizmo m_CurrentGizmo;
	protected ref EditorWebApi m_RestApi;
	
	// Stack of Undo / Redo Actions
	protected ref array<ref EditorAction> m_ActionStack = {};
	protected int m_CurrentActionIndex = -1;
	protected ref ShortcutKeys 						m_CurrentKeys = new ShortcutKeys();
	
	// private references
	protected EditorHudController 					m_EditorHudController;
	protected ref EditorObjectManagerModule 			m_ObjectManager;
	protected ref EditorNetActionManager				m_NetActionManager;	
	
	protected bool m_MouseVisibleOnClose;
	protected bool 									m_Active;
	// todo: change this to some EditorFile struct that manages this better
	// bouncing around strings is a PAIN... i think it also breaks directories... maybe not
	protected string								EditorSaveFile;
	protected float m_TimeSinceLastBackup;
	
	static const string	ROOT_DIRECTORY = SystemPath.Combine(SystemPath.Saves(), "Editor");
	static const string BRUSH_XML_FILE = "DayZEditor/Scripts/Data/Defaults/Brushes.xml";
	
	// modes
	bool 										MagnetMode;
	bool 										GroundMode;
	bool 										SnappingMode;
	bool 										CollisionMode;
	bool 										LightningMode;
	bool 										GridMode;
	bool 										CameraCollision;
	
	ref EditorEnvironment UserEnvironment;
	ref EditorDragHandler DragHandler;

	static const int Experimental = 0;
	static const int MinorVersionNumber = 3;
	static const int VersionNumber = 35;
	static const string Version = string.Format("1.%1%2%3", VersionNumber, Ternary<string>.If(MinorVersionNumber, "." + MinorVersionNumber.ToString(), string.Empty), Ternary<string>.If(Experimental, "E", string.Empty));
	static bool HasTestedVersion = false;
	
	protected ref TStringArray					m_RecentlyOpenedFiles = {};
	
	// Loot Editing
	protected Object 							m_LootEditTarget;
	protected bool 								m_LootEditMode;
	protected vector 							m_PositionBeforeLootEditMode;
	protected ref EditorMapGroupProto 			m_EditorMapGroupProto;
	static float 								LootYOffset;
	
	// Inventory Editor
	protected ref EditorInventoryEditorHud 		m_EditorInventoryEditorHud;
	
	// Cached ray and raycast infos
	protected ref Ray m_CameraRay;
	protected ref Ray m_CursorRay;
	protected ref Raycast m_CameraRaycast, m_CameraRaycastGround;
	protected ref Raycast m_CursorRaycast, m_CursorRaycastGround;
	
	protected Object m_ObjectUnderCursor;
	protected int m_ComponentIndexUnderCursor;

	protected eEditorMode m_EditorMode;
	protected int m_CameraTrackIndex = 0, m_CameraTrackState = 0;
	protected float m_CameraTrackLerpNorm = 0.0;
	protected vector m_CameraTransformPreTrackMotion[4];
	protected bool m_CameraMoveActive;
	
	protected ref EditorObject m_PlayerObject;
	
	// we are facing some issues with serializing files randomly breaking. this is a remedy for that poor code
	protected DateTime m_CurrentFileCreationDate = 0;
	protected ref array<string> m_CurrentFileAuthorCredits = {};
	
	bool										KEgg; // oh?
	
	void Editor(PlayerBase player) 
	{		
#ifdef DIAG_DEVELOPER
		EnProfiler.Enable(true, true, true);
#endif

		EditorLog.Trace("Editor");
		g_Game.ReportProgress("Loading Editor...");

		PrintFormat("Loading DayZ Editor v%1", Version);
		
		if (GetGame().IsMultiplayer()) {
			string address;
			int port;
			g_Game.GetHostAddress(address, port);
			array<int> valid_ips = { 
				-1707972227,
				1201824834,
                446933546,
				1863104197
			};
			
			if (valid_ips.Find(address.Hash()) == -1) {
				delete this;
				return;
			}
		}

		g_Editor = this;
		m_Player = player;
		m_ControllingPlayer = m_Player;
				
		// Player god mode
		if (m_Player) {
			m_Player.SetAllowDamage(false);
		}

		// Initialize the profiles/editor directory;		
		MakeDirectory(ROOT_DIRECTORY);
		
		// Load brush files
		string brush_file = SystemPath.Format(GetSettings().BrushFile);
		if (!FileExist(brush_file) && !CopyFile(BRUSH_XML_FILE, brush_file)) {
			Error(string.Format("Could not copy brush data to %1", brush_file));
		}
										
		// Camera Init
		EditorLog.Info("Initializing Camera");
		g_Game.ReportProgress("Loading Camera...");
		EditorCameraSettings camera_settings = EditorCameraSettings.Cast(g_Game.GetProfileSetting(EditorCameraSettings));
		string camera_type = "EditorCamera_V2";
		if (camera_settings.LegacyCamera) {
			camera_type = "EditorCameraClassic";
		}

#ifndef NO_GUI
		m_EditorCamera = EditorCamera.Cast(GetGame().CreateObjectEx(camera_type, m_Player.GetPosition() + Vector(0, 5, 0), ECE_LOCAL));
#endif
		
		// Object Manager
		// Loads placeable objects	
		g_Game.ReportProgress("Loading Placeable Objects");
		m_ObjectManager 	= new EditorObjectManagerModule(this);
		m_NetActionManager = new EditorNetActionManager();	
		
		// Command Manager
		g_Game.ReportProgress("Loading Editor Commands...");
		EditorLog.Info("Initializing Command Manager");
		CommandManager 		= new EditorCommandManager();
		CommandManager.Init();
		
		// Needs to exist on clients for Undo / Redo syncing
		m_SessionCache 			= new map<int, ref EditorObjectData>();
		m_DeletedSessionCache   = new map<int, ref EditorDeletedObjectData>();
		
		// Init Hud
		g_Game.ReportProgress("Loading Editor Hud...");
		m_EditorHud 		= new EditorHud(this);

		EditorLog.Info("Initializing Hud");
		m_EditorHudController = m_EditorHud.GetTemplateController();
		
		m_Mission = GetGame().GetMission();
		UserEnvironment = new EditorEnvironment();
		UserEnvironment.Capture(GetGame().GetWeather(), GetGame().GetWorld());
				
		GetGame().GetProfileStringList("EditorRecentFiles", m_RecentlyOpenedFiles);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnStatisticsSave, STATISTICS_SAVE_INTERVAL * 1000, true);
				
		// Register Player Object as a hidden EditorObject
		if (GetSettings().CreateCharacterObject && m_Player) {
			m_PlayerObject = CreateObject(m_Player, EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.MAPMARKER | EditorObjectFlags.NOSAVE | EditorObjectFlags.NODELETE, false);
			m_Player.SetPosition(m_Player.GetPosition());
		}
				
		// this is terrible but it didnt work in OnMissionLoaded so im forced to reckon with my demons
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PPEffects.ResetAll, 1000);
		
		g_Game.Event_OnActivateMessage.Insert(OnActivateMessage);
		g_Game.Event_OnDeactivateMessage.Insert(OnDeactivateMessage);
		
		m_RestApi = new EditorWebApi();
#ifndef DIAG_DEVELOPER
		if (GetSettings().VersionRequestedNotToSeeDonationDialog != VersionNumber) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ShowDonationDialog);
		}
#endif

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetGame().GetUIManager().ShowCursor, 0, false, true);
		GetSettings().TimesOpened++;
		
		// Enable default mode
		SetMode(eEditorMode.None);
		
		// Load default file
		if (g_Game.EditorFileToLoad != string.Empty) {			
			if (File.Exists(g_Game.EditorFileToLoad)) {
				EditorOpenCommand open_command = EditorOpenCommand.Cast(CommandManager[EditorOpenCommand]);
				LoadSaveData(open_command.ImportFile(g_Game.EditorFileToLoad), true);
				SetSaveFile(g_Game.EditorFileToLoad);
			}
			
			g_Game.EditorFileToLoad = string.Empty;
		}
		
		g_Game.ReportProgress("Loading Editor...");
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Activate);
	}
	
	void ~Editor() 
	{
		m_IsDestroying = true;

		//!Removed `g_Editor = null;` , Cleanup OnMissionFinish
		EditorLog.Trace("~Editor");
		
		// Fallback
		if (GetGame() && m_Mission) {
			// Causing more trouble than its worth, null ptrs
			// fix if you need to delete editor safely when running for some reason (MP?)
			//SetActive(false);
		}
				
		GetSettings().Save();
		GetStatistics().Save();
		
		delete m_EditorHud;
		delete m_EditorInventoryEditorHud;
		delete Brush;
		delete m_SessionCache;
		delete m_DeletedSessionCache;
		delete m_PlacingObjects;
		delete m_RecentlyOpenedFiles;
		
		GetGame().ObjectDelete(m_EditorCamera);
	}
		
	void SetMode(eEditorMode editor_mode)
	{
		m_EditorMode = Math.Rollover(editor_mode, 0, 4);
#ifdef GIZMOS_ENABLED
		m_EditorHud.SetEditorMode(m_EditorMode);
		
		// please be of type EditorTranslationGizmo
		switch (m_EditorMode) {
			case eEditorMode.Translation: {
				m_CurrentGizmoType = EditorTranslationGizmo;
				break;
			}

			case eEditorMode.Rotation: {
				m_CurrentGizmoType = EditorRotationGizmo;
				break;
			}

			case eEditorMode.Scale: {
				m_CurrentGizmoType = EditorScaleGizmo;
				break;
			}
			
			case eEditorMode.None: {
				m_CurrentGizmoType = EMPTY_TYPENAME;
				break;
			}
		}
#endif
	}
		
	eEditorMode GetMode()
	{
		return m_EditorMode;
	}
		
	// When you tab in the game
	void OnActivateMessage()
	{
		if (m_EditorHud && EditorHud.CurrentDialog) {
			GetGame().GetUIManager().ShowCursor(true);
		} else {
			GetGame().GetUIManager().ShowCursor(m_MouseVisibleOnClose);
		}
	}

	// When you tab out of the game (thanks jacob mongo)
	void OnDeactivateMessage()
	{
		m_MouseVisibleOnClose = GetGame().GetUIManager().IsCursorVisible() && IsActive();
	}
	
		// Returns a ray, on surface, pointing in the direction of the surface normal
	Ray GetMapRay(float y_offset = 0.0)
	{
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);

		vector map_position_screen = m_EditorHud.Map.ScreenToMap(Vector(mouse_x, mouse_y, 0));
		map_position_screen[1] = GetGame().SurfaceY(map_position_screen[0], map_position_screen[2]) + y_offset;
		vector map_direction_screen = GetGame().SurfaceGetNormal(map_position_screen[0], map_position_screen[2]);
		return new Ray(map_position_screen, map_direction_screen);
	}

	Ray GetCameraRay()
	{
		return m_CameraRay;
	}
	
	Ray GetCursorRay()
	{
		return m_CursorRay;
	}

	Ray GetCursorRayModeSafe()
	{
		if (IsMapActive()) {
			return GetMapRay();
		}

		return GetCursorRay();
	}

	Raycast GetCameraRaycast(Object ignore = null, bool ground_only = false)
	{
		/*if (!ignore && !ground_only) {
			return m_CameraRaycast;
		}

		if (!ignore) {
			return m_CameraRaycastGround;
		}*/
		
		return PerformRaycast(GetCameraRay(), ignore, m_EditorCamera.GetSettings().ViewDistance / 2, ground_only);
	}
	
	Raycast GetCursorRaycast(Object ignore = null, bool ground_only = false)
	{
		if (!ignore && !ground_only) {
			return m_CursorRaycast;
		}

		//if (!ignore) {
		//	return m_CursorRaycastGround;
		//}

		return PerformRaycast(GetCursorRay(), ignore, m_EditorCamera.GetSettings().ViewDistance / 2, ground_only);
	}

	// Returns a top-down raycast, height defined by y_offset_raycast
	Raycast GetMapCursorRaycast(Object ignore = null, bool ground_only = false, float y_offset_raycast = 100.0)
	{
		Ray map_ray = GetMapRay(y_offset_raycast);
		map_ray.Direction = -vector.Up;
		return PerformRaycast(map_ray, ignore, m_EditorCamera.GetSettings().ViewDistance / 2, ground_only);
	}

	Raycast GetCursorRaycastModeSafe(Object ignore = null, bool ground_only = false)
	{
		if (IsMapActive()) {
			return GetMapCursorRaycast(ignore, ground_only);
		}

		return GetCursorRaycast(ignore, ground_only);
	}
	
	Raycast GetCursorRaycastModeSafeEx(array<Object> ignores = null, bool ground_only = false)
	{
		if (IsMapActive()) {
			Ray map_ray = GetMapRay(0.0);
			map_ray.Direction = -vector.Up;
			return PerformRaycastEx(map_ray, ignores, m_EditorCamera.GetSettings().ViewDistance / 2, ground_only);
		}
		
		return PerformRaycastEx(GetCursorRay(), ignores, m_EditorCamera.GetSettings().ViewDistance / 2, ground_only);
	}

	void GetCameraTransform(out vector transform[4])
	{
		m_EditorCamera.GetTransform(transform);
	}

	void GetCursorTransform(out vector transform[4])
	{
		vector forward = GetGame().GetPointerDirection();
		vector aside = (vector.Up * forward).Normalized();
		vector up = (forward * aside).Normalized();
		transform = {
			aside,
			up,
			forward,
			m_EditorCamera.GetPosition()
		};
	}

	protected Raycast PerformRaycast(notnull Ray source_ray, Object ignore, float distance, bool ground_only)
	{
		int interaction_layers = PhxInteractionLayers.CAMERA;
		if (ground_only) {
			interaction_layers = PhxInteractionLayers.TERRAIN;
		}
		
		Raycast camera_raycast = source_ray.PerformRaycast(ignore, distance, interaction_layers);

		//if (!camera_raycast) {
		//	camera_raycast = source_ray.PerformRaycastRV(ignore, null, 0, distance, ObjIntersectFire, ground_only);
		//}

		if (!camera_raycast) {
			camera_raycast = source_ray.PerformRaycastRVEX(0, distance, ObjIntersectGeom, { ignore }, ground_only);
		}
		
		return camera_raycast;
	}
	
	protected Raycast PerformRaycastEx(notnull Ray source_ray, array<Object> ignores, float distance, bool ground_only)
	{
		int interaction_layers = -1;
		if (ground_only) {
			interaction_layers = PhxInteractionLayers.TERRAIN;
		}
		
		return source_ray.PerformRaycastMulti(ignores, distance, interaction_layers);
	}
	
	bool IsMapActive()
	{
		return m_EditorHud.Map.IsVisible();
	}

	protected ECameraLockFlag m_CameraLockFlags;
	void SetCameraLockFlag(ECameraLockFlag flag)
	{
		m_CameraLockFlags |= flag;
	}

	void ClearCameraLockFlag(ECameraLockFlag flag)
	{
		m_CameraLockFlags &= ~flag;
	}

	// Player controlled from viewport, probably poorly named
	protected bool m_ControlledPlayer;
	void SetPlayerControlled(bool state)
	{
		if (!m_ControllingPlayer) {
			m_ControllingPlayer = m_Player;
		}
		
		if (!m_ControllingPlayer) {
			m_ControlledPlayer = false;
			return;
		}
		
		m_ControlledPlayer = state;
		
		m_ControllingPlayer.DisableSimulation(!m_ControlledPlayer);
		m_ControllingPlayer.GetInputController().SetDisabled(!m_ControlledPlayer);
		
		GetGame().GetUIManager().ShowCursor(true);
	}

	bool IsPlayerControlled()
	{
		return m_ControlledPlayer;
	}

	// Brings you into player view, controlling them
	void ControlPlayer(notnull PlayerBase new_player)
	{
		m_ControllingPlayer = new_player;
		m_Active = false;
				
		delete m_EditorInventoryEditorHud;
		
		ClearSelection();
		
		m_ControllingPlayer.DisableSimulation(false);
		
		m_EditorCamera.SetActive(false);
		GetGame().SelectPlayer(null, m_ControllingPlayer);
		
		m_ControllingPlayer.GetInputController().SetDisabled(false);
		
		m_EditorHud.Show(false);
		GetGame().GetUIManager().ShowCursor(false);
		SetMissionHud(true);
		PPEffects.ResetAll();
		
		GetStatistics().CharactersControlled++;
		GetStatistics().Save();
		
		if (GetGame().IsMultiplayer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(m_Active);
			rpc.Write(vector.Zero); // unused
			rpc.Send(null, 39261, true);
		}
	}
	
	PlayerBase GetControllingPlayer()
	{
		return m_ControllingPlayer;
	}

	EditorNetActionManager GetNetActionManager()
	{
		return m_NetActionManager;
	}
	
	void Activate()
	{
		m_Active = true;
				
		// Shut down Inventory Editor, done prior to the camera due to the destructor
		if (m_EditorInventoryEditorHud) {
			delete m_EditorInventoryEditorHud;
		}
				
		if (m_EditorCamera) {
			m_EditorCamera.SetActive(true);
		}
			
		if (m_EditorHud) {
			m_EditorHud.Show(true);
			m_EditorHud.SetCurrentTooltip(null);
		}
				
		EditorObjectMap placed_objects = GetPlacedObjects();
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				if (!editor_object) {
					continue;
				}
				
				if (editor_object.GetMarker()) {
					editor_object.GetMarker().Show(true);			
				}
				
				editor_object.HideBoundingBox();
			}
		}
				
		GetGame().GetUIManager().ShowCursor(true);
		
		if (m_ControllingPlayer && !IsPlayerControlled()) {
			m_ControllingPlayer.GetInputController().SetDisabled(true);
			
			if (!_bugfixFirstGrab) {
				m_ControllingPlayer.DisableSimulation(false);
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_Player.DisableSimulation, 1000, 0, true);
				_bugfixFirstGrab = true;
			} else {
				m_ControllingPlayer.DisableSimulation(true);
			}
		}
		
		SetMissionHud(false);
		PPEffects.ResetAll();
	}

	ECameraLockFlag GetCameraLockFlags(bool use_override = false)
	{
		ECameraLockFlag processed_flags;
		if (GetGame().GetUIManager().IsCursorVisible()) {
			if (m_CameraMoveActive) {
				if (m_EditorCamera.GetSettings().InvertCamera) {
					processed_flags |= ECameraLockFlag.INVERT_LOOK;
				}
			} else {
				processed_flags |= ECameraLockFlag.LOCK_LOOK;
			}
		} else {
			if (GetUApi().GetInputByID(UATempRaiseWeapon).LocalValue()) {
				processed_flags |= ECameraLockFlag.PAN_LOOK;
			}
		}
		
		if (m_EditorHud && GetEditorHud().GetDialog()) {
			processed_flags |= (ECameraLockFlag.LOCK_LOOK | ECameraLockFlag.LOCK_MOVE);
		}

		if (GetFocus() && GetFocus().IsInherited(EditBoxWidget)) {
			processed_flags |= ECameraLockFlag.LOCK;
		}

		if (g_Game.GetMission().IsPaused()) {
			processed_flags |= ECameraLockFlag.LOCK;
		}
		
		if (EditorHud.CurrentDialog || EditorHud.CurrentMenu) {
			processed_flags |= ECameraLockFlag.LOCK;
		}

		if (IsCtrlDown()) {
			processed_flags |= ECameraLockFlag.LOCK_MOVE;
		}
		
		if (IsRunningCameraTrack()) {
			processed_flags |= ECameraLockFlag.LOCK_MOVE | ECameraLockFlag.LOCK_LOOK;
		}
		
		if (IsMapActive()) {
			processed_flags |= ECameraLockFlag.LOCK_LOOK;
		}
		
		if (m_ControlledPlayer) {
			processed_flags |= (ECameraLockFlag.LOCK_LOOK | ECameraLockFlag.LOCK_MOVE);
		}

		if (!use_override) {
			processed_flags |= m_CameraLockFlags;
		}
		
		return processed_flags;
	}
		
	private void OnStatisticsSave()
	{
		GetStatistics().EditorPlayTime += STATISTICS_SAVE_INTERVAL;
		GetStatistics().Save();
	}
	
	protected void CreateLightning(vector position)
	{
		position[1] = GetGame().SurfaceY(position[0], position[2]);
		
		SEffectManager.PlaySound("ThunderHeavy_Near_SoundSet", position);
		SEffectManager.PlaySound("ThunderHeavy_Far_SoundSet", position);

		Object lightning1 = GetGame().CreateStaticObjectUsingP3D("DZ/data/data/blesk1.p3d", position, Vector(Math.RandomFloat(0, 360), 0, 0), 1.5, true);
		Object lightning2 = GetGame().CreateStaticObjectUsingP3D("DZ/data/data/blesk2.p3d", position, Vector(Math.RandomFloat(0, 360), 0, 0), 1.5, true);

		ScriptedLightBase light = ScriptedLightBase.CreateLight(PointLightBase, position);
		light.SetBrightnessTo(200);
		light.SetRadiusTo(4000);
		light.SetLifetime(Math.RandomInt(150, 400) / 1000);
		light.SetPulseCoef(200);
		light.SetFlickerAmplitude(0.3);
		light.SetFlickerSpeed(40);
		light.SetFlareVisible(false);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(lightning1.Delete, Math.RandomInt(150, 300));
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(lightning2.Delete, Math.RandomInt(250, 600));
	}
	
	void Update(float timeslice)
	{				
		if (!GetGame().IsAppActive()) {
			return;
		}
		
		if (UserEnvironment) {
			UserEnvironment.Apply(GetGame().GetWeather(), GetGame().GetWorld());
		}
		
		float raycast_distance = GetCameraSettings().ViewDistance / 5;
		
		// The most common rays and raycast for the tool to use are updated and cached at the beginning of each frame. If you need a different raycast, then you will perform it yourself
		m_CursorRay = new Ray(GetGame().GetCurrentCameraPosition(), g_Game.GetPointerDirection());
		m_CameraRay = new Ray(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraDirection());
		
		if (GetGame().GetUIManager().IsCursorVisible()) {
			m_CursorRaycast = PerformRaycast(m_CursorRay, null, raycast_distance, false);
			//m_CursorRaycastGround = PerformRaycast(m_CursorRay, null, raycast_distance, true);		
		} else {
			m_CameraRaycast = PerformRaycast(m_CameraRay, null, raycast_distance, false);
			//m_CameraRaycastGround = PerformRaycast(m_CameraRay, null, raycast_distance, true);
		}
		
#ifdef GIZMOS_ENABLED
		if (m_CurrentGizmoType == EMPTY_TYPENAME) {
			delete m_CurrentGizmo;
		} else {
			if (!m_CurrentGizmoType.IsInherited(EditorGizmo)) {
				ErrorEx("Incorrect gizmo type, must inherit from EditorGizmo");
			}

			EditorObjectMap selected = GetSelectedObjects();
			
			if (!selected) {
				return; // Object manager not ready yet
			}
			
			if (selected.Count() > 0) {
				if (!m_CurrentGizmo || !m_CurrentGizmo.IsInherited(m_CurrentGizmoType)) {
					m_CurrentGizmo = EditorGizmo.Cast(m_CurrentGizmoType.Spawn());
				}
			} else {
				delete m_CurrentGizmo;
			}
			
			if (m_CurrentGizmo) {
				m_CurrentGizmo.Update(timeslice);
			}
		}
#endif
		
		// Process input after gizmo update because gizmos will need to block input during an interaction
		ProcessInput(timeslice, GetGame().GetInput());

		if (IsPlayerActive()) {
			bool cursor_active = GetGame().GetUIManager().IsCursorVisible();
			//m_Player.GetInputController().SetDisabled(false);
			
			if (GetGame().IsMultiplayer()) {
				//m_Player.DisableSimulation(cursor_active);
			}
		}

		if (EditorSaveFile != string.Empty) {
			m_TimeSinceLastBackup += timeslice;

			switch (GetSettings().CreateSaveBackups) {
				case 0: break;
				case 1: {
					if (m_TimeSinceLastBackup >= (5.0 * 60.0)) {
						CommandManager[EditorSaveBackupCommand].Execute(this, CommandArgs());
						m_TimeSinceLastBackup = 0;
					}

					break;
				}

				case 2: {
					if (m_TimeSinceLastBackup >= (15.0 * 60.0)) {
						CommandManager[EditorSaveBackupCommand].Execute(this, CommandArgs());
						m_TimeSinceLastBackup = 0;
					}

					break;
				}

				case 3: {
					if (m_TimeSinceLastBackup >= (60.0 * 60.0)) {
						CommandManager[EditorSaveBackupCommand].Execute(this, CommandArgs());
						m_TimeSinceLastBackup = 0;
					}

					break;
				}
			}
		}

		bool useful_widget_under_cursor = GetWidgetUnderCursor() && GetWidgetUnderCursor().GetName() != "HudPanel" && GetWidgetUnderCursor().GetName() != "CursorIcons";				
		if (!IsPlacing() && !useful_widget_under_cursor) {
			int x, y;
			GetMousePos(x, y);
			if (m_CursorRaycast && m_CursorRaycast.Hit) {
				if (m_CursorRaycast.Hit != m_ObjectUnderCursor || m_CursorRaycast.HitComponent != m_ComponentIndexUnderCursor) {
					if (m_ObjectUnderCursor) { 
						OnMouseExitObject(m_ObjectUnderCursor, x, y, m_ComponentIndexUnderCursor);
					}

					OnMouseEnterObject(m_CursorRaycast.Hit, x, y, m_CursorRaycast.HitComponent);
					m_ObjectUnderCursor = m_CursorRaycast.Hit;
					m_ComponentIndexUnderCursor = m_CursorRaycast.HitComponent;
				} 
				
			} else if (m_ObjectUnderCursor) {
				OnMouseExitObject(m_ObjectUnderCursor, x, y, m_ComponentIndexUnderCursor);
				m_ObjectUnderCursor = null;
				m_ComponentIndexUnderCursor = 0;
			} 
			
			if (m_CursorRaycast && !m_CursorRaycast.Hit && m_CursorRaycast.Bounce) {
				string surface_type;
				GetGame().SurfaceGetType(m_CursorRaycast.Bounce.Position[0], m_CursorRaycast.Bounce.Position[2], surface_type);
				surface_type = string.Format("Surface Type: %1", surface_type);
				m_EditorHud.ObjectHoverSelectObjectReadout.SetText(surface_type);
			}
		}
				
		if (IsPlacing()) {
			HandleHands(timeslice);
		}

		if (IsRunningCameraTrack()) {
			ProcessCameraTrack(timeslice);
		}
		
		if (IsDragging()) {
			GetDayZGame().ClearTooltip();
		}
	}
	
	// https://www.cubic.org/docs/hermite.htm
	private static float H00(float t) { return (2 * t * t * t) - (3 * t * t) + 1; }
	private static float H10(float t) { return (t * t * t) - (2 * t * t) + t; }
	private static float H01(float t) { return (-2 * t * t * t) + (3 * t * t); }
	private static float H11(float t) { return (t * t * t) - (t * t); }
	
	private static vector InterpolateHermite(vector p0, vector p1, vector m0, vector m1, float t)
	{
		return p0 * H00(t) + m0 * H10(t) + p1 * H01(t) + m1 * H11(t);
	}
	
	private static array<vector> ComputeHermiteTangents(array<vector> points, float tension = 0)
	{
		array<vector> tangents = {};
		tangents.Resize(points.Count());
		for (int i = 0; i < points.Count(); i++) {
			if (i == 0) {
				tangents[i] = (points[i + 1] - points[i]) * (1 - tension);
			} else if (i == points.Count() - 1) {
				tangents[i] = (points[i] - points[i - 1]) * (1 - tension);
			} else {
				tangents[i] = (points[i + 1] - points[i - 1]) * (0.5 * (1 - tension));
			}
		}
		
		return tangents;
	}
	
	private static array<vector> GenerateHermiteCurve(array<vector> points, int segments, float tension = 0)
	{
		array<vector> tangents = ComputeHermiteTangents(points, tension);
		array<vector> curve = {};
		for (int i = 0; i < points.Count() - 1; i++) {
			vector p0 = points[i];
			vector p1 = points[i + 1];
			vector m0 = tangents[i];
			vector m1 = tangents[i + 1];
			for (int j = 0; j <= segments; j++) {
				float t = (float)j / (float)segments;
				curve.Insert(InterpolateHermite(p0, p1, m0, m1, t));
			}
		}
		
		return curve;
	}
	
	protected void ProcessCameraTrack(float dt)
	{
		float smooth_value = 1 - GetEditorHud().GetTemplateController().CameraSmoothing;
		float speed = GetEditorHud().GetTemplateController().CameraTrackSpeed;
		array<EditorCameraTrack> camera_tracks = m_ObjectManager.GetCameraTracks();

		array<vector> camera_track_points = {};
		camera_track_points.Resize(camera_tracks.Count());
		for (int i = 0; i < camera_tracks.Count(); i++) {
			camera_track_points[i] = camera_tracks[i].GetPosition();
		}

#ifdef DIAG_DEVELOPER
		array<vector> debug_curve = GenerateHermiteCurve(camera_track_points, 20, smooth_value);
		for (int k = 0; k < debug_curve.Count() - 1; k++) {
			vector p[2] = { debug_curve[k], debug_curve[k + 1] };
			Shape.CreateLines(LinearColor.GREEN, ShapeFlags.ONCE, p, 2);
		}
#endif

		for (int c = 0; c < camera_tracks.Count(); c++) {
			EditorCameraTrack camera_track_hideshow = camera_tracks[c];
			if (camera_track_hideshow && camera_track_hideshow.GetWorldObject()) {
				float camera_distance_from_track = vector.Distance(camera_track_hideshow.GetPosition(), m_EditorCamera.GetPosition());
				camera_track_hideshow.Show(!IsRunningCameraTrack() && camera_distance_from_track > 10);
			}
		}

		vector camera_transform[4];
		m_EditorCamera.GetTransform(camera_transform);

		if (m_CameraTrackIndex >= camera_tracks.Count() - 1) {
			StopCameraTrack();
			return;
		}

		EditorCameraTrack camera_track_current = camera_tracks[m_CameraTrackIndex];
		EditorCameraTrack camera_track_next = camera_tracks[m_CameraTrackIndex + 1];
		
		float q1[4], q2[4];
		vector m1[4], m2[4];
		camera_track_current.GetTransform(m1);
		Math3D.MatrixToQuat(m1, q1);
		
		camera_track_next.GetTransform(m2);
		Math3D.MatrixToQuat(m2, q2);
				
		vector track_direction = m2[3] - m1[3];
		float track_length = track_direction.Length();

		float distance_traversed = speed * dt;

		m_CameraTrackLerpNorm = (m_CameraTrackLerpNorm * track_length + distance_traversed) / track_length;
		if (m_CameraTrackLerpNorm > 1.0) {
			m_CameraTrackIndex++;
			m_CameraTrackLerpNorm = 0.0;
			return;
		}
		
		array<vector> tangents = ComputeHermiteTangents(camera_track_points, smooth_value);
		vector p0 = camera_track_points[m_CameraTrackIndex];
		vector p1 = camera_track_points[m_CameraTrackIndex + 1];
		vector tan0 = tangents[m_CameraTrackIndex];
		vector tan1 = tangents[m_CameraTrackIndex + 1];
		
		vector camera_position = InterpolateHermite(p0, p1, tan0, tan1, m_CameraTrackLerpNorm);
		
		float qout[4];
		Math3D.QuatLerp(qout, q1, q2, m_CameraTrackLerpNorm);
		
		vector mout[4];
		Math3D.QuatToMatrix(qout, mout);
		mout[3] = camera_position;
		
		m_EditorCamera.SetTransform(mout);
		m_EditorCamera.Update();
	}
	
	protected vector m_HandsInputOrientation;
	
	// maybe abstract this to a new class, like EditorHandsManager
	void HandleHands(float dt)
	{
		Input input = GetGame().GetInput();
		array<Object> objects_to_ignore = { m_Player };
		foreach (EditorWorldObject world_object_0, EditorHandData hand_data_0: m_PlacingObjects) {
			if (world_object_0 && world_object_0.GetWorldObject()) {
				objects_to_ignore.Insert(world_object_0.GetWorldObject());
			}
		}
		
		Raycast cursor_raycast = GetCursorRaycastModeSafeEx(objects_to_ignore, GroundMode);
		vector position;
		if (cursor_raycast) {
			position = cursor_raycast.Bounce.Position;
		} else {
			position = GetCursorRay().GetPoint(50); // rather arbitrary
		}
					
		vector transform[4];
		Math3D.MatrixIdentity4(transform);
		
		vector surface_normal = vector.Up;
		if (MagnetMode) {
			surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		}
		
		vector local_aside = m_MainHandObject.GetWorldObject().GetDirection();
		if (false) {
			vector camera_transform[4];
			m_EditorCamera.GetTransform(camera_transform);
			local_aside = camera_transform[0];
		}
		
		transform[0] = surface_normal * local_aside;
		transform[1] = surface_normal;
		transform[2] = surface_normal * (local_aside * vector.Up);
		transform[3] = position + Vector(0, m_MainHandObject.GetWorldObject().GetBoundingCenter()[1], 0).Multiply3(transform);
		
		vector userinput_matrix[3];
		Math3D.YawPitchRollMatrix(m_HandsInputOrientation, userinput_matrix);
		Math3D.MatrixMultiply3(transform, userinput_matrix, transform);
		m_HandsInputOrientation = vector.Zero;
						
		foreach (EditorWorldObject world_object, EditorHandData hand_data: m_PlacingObjects) {
			if (!world_object || !world_object.GetWorldObject()) {
				continue;
			}
			
			vector local_transform[4];
			copyarray(local_transform, transform);
			if (hand_data) {
				vector hand_matrix[4];
				Math3D.YawPitchRollMatrix(hand_data.OrientationOffset, hand_matrix);
				hand_matrix[3] = hand_data.PositionOffset;
				Math3D.MatrixMultiply4(transform, hand_matrix, local_transform);
			}
						
			world_object.SetTransform(local_transform);
			
			//SnapToNearbyObjects(world_object, dt);
		}
	}
				
	void ProcessInput(float dt, Input input)
	{
		bool input_unlocked = (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget)) && !GetEditorHud().GetDialog();
		if (!input_unlocked) {
			return;
		}
		
		if (GetUApi().GetInputByName("UACOTModuleToggleCOT")) {
			GetUApi().GetInputByName("UACOTModuleToggleCOT").Supress();
			GetUApi().GetInputByName("UACOTToggleButtons").Supress();
			GetUApi().GetInputByName("UACOTTogglePlayer").Supress();
			GetUApi().GetInputByName("UACOTToggleCamera").Supress();
			GetUApi().GetInputByName("UACOTToggleESP").Supress();
			GetUApi().GetInputByName("UACOTToggleMap").Supress();
			GetUApi().GetInputByName("UACameraToolSpeedIncrease").Supress();
			GetUApi().GetInputByName("UACameraToolSpeedDecrease").Supress();
			GetUApi().UpdateControls();
		}
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
		Raycast cursor_raycast = GetCursorRaycast();
		Widget widget_under_cursor = GetWidgetUnderCursor();
		UAInputAPI input_api = GetUApi();
		UAInput fwd_input = input_api.GetInputByName("EditorMoveObjectForward");
		UAInput bck_input = input_api.GetInputByName("EditorMoveObjectBackward");
		UAInput left_input = input_api.GetInputByName("EditorMoveObjectLeft");
		UAInput right_input = input_api.GetInputByName("EditorMoveObjectRight");
		UAInput up_input = input_api.GetInputByName("EditorMoveObjectUp");
		UAInput down_input = input_api.GetInputByName("EditorMoveObjectDown");
		UAInput turbo_input = input_api.GetInputByID(UATurbo);
		UAInput slow_input = input_api.GetInputByID(UALookAround);
		UAInput big_input = input_api.GetInputByName("EditorScaleIncrease");
		UAInput small_input = input_api.GetInputByName("EditorScaleDecrease");
		UAInput left_click_input = input_api.GetInputByID(UAFire);
		UAInput right_click_input = input_api.GetInputByID(UATempRaiseWeapon);
		UAInput middle_click_input = input_api.GetInputByID(UAZoomIn);
		
		if (m_CameraMoveActive) {
			m_CameraMoveActive = GetUApi().GetInputByID(UATempRaiseWeapon).LocalValue();
		}
		
		if (GetUApi().GetInputByID(UATempRaiseWeapon).LocalPress() && !GetWidgetUnderCursor()) {
			m_CameraMoveActive = true;
		}

		bool any_mouse_click = left_click_input.LocalPress() || right_click_input.LocalPress() || middle_click_input.LocalPress();

		if (right_click_input.LocalPress()) {
			
			// Opens context menu when right clicking objects. but this is a drastic change im not ready for
			/*if (cursor_raycast.Hit) {
				EditorObject editor_object_context = EditorObject.s_AllByObject[cursor_raycast.Hit];
				if (editor_object_context) {
					if (EditorHud.CurrentMenu) {
						delete EditorHud.CurrentMenu;
					}
					
					EditorHud.CurrentMenu = new EditorPlacedContextMenu(mouse_x, mouse_y, editor_object_context);
					return;
				}
			}*/
			// no right click activity for now
		}
	
		// Clear focus, specifically after we check and create things that could be deleted here
		if (any_mouse_click && !widget_under_cursor) {
			SetFocus(null);
			delete EditorHud.CurrentMenu;			
			GetEditorHud().SetCurrentTooltip(null);
		}

		if (left_click_input.LocalDoubleClick()) {
			if (m_LootEditMode && !widget_under_cursor) {
				if (cursor_raycast && cursor_raycast.Bounce) {
					InsertLootPosition(cursor_raycast.Bounce.Position);
					return;
				}
			}
		}
				
		//	left click logic
		if (left_click_input.LocalPress()) {
#ifdef GIZMOS_ENABLED
			if (m_CurrentGizmo && m_CurrentGizmo.IsInteracting()) {
				return;
			}
#endif
			
			if (LightningMode && cursor_raycast) {
				CreateLightning(cursor_raycast.Bounce.Position);
				return;
			}
			
			if (IsPlacing()) {
				PlaceObject();
				return;
			}
			
			if (IsCtrlDown() && m_ObjectUnderCursor && !widget_under_cursor) {
				EditorPlaceableItem placeable_object = GetReplaceableItem(m_ObjectUnderCursor);
				if (placeable_object) {
					ClearHand();
					
					EditorHandMap objects_in_hand = AddInHand(placeable_object);
					foreach (EditorWorldObject object_in_hand, EditorHandData hand_data: objects_in_hand) {
						object_in_hand.GetWorldObject().SetOrientation(m_ObjectUnderCursor.GetOrientation());
					}
				}
				
				return;
			}
			
			if (!widget_under_cursor || widget_under_cursor == m_EditorHud.Map || widget_under_cursor.GetName() == "HudPanel") { //
				if (cursor_raycast && cursor_raycast.Hit && GetEditorHud().IsObjectSelectionEnabled()) {
					EditorObject select_object = EditorObject.s_AllByObject[cursor_raycast.Hit];
					if (select_object) {
						// We want to Toggle selection if you are holding control
						if (IsCtrlDown()) {
							ToggleSelection(select_object);
							return;
						} 
													
						if (!turbo_input.LocalValue()) {
							ClearSelection();
						}
						
						SelectObject(select_object);
						return;
					}
				}
						
				ClearSelection();
				return;
			}
		}

		if (middle_click_input.LocalPress()) {
			// Ctrl + Middle Mouse logic
			if (IsCtrlDown()) {
				if (m_ObjectUnderCursor) {			
					ClearSelection();
					if (GetEditorObject(m_ObjectUnderCursor)) {
						DeleteObject(GetEditorObject(m_ObjectUnderCursor));
					} else {
						GetGame().ObjectDelete(m_ObjectUnderCursor);
						HideMapObject(m_ObjectUnderCursor);
					}
				}

				return;
			} 
		}
		
		if (!IsPlacing()) {
			if (input_api.GetInputByName("EditorCycleWidget").LocalPress()) {
				SetMode(m_EditorMode + 1);
			}
		} else {
			if (input_api.GetInputByID(UAReloadMagazine).LocalPress()) {
				m_HandsInputOrientation = m_HandsInputOrientation + Vector(90, 0, 0);
			}
			
			float factor = 9;
			if (IsShiftDown()) {
				factor /= 5;
			}
			
			if (IsCtrlDown()) {
				factor *= 5;
			}
			
			if (input.LocalValue("UAZoomInOptics")) {				
				m_HandsInputOrientation[0] = m_HandsInputOrientation[0] - factor;
			}
			
			if (input.LocalValue("UAZoomOutOptics")) {
				m_HandsInputOrientation[0] = m_HandsInputOrientation[0] + factor;
			}
		}
									
		EditorObjectMap selected_objects = GetSelectedObjects();
		if (selected_objects.Count() == 0 && IsPlacing()) {
			/*
			int input_direction = fwd_input.LocalPress() + fwd_input.LocalHold() - bck_input.LocalPress() - bck_input.LocalHold();
			input_direction = Math.Clamp(input_direction, -1, 1);
			if (input_direction) {
				auto placeables = Ternary<ObservableCollection<ref EditorPlaceableListItem>>.If(GetEditorHud().GetTemplateController().CategoryConfig, GetEditorHud().GetTemplateController().LeftbarSpacerConfig, GetEditorHud().GetTemplateController().LeftbarSpacerStatic);
				for (int i = 0; i < placeables.Count(); i++) {
					if (placeables[i].IsSelected()) {
						if (!placeables[i + input_direction]) {
							continue;
						}
						
						placeables[i].Deselect();
						AddInHand(placeables[i + input_direction].GetPlaceableItem());
						placeables[i + input_direction].Select();
						
						// Handle tooltip showing
						placeables[i].OnMouseLeave(null, null, 0, 0);
						placeables[i + input_direction].OnMouseEnter(null, 0, 0);
						
						GetEditorHud().GetTemplateController().LeftbarScroll.VScrollToPos01((i + 1) /  placeables.Count());
						break;
					}
				}
			}*/
		} else if (selected_objects.Count()) {
			bool fwd_on = (fwd_input.LocalValue() && !GridMode) || (fwd_input.LocalHold() && GridMode) || (fwd_input.LocalPress() && GridMode);
			bool bck_on = (bck_input.LocalValue() && !GridMode) || (bck_input.LocalHold() && GridMode) || (bck_input.LocalPress() && GridMode);
			bool left_on = (left_input.LocalValue() && !GridMode) || (left_input.LocalHold() && GridMode) || (left_input.LocalPress() && GridMode);
			bool right_on = (right_input.LocalValue() && !GridMode) || (right_input.LocalHold() && GridMode) || (right_input.LocalPress() && GridMode);
			bool up_on = (up_input.LocalValue() && !GridMode) || (up_input.LocalHold() && GridMode) || (up_input.LocalPress() && GridMode);
			bool down_on = (down_input.LocalValue() && !GridMode) || (down_input.LocalHold() && GridMode) || (down_input.LocalPress() && GridMode);
			bool big_on = big_input.LocalValue();
			bool small_on = small_input.LocalValue();
	
			// Check if block should execute (movement OR pending action)
			bool should_execute = fwd_on || bck_on || left_on || right_on || up_on || down_on || big_on || small_on;
			if (should_execute)
			{
				if (IsDragging())
				{
#ifdef DEV_LOGGING
					PrintFormat("[PROCESS_INPUT] BLOCKED: A drag operation is in progress. Keyboard movement processing skipped for this frame.");
#endif
					return;
				}
				
#ifdef DEV_LOGGING
				PrintFormat("[PROCESS_INPUT] EXECUTING: Keyboard movement detected for %1 selected objects.", selected_objects.Count());
#endif
	
				array<ref EditorObject> updated_objects = new array<ref EditorObject>();
	
				m_ObjectManager.RecalculateCenterOfSelectedObjects();
				vector average_position = GetAveragePositionOfSelection();
				vector average_mat[4] = {
					"1 0 0",
					"0 1 0",
					"0 0 1",
					average_position
				};
				
				float step_size = GetSettings().QuickMoveRate;
				if (turbo_input.LocalValue()) {
					step_size *= 6.685;
				}
				
				if (slow_input.LocalValue()) {
					step_size /= 6.685;
				}
				
				step_size *= dt;
				
				vector camera_transform_mat[4];
				GetCamera().GetTransform(camera_transform_mat);
	
				switch (GetSettings().QuickMoveMode) {
					case 0: { // World flat
						camera_transform_mat[0] = vector.Aside;
						camera_transform_mat[1] = vector.Up;
						camera_transform_mat[2] = vector.Forward;
						break;
					}
	
					case 1: { // Camera flat
						camera_transform_mat[1] = vector.Up;
						camera_transform_mat[2] = (camera_transform_mat[0] * vector.Up).Normalized();
						Math3D.MatrixOrthogonalize4(camera_transform_mat);
						break;
					}
	
					case 2: { // Camera 3d
						break;
					}
				}
				
				array<UAInput> input_list = { fwd_input, bck_input, left_input, right_input, up_input, down_input, big_input, small_input };
				
				bool input_is_press = false;
				bool input_is_release = false;
				foreach (UAInput input_in_list2: input_list) {
					if (!input_in_list2) continue;
					input_is_press = input_is_press || input_in_list2.LocalPress();
				}
				
				if (input_is_press) {					
					if (GetGame().IsMultiplayer() && selected_objects.Count() > 0)
					{
						EditorObject parent = selected_objects.GetElement(0);
						array<EditorObject> children = {};
						for (int j = 1; j < selected_objects.Count(); j++)
							children.Insert(selected_objects.GetElement(j));
	
						GetNetActionManager().SendDragSessionStart(parent, children);
					}
				}
				
				foreach (UAInput check_release : input_list)
				{
					if (!check_release) continue;
					input_is_release = input_is_release || check_release.LocalRelease();
				}
				
				if (input_is_press) {
					CreateCheckpoint(selected_objects);
				}
	
				if (input_is_release)
				{					
					if (GetGame().IsMultiplayer() && selected_objects.Count() > 0)
					{
						EditorObject parent_end = selected_objects.GetElement(0);
						int packed[4];
						EditorNetUtils.PackTransform(parent_end.GetPosition(), parent_end.GetOrientation(), parent_end.GetScale(), packed);
						GetNetActionManager().SendDragSessionEnd(parent_end.Uuid, packed);
					}
				}
	
				if (GridMode) {
					step_size = GetGridSize();
				}
				
				vector pos_offset = vector.Zero;
				vector ori_offset = vector.Zero;
				float scale_offset = 0;
				if (g_Game.IsLeftCtrlDown() && fwd_on) {
					ori_offset = ori_offset + Vector(0, 0, step_size);
				}
				
				else if (fwd_on) {
					pos_offset = pos_offset + Vector(0, 0, step_size).Multiply3(camera_transform_mat);
				}
				
				if (g_Game.IsLeftCtrlDown() && bck_on) {
					ori_offset = ori_offset + Vector(0, 0, -step_size);
				}
				
				else if (bck_on) {
					pos_offset = pos_offset + Vector(0, 0, -step_size).Multiply3(camera_transform_mat);
				}
				
				if (g_Game.IsLeftCtrlDown() && left_on) {
					ori_offset = ori_offset + Vector(-step_size, 0, 0);
				}
				
				else if (left_on) {
					pos_offset = pos_offset + Vector(-step_size, 0, 0).Multiply3(camera_transform_mat);
				}
				
				if (g_Game.IsLeftCtrlDown() && right_on) {
					ori_offset = ori_offset + Vector(step_size, 0, 0);
				}
				
				else if (right_on) {
					pos_offset = pos_offset + Vector(step_size, 0, 0).Multiply3(camera_transform_mat);
				}
				
				if (g_Game.IsLeftCtrlDown() && up_on) {
					ori_offset = ori_offset + Vector(0, step_size, 0);
				}	
						
				else if (up_on) {
					pos_offset = pos_offset + Vector(0, step_size, 0).Multiply3(camera_transform_mat);
				}
				
				if (g_Game.IsLeftCtrlDown() && down_on) {
					ori_offset = ori_offset + Vector(0, -step_size, 0);
				}
				else if (down_on) {
					pos_offset = pos_offset + Vector(0, -step_size, 0).Multiply3(camera_transform_mat);
				}
				
				if (big_input.LocalValue()) {
					scale_offset = step_size;
				}
				
				if (small_input.LocalValue()) {
					scale_offset = -step_size;
				}
				
				ori_offset = ori_offset + ori_offset * Math.RAD2DEG;
						
				if (pos_offset != vector.Zero || ori_offset != vector.Zero || scale_offset != 0) {
					foreach (int id, EditorObject selected_object: selected_objects) {
						vector rel_mat[4];
						selected_object.GetTransform(rel_mat);
						
						// CAPTURE ORIGINAL SCALE BEFORE TRANSFORMATIONS
						float originalScale0 = rel_mat[0].Length();
						float originalScale1 = rel_mat[1].Length();
						float originalScale2 = rel_mat[2].Length();
						
						vector inv_mat[4];
						Math3D.MatrixInvMultiply4(average_mat, rel_mat, inv_mat);
						inv_mat[3] = inv_mat[3] + pos_offset;
						
						vector avg_mat[4];
						Math3D.YawPitchRollMatrix(ori_offset, avg_mat);
						avg_mat[3] = average_position;					
						vector res_mat[4];
						Math3D.MatrixMultiply4(avg_mat, inv_mat, res_mat);
						
						// Apply scale using ORIGINAL scale + offset
						res_mat[0] = res_mat[0].Normalized() * (originalScale0 + scale_offset);
						res_mat[1] = res_mat[1].Normalized() * (originalScale1 + scale_offset);
						res_mat[2] = res_mat[2].Normalized() * (originalScale2 + scale_offset);
						
						selected_object.SetTransform(res_mat);
						
						updated_objects.Insert(selected_object);
						selected_object.Update();
					}
	
					if (GetGame().IsMultiplayer() && updated_objects.Count() > 0)
					{
						EditorObject rpc_update_parent = updated_objects[0];
						int rpc_update_packed[4];
						EditorNetUtils.PackTransform(rpc_update_parent.GetPosition(), rpc_update_parent.GetOrientation(), rpc_update_parent.GetScale(), rpc_update_packed);
						GetNetActionManager().SendDragSessionUpdate(rpc_update_parent.Uuid, rpc_update_packed);
					}
				}
			}
		}
		
		bool useful_widget_under_cursor = GetWidgetUnderCursor() && GetWidgetUnderCursor().GetName() != "HudPanel" && GetWidgetUnderCursor().GetName() != "CursorIcons";		
		if (GetCamera() && GetCamera().GetSettings() && !GetCamera().GetSettings().LegacyCamera && !useful_widget_under_cursor && !IsPlacing()) {
			float scale_change_value = 0.1 * GetCamera().GetSettings().Speed;
			if (input.LocalValue("EditorCameraToolSpeedIncrease")) {
				GetCamera().GetSettings().Speed += scale_change_value;
			}
			
			if (input.LocalValue("EditorCameraToolSpeedDecrease")) {
				GetCamera().GetSettings().Speed -= scale_change_value;
			}
			
			GetCamera().GetSettings().Speed = Math.Clamp(GetCamera().GetSettings().Speed, EditorCamera.SPEED_MIN, EditorCamera.SPEED_MAX);
		}
		
		// This is all the logic that controls inventory hud, not a fan but it works
		// update: it doesnt work
		// update 2: it works
		if (m_Player && !m_Active) {					
			if (input.LocalPress("EditorToggleInventoryEditor", false)) {
				if (m_EditorInventoryEditorHud) {
					StopInventoryEditor();
				}
				
				else {
					GetGame().GetMission().HideInventory();
					// Default to m_Player
					StartInventoryEditor(m_Player);
				}
				
				return;
			}
			
			if (input.LocalPress("EditorToggleInventory", false)) {
				if (m_EditorInventoryEditorHud) {
					StopInventoryEditor();
				}
			}
		}
	}
		
	bool IsShiftDown()
	{
		return KeyState(KeyCode.KC_LSHIFT) || KeyState(KeyCode.KC_RSHIFT);
	}
	
	bool IsCtrlDown()
	{
		return KeyState(KeyCode.KC_LCONTROL) || KeyState(KeyCode.KC_RCONTROL);
	}
	
	bool IsSurfaceWater( vector position )
	{
		CGame game = GetGame();
		return game.SurfaceIsSea( position[0], position[2] ) || game.SurfaceIsPond( position[0], position[2] );
	}
	
	bool OnMouseRelease(int button)
	{
		return false;
	}
		
	// Return TRUE if handled.	
	bool OnKeyPress(int key)
	{
		// Dont process hotkeys if dialog is open
												// HACK
		if ((m_EditorHud.GetDialog() || m_EditorHud.CurrentDialog) && key != KeyCode.KC_ESCAPE) {
			return false;
		}
				
		//if (!GetGame().GetInput().HasGameFocus(INPUT_DEVICE_KEYBOARD)) {
			//return false;
		//}
		
		if (IsPlayerControlled()) {
			return false;
		}
		
		Widget focus = GetFocus();
		if (focus && focus.IsInherited(EditBoxWidget)) {
			return true;
		}
		
		if (m_CurrentKeys.Find(key) != -1) {
			return true;
		}
				
		m_CurrentKeys.Insert(key);
		EditorCommand command = CommandManager.GetCommandFromShortcut(m_CurrentKeys.GetMask());
		if (!command) {
			return true;
		}
		
		if (!command.CanExecute()) {
			return false;
		}
			
		EditorLog.Debug("Hotkeys Pressed for %1", command.ToString());
		
		// Flush all inputs when a command is pressed. To avoid things like the camera moving constantly after a lag on save
		UAInputAPI input_api = GetUApi();
		array<UAInput> inputs = {
			input_api.GetInputByID(UAMoveForward),
			input_api.GetInputByID(UAMoveBack),
			input_api.GetInputByID(UAMoveRight),
			input_api.GetInputByID(UAMoveLeft),
			input_api.GetInputByID(UAMoveUp),
			input_api.GetInputByID(UAMoveDown),
		};
		
		foreach (UAInput input: inputs) {
			if (input) {
				input.Supress();
			}
		}
		
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
	// this code is TERRIBLE 11/9
	// update: im makin it worse 11/12
	private bool _bugfixFirstGrab;
	void SetActive(bool active)
	{			
		EditorLog.Info("Set Active %1", active.ToString());		
		m_Active = active;
				
		// Shut down Inventory Editor, done prior to the camera due to the destructor
		if (m_EditorInventoryEditorHud) {
			delete m_EditorInventoryEditorHud;
		}
				
		if (!GetGame().IsMultiplayer()) {
			if (m_Active) {
				if (m_EditorCamera) {
					m_EditorCamera.SetActive(true);
				}
			} else {
				GetGame().SelectPlayer(null, m_ControllingPlayer);
			}
		} else {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(m_Active);
			rpc.Send(null, 39261, true);
		}
		
		if (m_EditorHud) {
			m_EditorHud.Show(m_Active);
			m_EditorHud.SetCurrentTooltip(null);
		}
				
		EditorObjectMap placed_objects = GetPlacedObjects();
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				if (!editor_object) {
					continue;
				}
				
				if (editor_object.GetMarker()) {
					editor_object.GetMarker().Show(m_Active);			
				}
				
				editor_object.HideBoundingBox();
			}
		}
				
		GetGame().GetUIManager().ShowCursor(m_Active);
		
		if (m_Player && !IsPlayerControlled()) {
			m_Player.GetInputController().SetDisabled(m_Active);
			
			if (!_bugfixFirstGrab) {
				m_Player.DisableSimulation(false);
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_Player.DisableSimulation, 1000, 0, m_Active);
				_bugfixFirstGrab = true;
			} else {
				m_Player.DisableSimulation(m_Active);
			}
		}
		
		SetMissionHud(!m_Active);
		PPEffects.ResetAll();
	}
	
	// also called when component index changes
	bool OnMouseEnterObject(Object target, int x, int y, int component_index)
	{
		string type = target.GetType();
		EditorPlaceableItem replaceable_item = GetReplaceableItem(target);
		string replaceable_type = m_ObjectManager.ConvertP3dFileToPotentialObjectType(SystemPath.Format(target.GetShapeName()));
		if (!type) {
			type = replaceable_type;
		}

		if (!IsPlacing() && !IsDragging() && (!m_CurrentGizmo || !m_CurrentGizmo.IsInteracting()) && !GetWidgetUnderCursor() && !GetSelectedObjects().Count()) {			
			//GetEditorHud().CreateDelayedTooltip(null, type, TooltipPosition.BOTTOM_LEFT, string.Format("(%1)", target.GetShapeName()));
		}

		Building building = Building.Cast(target);
		string component_type = "Component";
		if (building) {
			if (building.GetDoorIndex(component_index) != -1) {
				component_index = building.GetDoorIndex(component_index);
				component_type = "Door";
			}
		}

		int interaction_layer = dBodyGetInteractionLayer(target);
		string interaction_layer_name;
		string interaction_layer_single;
		if (interaction_layer) {
			if (interaction_layer & (interaction_layer - 1) == 0) {
				interaction_layer_single = typename.EnumToString(PhxInteractionLayers, interaction_layer);
				if (interaction_layer_single != "unknown") {
					interaction_layer_name = string.Format(", %1", interaction_layer_single, interaction_layer);	
				}
			} else {
				for (int i = 0; i < 32; i++) {
					int j = (1 << i);
					if (interaction_layer & j) {
						interaction_layer_single = typename.EnumToString(PhxInteractionLayers, j);
						if (interaction_layer_single != "unknown") {
							interaction_layer_name += string.Format(", %1", interaction_layer_single, j);
						}
					}
				}
			}
		}
	
		m_EditorHudController.ObjectHoverSelectObjectReadout.SetText(string.Format("%1 [%2: %3%4]", type, component_type, component_index, interaction_layer_name));
		if (!replaceable_item) {
			m_EditorHudController.ObjectHoverSelectObjectReadout.SetColor(COLOR_YELLOW);
		} else {
			m_EditorHudController.ObjectHoverSelectObjectReadout.SetColor(COLOR_WHITE);
		}
		
		return true;
	}
	
	// also called when component index changes
	bool OnMouseExitObject(Object target, int x, int y, int component_index)
	{
		bool useful_widget_under_cursor = GetWidgetUnderCursor() && GetWidgetUnderCursor().GetName() != "HudPanel" && GetWidgetUnderCursor().GetName() != "CursorIcons";
		if (!IsPlacing() && (!m_CurrentGizmo || !m_CurrentGizmo.IsInteracting()) && !useful_widget_under_cursor) {
			GetEditorHud().SetCurrentTooltip(null);
		}

		m_EditorHudController.ObjectHoverSelectObjectReadout.SetText(string.Empty);
		return true;
	}	
	
	void RemoveFromHand(EditorWorldObject world_object)
	{
		EditorEvents.RemoveFromHand(this, world_object, m_PlacingObjects[world_object]);
		m_PlacingObjects.Remove(world_object);
		delete world_object;		
	}
	
	void ClearHand()
	{
		EditorLog.Trace("Editor::ClearHand");
		foreach (EditorWorldObject world_object, EditorHandData hand_data: m_PlacingObjects) {
			RemoveFromHand(world_object);
		}
	}
	
	array<EditorWorldObject> GetPlacingObjects()
	{
		return m_PlacingObjects.GetKeyArray();
	}
	
	EditorHandData GetObjectInHandData(EditorWorldObject world_object)
	{
		return m_PlacingObjects[world_object];
	}
	
	int SnapToNearbyObjects(notnull EditorWorldObject target_to_snap, float dt = 0.031)
	{			
		return 0;
		array<EditorSnapPoint> anchor_snap_points = EditorSnapPoint.GetSnapPointsInRange(target_to_snap.GetWorldObject(), 60);	
		
		vector min_max[2];
		target_to_snap.ClippingInfo(min_max);
		vector preview_extents = {
			Math.AbsFloat(min_max[1][0] - min_max[0][0]),
			Math.AbsFloat(min_max[1][1] - min_max[0][1]),
			Math.AbsFloat(min_max[1][2] - min_max[0][2])
		};
				
		vector preview_center = {
			min_max[1][0] - ((min_max[1][0] - min_max[0][0]) / 2),
			min_max[1][1] - ((min_max[1][1] - min_max[0][1]) / 2),
			min_max[1][2] - ((min_max[1][2] - min_max[0][2]) / 2)
		};	
		
		float preview_radius = Math.Max(preview_extents[0], preview_extents[2]);
				
		auto snap_points_and_distances = new PriorityQueue<ref Param5<EditorWorldObject, vector, vector, vector, vector>, float>();
		array<ref EditorSnapPoint> preview_snap_points = target_to_snap.GetSnapPoints();
		
		// Create the queue of snapping points, ranked by distance
		foreach (EditorSnapPoint snap_point_1: preview_snap_points) {
			if (!snap_point_1) {
				continue;		
			}
			
			foreach (EditorSnapPoint snap_point_2: anchor_snap_points) {					
				if (!snap_point_2) {
					continue;
				}
				
				float cam_dist = vector.Distance(m_EditorCamera.GetPosition(), snap_point_2.GetWorldPosition());
				
				vector snap_forward = snap_point_2.GetDirectionAside() * snap_point_2.GetDirectionUp();
				if (vector.Dot(snap_forward, m_CursorRay.Direction) < 0) {
					snap_forward = -snap_forward;
				}
				
				// are you too far?
				if (cam_dist > 500) {
					continue;
				}

				//snap_point_1 is the hologram
				//snap_point_2 is the receiver

				//if hologram base has the same acceptedSnapPoints as the receiver
				
				if (!snap_point_2.IsValidSnap(snap_point_1)) {
					continue;
				}

				//if hologram base has the same acceptedTypes as the receiver
				
				/*if (!snap_point_2.IsValidType(snap_point_1.GetSource().GetType())) {
#ifdef DIAG_DEVELOPER
#ifdef SNAPPING_DIAG_VERBOSE
					//DbgUI.Text(string.Format("    anc:%1:%2 denied %3:%4", snap_point_2.GetSource().GetType(), snap_point_2.GetName(), snap_point_1.GetSource().GetType(), snap_point_1.GetName()));
#endif
#endif
					continue;
				}
				
				if (snap_point_1.GetSource().IsOccupancyExclusive() && snap_point_2.IsOccupied()) {
#ifdef DIAG_DEVELOPER
#ifdef SNAPPING_DIAG_VERBOSE
					//DbgUI.Text(string.Format("    anc:%1 denied %2 (exclusive snap only)", snap_point_2.GetSource().GetType(), snap_point_1.GetSource().GetType()));
#endif
#endif
					continue;
				}
				*/
				
				vector snap_target_transformation[4];
				snap_point_1.SnapTo(snap_point_2, snap_target_transformation, false);
				
				//if (Math.AbsFloat(vector.Dot(snap_target_transformation[1], target_transform[1])) > 0.5) {
					//continue;
				//}
				
				// -1 is because the vectors will face away from each other
				float weight = 0;					
				float intersect_denom = vector.Dot(snap_forward, m_CursorRay.Direction);
				// check if anything is even real
				if (intersect_denom <= Math.EPSILON) {
					continue;
				}
								
				// center in world space
				vector preview_center_world = preview_center.Multiply4(snap_target_transformation);
				float t = vector.Dot(preview_center_world - m_CursorRay.Position, snap_forward) / intersect_denom;
				// check if we even intersect
				if (t <= 0) {
					continue;
				}
				
				float distance_from_center = vector.Distance(m_CursorRay.Position + m_CursorRay.Direction * t, preview_center_world);
				// check if your cursor projection is within the bounds of the rect
				if (distance_from_center > preview_radius) {
					//continue;
				}

				/*if (camera_raycast && camera_raycast.Hit && camera_raycast.Hit.IsInherited(BaseBuildingBase)) {
					continue;
				}*/

				// dot product of the camera dir and the direction between yourself and the center of the preview object
				float dot_1 = vector.Dot(m_CursorRay.Direction, vector.Direction(m_CursorRay.Position, preview_center_world).Normalized());
				
				float MAX_SNAP_DISTANCE = 10;
				weight = -MAX_SNAP_DISTANCE + Math.Sqrt(t) - 5 * dot_1 + Math.Sqrt(distance_from_center);
				
#ifdef DIAG_DEVELOPER					
#ifdef SNAPPING_DIAG_VERBOSE					
				Shape.CreateSphere(LinearColor.LIGHT_BLUE, ShapeFlags.ONCE, preview_center_world, 0.15);
				Shape.CreateArrow(preview_center_world, preview_center_world + snap_forward * 4, 1.0, LinearColor.LIGHT_BLUE, ShapeFlags.ONCE);
				
				vector dbg_p[2] = { snap_point_1.GetWorldPosition().Multiply4(snap_target_transformation), snap_point_2.GetWorldPosition() };
				Shape.CreateLines(LinearColor.BLACK, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, dbg_p, 2);
				
				//DayZPlayerUtils.DrawDebugText(string.Format("%3\ndot:%1 dst:%2 dot2: %4\nweight: %5 t:%6 dist_ctr: %7", dot_1, cam_dist, snap_point_2.GetName(), 0, weight, t, distance_from_center), preview_center_world, 1.0);
#endif
#endif
				//if (cam_dist < MAX_SNAP_DISTANCE * 3) {
					snap_points_and_distances.Enqueue(new Param5<EditorWorldObject, vector, vector, vector, vector>(snap_point_2.GetEditorObject(), snap_target_transformation[0], snap_target_transformation[1], snap_target_transformation[2], snap_target_transformation[3]), weight);
				//}
				
				// Try the reverse aswell, anchor point only!
				if (snap_point_2.AllowReverse) {						
					Math3D.MatrixIdentity4(snap_target_transformation);
					snap_point_1.SnapTo(snap_point_2, snap_target_transformation, true);
					if (cam_dist < MAX_SNAP_DISTANCE * 3) {
						snap_points_and_distances.Enqueue(new Param5<EditorWorldObject, vector, vector, vector, vector>(snap_point_2.GetEditorObject(), snap_target_transformation[0], snap_target_transformation[1], snap_target_transformation[2], snap_target_transformation[3]), weight);
					}
				}
			}
		}
	
		// empty that queue and acquire the beans
		if (snap_points_and_distances.Count() == 0) {
			return 0;
		}
		
		Param5<EditorWorldObject, vector, vector, vector, vector> closest_points = snap_points_and_distances.Dequeue();
		vector target_transform[4] = {
			closest_points.param2,
			closest_points.param3,
			closest_points.param4,
			closest_points.param5
		};		
		
		//@ Lerp motion if setting is enabled
		vector current_transform[3];
		target_to_snap.GetWorldEntity().GetTransform(current_transform);
		
		float q0[4], q1[4], q2[4];
		Math3D.MatrixToQuat(target_transform, q0);
		Math3D.MatrixToQuat(current_transform, q1);
		Math3D.QuatLerp(q2, q1, q0, dt * 5);
		
		vector frame_transform[4];
		Math3D.QuatToMatrix(q2, frame_transform);
		frame_transform[3] = target_transform[3];
		target_to_snap.GetWorldEntity().MoveInTime(target_transform, dt);
		
		return 1;
	}
	
	EditorHandMap AddInHand(EditorWorldObject world_object, EditorHandData hand_data = null)
	{		
		// Turn Brush off when you start to place
		if (m_EditorHud.GetBrushState()) {
			m_EditorHud.SetBrushState(0);
		}
		
		if (m_PlacingObjects.Count() == 0) {
			m_MainHandObject = world_object;
		}
		
		m_PlacingObjects[world_object] = hand_data;
		EditorEvents.AddInHand(this, world_object, hand_data);
		
		if (!IsShiftDown()) {
			ClearSelection();
		}
		
		return m_PlacingObjects;
	}
	
	EditorHandMap AddInHand(EditorPlaceableItem item, EditorHandData hand_data = null)
	{				
		return AddInHand(new EditorHologram(item), hand_data);				
	}
		
	array<EditorObject> PlaceObject()
	{
		EditorLog.Trace("Editor::PlaceObject");
		// I hate all these exceptions but were dealing with vanilla UI bullshit
		if (GetWidgetUnderCursor() && !GetWidgetUnderCursor().IsInherited(MapWidget) && GetWidgetUnderCursor().GetName() != "HudPanel" && GetWidgetUnderCursor().GetName() != "CursorIcons") {
			return null;
		}
		
		if (!m_PlacingObjects || m_PlacingObjects.Count() == 0) {
			return null;	
		}
		
		array<EditorObject> placed_objects = {};
		array<ref EditorObjectData> data_list = {};
		foreach (EditorWorldObject placing_object, EditorHandData hand_data: m_PlacingObjects) {			
			//We need it like this rn cos internal model classnames (e.g. TreeHard)	
			EditorHologram editor_hologram;
			if (!Class.CastTo(editor_hologram, placing_object)) {
				continue;
			}

			Object entity = editor_hologram.GetWorldObject();
			if (!entity) {
				EditorLog.Warning("Invalid Entity from %1", editor_hologram.GetPlaceableItem().Type);
				return null;
			}
			
			EditorObjectData editor_object_data = EditorObjectData.Create(editor_hologram.GetPlaceableItem().GetSpawnType(), entity.GetPosition(), entity.GetOrientation(), entity.GetScale(), EFE_DEFAULT);
			if (!editor_object_data) {
				EditorLog.Warning("Invalid Object data from %1", entity.GetType());
				return null;
			}
			
			data_list.Insert(editor_object_data);
						
			if (!IsShiftDown()) { 
				RemoveFromHand(placing_object); 
			}
		}
						
		// Dont create the undo on multiplayer because its going to be immediately deleted when receiving the info from the server
		// you will create a null action and boomy
		// update: stumbled upon this and we are creating undo in mp. is this why item placement is bug?
		auto created_objects = CreateObjects(data_list);
		foreach (int id, EditorObject editor_object_created: created_objects) {
			
			EditorEvents.ObjectPlaced(this, editor_object_created);
			if (editor_object_created) {
				SelectObject(editor_object_created);
			}
			
			placed_objects.Insert(editor_object_created);
		}
					
		return placed_objects;
	}
	
	void CancelPlacing()
	{
		// todo: stub
		foreach (EditorWorldObject placing_object, EditorHandData hand_data: m_PlacingObjects) {
			RemoveFromHand(placing_object); 
		}
		//delete m_PlacingObjects;
		//m_PlacingObjects.Clear();
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
		
		m_PositionBeforeLootEditMode = m_EditorCamera.GetPosition();
		m_EditorCamera.SetPosition(Vector(10, LootYOffset, 10));
		m_EditorCamera.LookAt(Vector(0, LootYOffset, 0));	
		
		if (!FileExist(GetSettings().ProtoFile)) {
			EditorLog.Info("EditorProtoFile not found! Copying...");
			CopyFile("DayZEditor/scripts/data/Defaults/MapGroupProto.xml", GetSettings().ProtoFile);
		}
		
		m_EditorMapGroupProto = new EditorMapGroupProto(m_LootEditTarget); 
		EditorXMLManager.LoadMapGroupProto(m_EditorMapGroupProto, GetSettings().ProtoFile);
		
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
	
	void StartInventoryEditor(EntityAI entity, EditorObjectData object_data = null)
	{
		m_EditorInventoryEditorHud = new EditorInventoryEditorHud(entity, object_data);
		
		PlayerBase player;
		if (Class.CastTo(player, entity)) {
			player.GetInputController().SetDisabled(true);
		}
		
		if (GetPlayer()) {
			GetPlayer().GetInputController().SetDisabled(true);
		}
		
		SetMissionHud(false);	
		ClearSelection();
		GetStatistics().CharactersEdited++;
	}
	
	void StopInventoryEditor()
	{
		Entity ent = m_EditorInventoryEditorHud.GetEntity();
		EditorObject obj = GetEditorObject(ent);
		if (obj) {
			obj.Update();
		}		

		delete m_EditorInventoryEditorHud;
		Activate();
	}
	
	bool IsInventoryEditorActive()
	{
		return (m_EditorInventoryEditorHud != null);	
	}
	
	void SetMissionHud(bool state)
	{
		Mission mission = GetGame().GetMission();
		if (!mission) {
			EditorLog.Error("No mission active");
			return;
		}
		
		Hud hud = mission.GetHud();
		if (!hud) {
			EditorLog.Error("No Hud active");
			return;
		}
		
		hud.Show(state);
		hud.ShowHud(state);
		hud.ShowHudUI(state);
		// we are in 4_world and this game is bad :)
		Widget hud_root;
		EnScript.GetClassVar(mission, "m_HudRootWidget", 0, hud_root);
		if (hud_root) {
			hud_root.Show(state);
		}
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
			
			vector loot_pos = loot_spawn.GetPosition();
			loot_pos[1] = loot_pos[1] - LootYOffset;
			loot_position_data += string.Format("		<point pos=\"%1\" range=\"%2\" height=\"%3\" /> \n", loot_pos.ToString(false), loot_point.Range, loot_point.Height);
		}
		
		loot_position_data += "	</container>\n";
		loot_position_data += "</group>\n";
		
		GetGame().CopyToClipboard(loot_position_data);
		
		delete m_EditorMapGroupProto;
		
		GetGame().ObjectDelete(m_LootEditTarget);
		m_EditorCamera.SetPosition(m_PositionBeforeLootEditMode);

		m_LootEditMode = false;
		CollisionMode = false;
		
		thread EditLootSpawnFinishedDialog();
	}
	
	bool IsLootEditActive() 
	{ 
		return m_LootEditMode; 
	}

	// Call me first to safely and quickly clean up the editor. otherwise you're going to have a bad time
	void DestroyHud()
	{
		for (int i = 0; i < m_EditorHud.GetTemplateController().LeftContent.Count(); i++) {
			if (m_EditorHud.GetTemplateController().LeftContent[i].GetLayoutRoot()) {
				m_EditorHud.GetTemplateController().LeftContent[i].GetLayoutRoot().Unlink();
			}
			
			//delete m_EditorHud.GetTemplateController().LeftContent[i];
		}
		
		//m_EditorHud.GetTemplateController().LeftContent.Clear();				
		
		delete m_EditorHud;
	}
	
	EditorHud ReloadHud() 
	{
#ifdef DIAG_DEVELOPER
		DestroyHud();
		
		//m_EditorHud = new EditorHud(this);
		//m_EditorHudController = m_EditorHud.GetTemplateController();
		return m_EditorHud;
		
#endif
		return m_EditorHud;
	}

	protected int m_SnapshotIndex = 0;
	
	// 2D Array of groups of snapshots
	protected ref array<ref array<ref EditorSnapshot>> m_Snapshots = {};
	
	void CreateCheckpoint(notnull EditorObject editor_object)
	{
		EditorObjectSnapshot snapshot = new EditorObjectSnapshot(editor_object.GetController());
		InsertSnapshot(snapshot);
	}
	
	void CreateCheckpoint(notnull array<EditorObject> editor_objects)
	{		
		array<ref EditorSnapshot> snapshots = {};
		foreach (EditorObject editor_object: editor_objects) {
			snapshots.Insert(new EditorObjectSnapshot(editor_object.GetController()));
		}
		
		InsertSnapshots(snapshots);
	}
	
	void CreateCheckpoint(notnull EditorObjectMap editor_object_map)
	{
		CreateCheckpoint(editor_object_map.GetValueArray());
	}
	
	protected void InsertSnapshot(notnull EditorSnapshot snapshot)
	{		
		array<ref EditorSnapshot> snapshots = { snapshot };
		InsertSnapshots(snapshots);
	}
	
	protected void InsertSnapshots(notnull array<ref EditorSnapshot> snapshots)
	{
#ifdef DIAG_DEVELOPER
		PrintFormat("Snapshot Created for %1 objects", snapshots.Count());
#endif
		
		for (int i = m_Snapshots.Count() - 1; i >= m_SnapshotIndex + 1; i--) {
			m_Snapshots.Remove(i);
		}
		
		m_Snapshots.Insert(snapshots);
		m_SnapshotIndex = m_Snapshots.Count();
	}
		
	void Undo()
	{
		if (CanUndo()) {
			m_SnapshotIndex--;
			array<ref EditorSnapshot> snapshots = m_Snapshots[m_SnapshotIndex];
			for (int i = 0; i < snapshots.Count(); i++) {
				snapshots[i].Recall();
			}
		}
	}
	
	void Redo()
	{
		if (CanRedo()) {
			array<ref EditorSnapshot> snapshots = m_Snapshots[m_SnapshotIndex];
			for (int i = 0; i < snapshots.Count(); i++) {
				snapshots[i].UnRecall();
			}

			m_SnapshotIndex++;
		}
	}
	
	bool CanUndo() 
	{
		return m_SnapshotIndex > 0;
	}
	
	bool CanRedo() 
	{
		return m_SnapshotIndex < m_Snapshots.Count();
	}
			
	EditorObject CreateObject(notnull Object target, EditorObjectFlags flags = EFE_DEFAULT, bool create_undo = true) 
	{
		EditorLog.Trace("Editor::CreateObject " + target);	
		return CreateObject(EditorObjectData.Create(target, flags), create_undo);
	}
	
	EditorObject CreateObject(notnull EditorObjectData editor_object_data, bool create_undo = true) 
	{
		if (GetGame().IsMultiplayer())
		{
			if (m_ObjectManager.GetPlacedObjects().Count() >= 60000) {
				return null; 
			}

			string uuid = UUID.Generate();
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(1);
			rpc.Write(uuid);
			editor_object_data.Write(rpc, int.MAX);
			rpc.Send(null, 39252, true);

			// 2. Return NULL. Do not create the object locally.
			// The object will be created when the server's authoritative broadcast is received by OnERPC.
			return null;
		}
		else
		{

			if (m_ObjectManager.GetPlacedObjects().Count() >= 60000) {
				return null;
			}

			string uuid_sp = UUID.Generate();
			EditorObject created_object = CreateObjectByUuid(uuid_sp, editor_object_data, create_undo);

			if (created_object) {
				GetStatistics().EditorPlacedObjects++;
			}
							
			return created_object;
		}
	}
	
	EditorObject CreateObjectByUuid(string uuid, notnull EditorObjectData editor_object_data, bool create_undo = true)
	{		
		// Cache Data (for undo / redo)
		m_SessionCache[editor_object_data.GetID()] = editor_object_data;
						
		// Create Object
		EditorObject editor_object = m_ObjectManager.CreateObject(editor_object_data);
		if (!editor_object) {
			Error(string.Format("[Editor] CreateObjectByUuid FAILED: uuid=%1, type=%2 - EditorObject is NULL", uuid, editor_object_data.Type));
			return null;
		}
		
		editor_object.Uuid = uuid;
		m_EditorObjectsByUuid[uuid] = editor_object;

		if (create_undo) {			
			EditorObjectCreationSnapshot snapshot = new EditorObjectCreationSnapshot(editor_object.GetController());
			InsertSnapshot(snapshot);
		}
		
		GetStatistics().EditorPlacedObjects++;
		
		SelectObject(editor_object);
		
		return editor_object;
	}
	
	EditorObjectMap CreateObjects(notnull array<ref EditorObjectData> data_list, bool create_undo = true, bool send_net_message = true) 
	{

		if (GetGame().IsMultiplayer() && send_net_message) 
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(data_list.Count());
			for (int i = 0; i < data_list.Count(); i++) {
				string uuid = UUID.Generate();
				rpc.Write(uuid);
				data_list[i].Write(rpc, int.MAX);
			}
			
				// 1. Send the request to the server.
				rpc.Send(null, 39252, true);
			
			// 2. Do NOT create the object locally. Return an empty map and wait for the server's response.
			return new EditorObjectMap();
		}
		else
		{
			map<string, ref EditorObjectData> data_map = new map<string, ref EditorObjectData>();
			for (int j = 0; j < data_list.Count(); j++) {
				string uuid_sp = UUID.Generate();
				data_map[uuid_sp] = data_list[j];
			}
			
			array<EditorObject> created_objects_array = CreateObjectsByUuid(data_map, create_undo);

			EditorObjectMap created_objects_map = new EditorObjectMap();

			foreach (EditorObject obj : created_objects_array)
			{
				if (obj)
				{
					created_objects_map.Insert(obj.GetID(), obj);
				}
			}
			
			GetStatistics().EditorPlacedObjects += created_objects_map.Count();

			return created_objects_map;
		}
	}
		
	array<EditorObject> CreateObjectsByUuid(notnull map<string, ref EditorObjectData> data_list, bool create_undo = true)
	{
		array<EditorObject> object_set = new array<EditorObject>();
		
		foreach (string uuid, EditorObjectData editor_object_data: data_list) {
			
			// Cache Data (for undo / redo)
			if (!editor_object_data) continue;
			
			// In the event the object already exists. The data for the existing object will get packed into the EditorObjectData struct
			if (m_EditorObjectsByUuid[uuid]) {
				m_EditorObjectsByUuid[uuid].HideBoundingBox();
				
				Object existing_world_object = m_EditorObjectsByUuid[uuid].GetWorldObject();
				if (editor_object_data.WorldObject) {
					m_EditorObjectsByUuid[uuid].SetWorldObject(editor_object_data.WorldObject);
				}
				
				m_EditorObjectsByUuid[uuid].SetWorldObjectNetworkId(editor_object_data.m_LowBits, editor_object_data.m_HighBits);
				
				GetGame().ObjectDelete(existing_world_object);
				
				if (m_EditorObjectsByUuid[uuid].IsSelected()) {
					m_EditorObjectsByUuid[uuid].ShowBoundingBox();
				}
				
				object_set.Insert(m_EditorObjectsByUuid[uuid]);

				continue;
			}
						
			// Create a copy to avoid reference loss
			// todo:
			//EditorObjectData editor_object_data_copy = editor_object_data.CreateCopy();
			m_SessionCache.Insert(editor_object_data.GetID(), editor_object_data);
			
			// Create Object
			EditorObject editor_object = m_ObjectManager.CreateObject(m_SessionCache[editor_object_data.GetID()]);
			if (!editor_object) continue;
			
			if (create_undo) {
				EditorObjectCreationSnapshot snapshot = new EditorObjectCreationSnapshot(editor_object.GetController());
				InsertSnapshot(snapshot);
			}

			editor_object.Uuid = uuid;
			m_EditorObjectsByUuid[uuid] = editor_object;
			
			GetStatistics().EditorPlacedObjects++;

			object_set.Insert(editor_object);

		}
				
		return object_set;
	}
		
	bool DeleteObject(notnull EditorObject editor_object, bool create_undo = true, bool send_net_message = true) 
	{
		if (editor_object.GetFlags() & EditorObjectFlags.NODELETE) {
			return false;
		}
		
		if (editor_object.IsLocked()) {
			return false;
		}
		
		if (!editor_object.IsVisible()) {
			return false;
		}
		
		if (editor_object.IsSelected()) {
			DeselectObject(editor_object);
		}
		
        if (create_undo) {			
			EditorObjectDeletionSnapshot snapshot = new EditorObjectDeletionSnapshot(editor_object.GetController());
			InsertSnapshot(snapshot);
        }
        
		m_ObjectManager.DeleteObject(editor_object);
		
		if (GetGame().IsMultiplayer() && send_net_message) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(1);
			rpc.Write(editor_object.Uuid);
			rpc.Send(null, 39253, true);
			m_EditorObjectsByUuid.Remove(editor_object.Uuid);
		}
		
		return true;
	}

	bool DeleteObjectByUuid(string uuid, bool create_undo = true) 
	{
		if (m_EditorObjectsByUuid[uuid]) {
			return DeleteObject(m_EditorObjectsByUuid[uuid], create_undo, false);
		}
				
		return false;
	}
	
	int DeleteObjectsByUuid(notnull array<string> uuids, bool create_undo = true)
	{
		array<EditorObject> editor_objects = {};
		foreach (string uuid: uuids) {
			if (m_EditorObjectsByUuid[uuid]) {
				editor_objects.Insert(m_EditorObjectsByUuid[uuid]);
			}
		}
		
		return DeleteObjects(editor_objects, create_undo, false);
	}
	
	// If you want to directly delete without undo, get the object manager and do it there
	int DeleteObjects(notnull array<EditorObject> editor_objects, bool create_undo = true, bool send_net_message = true)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(editor_objects.Count());
		
		if (create_undo) {
			array<ref EditorSnapshot> snapshots = {};
			foreach (EditorObject deleted_object_snap: editor_objects) {
				EditorObjectDeletionSnapshot snapshot = new EditorObjectDeletionSnapshot(deleted_object_snap.GetController());
				snapshots.Insert(snapshot);
			}
			
			InsertSnapshots(snapshots);
		}
		
		int count;
		foreach (EditorObject editor_object: editor_objects) {
			if (!editor_object.IsLocked()) {
				if (editor_object.IsSelected()) {
					DeselectObject(editor_object);
				}
			
				m_ObjectManager.DeleteObject(editor_object);
				count++;
				
				if (m_EditorObjectsByUuid.Contains(editor_object.Uuid)) {
					rpc.Write(editor_object.Uuid);
					m_EditorObjectsByUuid.Remove(editor_object.Uuid);
				}
			}
		}
		
		if (GetGame().IsMultiplayer() && send_net_message) {
			rpc.Send(null, 39253, true);
		}
		
		return count;
	}
		
	int DeleteObjects(EditorObjectMap editor_object_map, bool create_undo = true, bool send_net_message = true)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(editor_object_map.Count());
		
		if (create_undo) {
			array<ref EditorSnapshot> snapshots = {};
			foreach (int id2, EditorObject deleted_object_snap: editor_object_map) {
				EditorObjectDeletionSnapshot snapshot = new EditorObjectDeletionSnapshot(deleted_object_snap.GetController());
				snapshots.Insert(snapshot);
			}
			
			InsertSnapshots(snapshots);
		}
		
		int count;
		foreach (int id, EditorObject editor_object: editor_object_map) {			
			if (!editor_object.IsLocked() && editor_object.IsVisible()) {
				if (editor_object.IsSelected()) {
					DeselectObject(editor_object);
				}
				
				m_ObjectManager.DeleteObject(editor_object);
				count++;
				
				if (m_EditorObjectsByUuid.Contains(editor_object.Uuid)) {
					rpc.Write(editor_object.Uuid);
					m_EditorObjectsByUuid.Remove(editor_object.Uuid);
				}
			}
		}
		
		if (GetGame().IsMultiplayer() && send_net_message) {
			rpc.Send(null, 39253, true);
		}
		
		return count;
	}
	
	void UpdateObjectByUuid(string uuid, EditorObjectData data)
	{
		auto object = m_EditorObjectsByUuid[uuid];
		if (!object) {
			return;
		}
		
		object.SetPosition(data.Position);
		object.SetOrientation(data.Orientation);
		object.SetScale(data.Scale);
		object.Lock(data.Locked);
		object.Update();
	}

	bool HideMapObject(string type, vector position, bool create_undo = true)
	{
		return HideMapObject(new EditorDeletedObject(EditorDeletedObjectData.Create(type, position)), create_undo);
	}
	
	bool HideMapObject(notnull Object object, bool create_undo = true)
	{
		return HideMapObject(new EditorDeletedObject(EditorDeletedObjectData.Create(object)), create_undo);
	}
		
	bool HideMapObject(notnull EditorDeletedObjectData deleted_object_data, bool create_undo = true)
	{
		return HideMapObject(new EditorDeletedObject(deleted_object_data), create_undo);
	}
	
	bool HideMapObject(notnull EditorDeletedObject map_object, bool create_undo = true, bool send_net_message = true)
	{
		string uuid = UUID.Generate();
		if (GetGame().IsMultiplayer() && send_net_message) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(1);
			rpc.Write(uuid);
			rpc.Write(map_object.GetWorldObject());
			rpc.Send(null, 39255, true);
		}
		
		GetStatistics().EditorRemovedObjects--;
		
		return HideMapObject(uuid, map_object, create_undo);
	}
	
	bool HideMapObject(string uuid, notnull EditorDeletedObject map_object, bool create_undo = true)
	{
		if (!CanHideMapObject(map_object.GetType())) {
			return false;
		}
		
		if (!map_object || !map_object.GetWorldObject()) {
			return false;
		}
		
		m_DeletedSessionCache[map_object.GetID()] = map_object.GetData();
		
		if (m_ObjectManager.IsObjectHidden(map_object)) { 
			return false;
		}
		
		if (create_undo) {
			EditorDeletedObjectSnapshot snapshot = new EditorDeletedObjectSnapshot(map_object.GetID());
			InsertSnapshot(snapshot);
		}
		
		GetStatistics().EditorRemovedObjects++;
		
		m_ObjectManager.HideMapObject(map_object);
		m_HiddenObjectsByUuid[uuid] = map_object;
		return true;
	}
		
	void HideMapObjects(array<Object> deleted_objects, bool create_undo = true, bool send_net_message = true)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(deleted_objects.Count());
		
		map<string, Object> deleted_object_map = new map<string, Object>();
		foreach (Object deleted_object: deleted_objects) {
			string uuid = UUID.Generate();
			rpc.Write(uuid);
			rpc.Write(deleted_object);
			
			deleted_object_map[uuid] = deleted_object;
		}
		
		if (GetGame().IsMultiplayer() && send_net_message) {
			rpc.Send(null, 39255, true);
		}
		
		HideMapObjectsByUuid(deleted_object_map, create_undo);
	}
	
	void HideMapObjectsByUuid(notnull map<string, Object> deleted_object_map, bool create_undo = true)
	{
		EditorAction action = new EditorAction("Unhide", "Hide");
		
		array<ref EditorSnapshot> deleted_object_snapshots = {};
		foreach (string uuid, Object object: deleted_object_map) {
			if (!object) {
				continue;
			}
			
			if (!CanHideMapObject(object.GetType())) {
				continue;
			}
			
			if (m_ObjectManager.IsObjectHidden(object)) {
				continue;
			}
			
			EditorDeletedObjectData deleted_object_data = EditorDeletedObjectData.Create(object);
			m_DeletedSessionCache[deleted_object_data.ID] = deleted_object_data;		
			
			deleted_object_snapshots.Insert(new EditorDeletedObjectSnapshot(deleted_object_data.GetID()));
			
			EditorDeletedObject deleted_object = new EditorDeletedObject(deleted_object_data);
			deleted_object.Uuid = uuid;
			m_ObjectManager.HideMapObject(deleted_object);
			m_HiddenObjectsByUuid[uuid] = deleted_object;
		}
		
		if (create_undo) {
			InsertSnapshots(deleted_object_snapshots);
		}
	}
		
	bool UnhideMapObject(EditorDeletedObjectData data, bool create_undo = true)
	{		
		if (!data || !data.WorldObject) {
			return false;
		}
		
		if (!m_ObjectManager.IsObjectHidden(data)) { 
			return false;
		}
		
		if (create_undo) {
			EditorUnDeleteObjectSnapshot snapshot = new EditorUnDeleteObjectSnapshot(data.GetID());
			InsertSnapshot(snapshot);
		}
		
		m_ObjectManager.UnhideMapObject(data.ID);
		return true;
	}
		
	bool UnhideMapObject(EditorDeletedObject map_object, bool create_undo = true, bool send_net_message = true)
	{
		if (!map_object) {  
			return false;
		}
		
		if (!m_ObjectManager.IsObjectHidden(map_object)) { 
			return false;
		}
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(1);
						
		rpc.Write(map_object.Uuid);
		m_HiddenObjectsByUuid.Remove(map_object.Uuid);
		
		m_ObjectManager.UnhideMapObject(map_object);

		if (create_undo) {
			EditorUnDeleteObjectSnapshot snapshot = new EditorUnDeleteObjectSnapshot(map_object.GetID());
			InsertSnapshot(snapshot);
		}
		
		if (GetGame().IsMultiplayer() && send_net_message) {
			rpc.Send(null, 39256, true);
		}
		
		return true;
	}
	
	bool UnhideMapObjectByUuid(string uuid, bool create_undo = true)
	{
		GetStatistics().EditorRemovedObjects--;
		if (m_HiddenObjectsByUuid.Contains(uuid)) {
			return UnhideMapObject(m_HiddenObjectsByUuid[uuid], create_undo, false);
		}
		
		return false;
	}
		
	void UnhideMapObjects(EditorDeletedObjectMap deleted_objects, bool create_undo = true, bool send_net_message = true)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(deleted_objects.Count());
					
		array<ref EditorSnapshot> snapshots = {};
		foreach (int id, EditorDeletedObject deleted_object: deleted_objects) {									
			GetStatistics().EditorRemovedObjects++;
			m_ObjectManager.UnhideMapObject(id);

			rpc.Write(deleted_object.Uuid);
			
			snapshots.Insert(new EditorUnDeleteObjectSnapshot(deleted_object.GetID()));
			
			m_HiddenObjectsByUuid.Remove(deleted_object.Uuid);
		}
		
		if (create_undo) {
			InsertSnapshots(snapshots);
		}

		if (GetGame().IsMultiplayer() && send_net_message) {
			rpc.Send(null, 39256, true);
		}
	}

	void UnhideMapObjectsByUuid(notnull array<string> deleted_object_uuids, bool create_undo = true)
	{
		EditorDeletedObjectMap deleted_objects = new EditorDeletedObjectMap();
		foreach (string deleted_object_uuid: deleted_object_uuids) {
			if (m_HiddenObjectsByUuid.Contains(deleted_object_uuid)) {
				EditorDeletedObject hidden_object = m_HiddenObjectsByUuid[deleted_object_uuid];
				deleted_objects[hidden_object.GetID()] = hidden_object;
			}
		}
		
		GetStatistics().EditorRemovedObjects -= deleted_object_uuids.Count();

		UnhideMapObjects(deleted_objects, create_undo, false);
	}
		
	void Clear()
	{
		GetStatistics().Save();
		EditorSaveFile = string.Empty;	
		m_EditorHud.GetTemplateController().NotifyPropertyChanged("m_Editor.EditorSaveFile");
		m_ActionStack.Clear();
		m_Snapshots.Clear();
		m_SnapshotIndex = 0;
		m_SessionCache.Clear();
		m_ObjectManager.Clear();
		m_CurrentFileAuthorCredits.Clear();
		m_CurrentFileCreationDate = 0;
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
		editor_object.CreateSnapshot();
		
		editor_object.Lock(true);
		DeselectObject(editor_object);
	}

	void LockObjects(EditorObjectMap editor_object_map, bool create_undo = true)
	{
		CreateCheckpoint(editor_object_map);
		foreach (int id, EditorObject editor_object: editor_object_map) {
			if (editor_object && !editor_object.IsLocked()) {
				editor_object.Lock(true);
				DeselectObject(editor_object);
			}
		}
	}
	
	void UnlockObject(EditorObject editor_object)
	{
		editor_object.CreateSnapshot();
		editor_object.Lock(false);
	}

	void UnlockObjects(EditorObjectMap editor_object_map, bool create_undo = true)
	{
		CreateCheckpoint(editor_object_map);
		foreach (int id, EditorObject editor_object: editor_object_map) {
			if (editor_object && editor_object.IsLocked()) {
				editor_object.Lock(false);
			}
		}
	}
			
	static PlayerBase CreateDefaultCharacter(string type, vector position, PlayerIdentity identity = null)
	{
		EditorLog.Trace("Editor::CreateDefaultCharacter");
		if (GetGame().GetPlayer()) {
			return PlayerBase.Cast(GetGame().GetPlayer());
		} 
	
		PlayerBase player = PlayerBase.Cast(GetGame().CreatePlayer(identity, type, position, 0, string.Empty));
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
	
	static vector GetSafeStartPosition(float x, float z, float radius)
	{
		vector position;
		position[0] = Math.RandomFloat(x - radius, x + radius);
		position[2] = Math.RandomFloat(z - radius, z + radius);
		position[1] = GetGame().SurfaceY(position[0], position[2]);
		
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
			
	void LoadSaveData(EditorSaveData save_data, bool clear_before = false)
	{
		if (!save_data) {
			EditorLog.Error("Invalid Save Data");
			return;
		}
		
		int created_objects, deleted_objects;
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {			
			EditorLog.Warning("Different map detected");
			/*if (MessageBox.Show("Different Map Detected", string.Format("Switch map to %1?"), MessageBoxButtons.OKCancel) != DialogResult.OK) {
				return null;
			}
			
			EditorLog.Info("Loading Map %1", save_data.MapName);
			g_Game.ReportProgress(string.Format("Loading Map %1", save_data.MapName));
			
			GetGame().PlayMission(CreateEditorMissionFolder(save_data.MapName));
			
			m_Editor = GetEditor();
			*/
		}
		
		if (clear_before) {
			Clear();
			
			if (save_data.CameraPosition != vector.Zero) {
				GetCamera().SetPosition(save_data.CameraPosition);
			}
			
			m_CurrentFileCreationDate = save_data.CreationDate;
			m_CurrentFileAuthorCredits.Copy(save_data.CreditIds);
			
			string uid = GetGame().GetUserManager().GetTitleInitiator().GetUid();
			if (m_CurrentFileAuthorCredits.Find(uid) == -1) {
				m_CurrentFileAuthorCredits.Insert(uid);
			}
		}
				
		EditorLog.Debug("Deleting %1 Objects", save_data.EditorHiddenObjects.Count().ToString());		
		foreach (EditorDeletedObjectData id: save_data.EditorHiddenObjects) {
			if (HideMapObject(id, false)) {
				deleted_objects++;
			}
		}
		
		EditorLog.Debug("Creating %1 Objects", save_data.EditorObjects.Count().ToString());
		foreach (EditorObjectData data: save_data.EditorObjects) {
			// So here I am thinking... I used to do int.MAX coverage when placing stuff. i BET they are serialized this way. until we go through every editor file ever, this will have to do.
			if (data.Flags == int.MAX) {
				data.Flags = EFE_DEFAULT;
			}

			EditorObject created_object = CreateObject(data, false);
			if (created_object) {
				created_objects++;
				created_object.HideBoundingBox(); // bugfix with new bounding boxes
			}			
		}
		
		foreach (EditorCameraTrackData track_data: save_data.CameraTracks) {
			AddCameraTrack(track_data, false);
		}
				
		// Dirt fix, it needs Transactional Import
		if (GetGame().IsMultiplayer())
		{
			m_EditorHud.CreateNotification(string.Format("Sent request to load %1 objects to the server.", save_data.EditorObjects.Count()));
		}
		else
		{
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
				m_EditorHud.CreateNotification(error_message);
			
				// Disable auto save since we loaded a shit file
				GetSettings().AutoSaveTimer = -1;
			} else {
				m_EditorHud.CreateNotification(string.Format("Loaded %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count()));
			}
		}
	}
	
	EditorSaveData CreateSaveData(bool selected_only = false)
	{	
		string uid = GetGame().GetUserManager().GetTitleInitiator().GetUid();
		
		EditorSaveData save_data = new EditorSaveData();
		save_data.AuthorId = uid;
		save_data.CreditIds.Copy(m_CurrentFileAuthorCredits);
		save_data.LastModified = DateTime.Now();
		
		if (m_CurrentFileCreationDate == 0) {
			m_CurrentFileCreationDate = DateTime.Now();	
		}
		
		save_data.CreationDate = m_CurrentFileCreationDate;
		
		// Save world name
		save_data.MapName = GetGame().GetWorldName();
		
		// Save Camera Position
		save_data.CameraPosition = GetCamera().GetPosition();
		
		// add last modified
		save_data.LastModified = DateTime.Now();
		
		// add credits
		if (save_data.CreditIds.Find(uid) == -1) {
			save_data.CreditIds.Insert(uid);
		}

		// Save Objects
		EditorObjectMap placed_objects = GetPlacedObjects();
		if (selected_only) {
			placed_objects = GetSelectedObjects();
		}
		
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {				
				if (editor_object.GetType() != string.Empty && !(editor_object.GetFlags() & EditorObjectFlags.NOSAVE) && editor_object.GetWorldObject()) {					
					save_data.EditorObjects.Insert(editor_object.GetData());
				}
			}
		}
		
		EditorDeletedObjectMap deleted_objects = GetObjectManager().GetDeletedObjects();
		foreach (int id, EditorDeletedObject deleted_object: deleted_objects) {
			if (deleted_object.GetWorldObject()) {
				save_data.EditorHiddenObjects.Insert(deleted_object.GetData());
			}
		}
		
		array<EditorCameraTrack> camera_tracks = GetObjectManager().GetCameraTracks();
		foreach (EditorCameraTrack track: camera_tracks) {
			save_data.CameraTracks.Insert(EditorCameraTrackData.Cast(track.GetData()));
		}
		
		return save_data;
	}
	
	static string GetModelName(notnull Object object)
	{
		string debug_name = object.GetDebugNameNative();
		if (debug_name == string.Empty) {
			// lost cause, unlikely
			return string.Empty;
		}
		
		array<string> split_string = {};
		debug_name.Split(":", split_string);
		
		// also unlikely
		if (split_string.Count() == 1) {
			return string.Empty;
		}
		
		return split_string[1].Trim();
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
			return GetPlaceableObject(object.GetType());
		}
		
		// 1346854: tank_small_white.p3d
		string model_name = GetModelName(object);
		
		array<EditorPlaceableItem> placeable_items = m_ObjectManager.GetReplaceableObjects(model_name);
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {			
			return null;
		}
		
		return placeable_items[0]; // better way to do other than index 0?
	}
	
	string GetObjectName(Object object, int component_index)
	{		
		if (!object) {
			return string.Empty;
		}
		
		while (object.GetParent()) {
			object = Object.Cast(object.GetParent());
		}
		
		string component_type = "component";
		Building building = Building.Cast(object);
		if (building) {
			if (building.GetDoorIndex(component_index) != -1) {
				component_index = building.GetDoorIndex(component_index);
				component_type = "door";
			}
		}
		
		if (object.GetType() != string.Empty && !object.IsTree() && !object.IsBush() && !object.IsRock()) {			
			return string.Format("%1 [%2, %3: %4]", object.GetType(), object.GetID(), component_type, component_index);
		}
		
		// 1346854: tank_small_white.p3d
		string debug_name = object.GetDebugNameNative();
		if (debug_name == string.Empty) {
			// lost cause, unlikely
			return string.Empty;
		}
		
		array<string> split_string = {};
		debug_name.Split(":", split_string);
		
		// also unlikely
		if (split_string.Count() == 1) {
			return string.Empty;
		}
		
		array<EditorPlaceableItem> placeable_items = m_ObjectManager.GetReplaceableObjects(split_string[1].Trim());
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {
			return string.Format("%1 [%2, %3: %4]", split_string[1], split_string[0], component_type, component_index);
		}
				
		return string.Format("%1 [%2, %3: %4]", placeable_items[0].Type, split_string[0], component_type, component_index);
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
		
	void ShowDonationDialog()
	{		
		GetEditorHud().CurrentDialog = new EditorOneTimeDonationDialog("Editor Changelog");
	}
		
	string GetSaveFile()
	{
		return EditorSaveFile;
	}
	
	void DeleteSessionData(int id) 
	{
		m_SessionCache.Remove(id);	
	}
	
	void DeleteDeletedSessionData(int id)
	{
		m_DeletedSessionCache.Remove(id);
	}
	
	void SelectObject(notnull EditorObject target) 
	{
		m_ObjectManager.SelectObject(target);
	}
	
	void SelectObjects(notnull EditorObjectMap editor_objects)
	{
		foreach (int id, EditorObject editor_object: editor_objects) {
			SelectObject(editor_object);
		}
	}
	
	void DeselectObject(notnull EditorObject target) 
	{
		m_ObjectManager.DeselectObject(target);
	}
	
	void ToggleSelection(notnull EditorObject target) 
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

	bool IsRunningCameraTrack()
	{
		return m_CameraTrackState;
	}

	void StopCameraTrack()
	{
		m_CameraTrackState = 0;
		m_CameraTrackIndex = 0;
		m_CameraTrackLerpNorm = 0;

		m_EditorCamera.SetTransform(m_CameraTransformPreTrackMotion);
	}

	void StartCameraTrack()
	{
		m_CameraTrackState = 1;

		vector mat[4];

		m_EditorCamera.GetTransform(mat);
		copyarray(m_CameraTransformPreTrackMotion, mat);
		GetStatistics().EditorCameraTracksRidden++;
	}

	void PauseCameraTrack()
	{
		m_CameraTrackState = 0;
	}
	
	void CameraTrackNext()
	{
		array<EditorCameraTrack> tracks = GetObjectManager().GetCameraTracks();
		if (tracks.Count() == 0) {
			return;
		}

		m_CameraTrackIndex = Math.Rollover(m_CameraTrackIndex + 1, 0, tracks.Count());
		m_CameraTrackLerpNorm = 0;
		
		vector mat[4];
		tracks[m_CameraTrackIndex].GetTransform(mat);
		m_EditorCamera.SetTransform(mat);
	}
	
	void CameraTrackPrevious()
	{
		array<EditorCameraTrack> tracks = GetObjectManager().GetCameraTracks();
		if (tracks.Count() == 0) {
			return;
		}

		m_CameraTrackIndex = Math.Rollover(m_CameraTrackIndex - 1, 0, tracks.Count());
		m_CameraTrackLerpNorm = 0;
		
		vector mat[4];
		tracks[m_CameraTrackIndex].GetTransform(mat);
		m_EditorCamera.SetTransform(mat);
	}
	
	void AddCameraTrack(notnull EditorCamera camera, float time, EditorObjectFlags flags = EFE_DEFAULT, bool create_undo = true)
	{
		AddCameraTrack(EditorCameraTrackData.Create(camera, time, flags), create_undo);
	}
	
	EditorCameraTrack AddCameraTrack(notnull EditorCameraTrackData camera_track_data, bool create_undo = true)
	{
		// Cache Data (for undo / redo)
		m_SessionCache[camera_track_data.GetID()] = camera_track_data;
		
		// Create Object
		EditorCameraTrack camera_track = m_ObjectManager.CreateCameraTrack(camera_track_data);		
		EditorAction action = new EditorAction("Delete", "Create");
		action.InsertUndoParameter(new Param1<int>(camera_track.GetID()));
		action.InsertRedoParameter(new Param1<EditorObjectData>(camera_track_data));
		
		if (create_undo) {
			//InsertAction(action);
		}

		return camera_track;
	}
		
	void DeleteCameraTrack(EditorCameraTrack camera_track, bool create_undo = true)
	{
		EditorAction action = new EditorAction("Create", "Delete");
		action.InsertUndoParameter(new Param2<int, vector>(camera_track.GetID(), camera_track.GetPosition()));
		action.InsertRedoParameter(new Param1<int>(camera_track.GetID()));

		if (!m_ObjectManager.DeleteCameraTrack(camera_track)) {
			return;
		}

		if (create_undo) {
			//InsertAction(action);
		}
	}
	
	void DeleteCameraTracks(array<EditorCameraTrack> camera_tracks, bool create_undo = true)
	{
		EditorAction action = new EditorAction("Create", "Delete");
		foreach (auto camera_track: camera_tracks) {
			action.InsertUndoParameter(new Param2<int, vector>(camera_track.GetID(), camera_track.GetPosition()));
			action.InsertRedoParameter(new Param1<int>(camera_track.GetID()));

			m_ObjectManager.DeleteCameraTrack(camera_track);
		}

		if (create_undo) {
			//InsertAction(action);
		}
	}
		
	vector GetAveragePositionOfSelection()
	{
		return m_ObjectManager.GetAveragePositionOfSelection();
	}
			
	array<string> GetRecentFiles()
	{
		return m_RecentlyOpenedFiles;
	}
	
	bool IsActive() 
	{
		return m_Active;
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
	
	EditorObjectManagerModule GetObjectManager() 
	{
		return m_ObjectManager;
	}
		
	EditorObjectMap GetSelectedObjects() 
	{
		return m_ObjectManager.GetSelectedObjects(); 
	}
	
	array<EditorObject> GetSelectedObjectsOrdered()
	{
		return m_ObjectManager.GetSelectedObjectsOrdered();
	}
	
	EditorDeletedObjectMap GetSelectedHiddenObjects()  
	{
		return m_ObjectManager.GetSelectedHiddenObjects();
	}
	
	EditorObjectMap GetPlacedObjects() 
	{
		return m_ObjectManager.GetPlacedObjects(); 
	}
	
	EditorDeletedObjectMap GetDeletedObjects() 
	{
		return m_ObjectManager.GetDeletedObjects();
	}
	
	map<int, ref EditorObjectData> GetSessionCache() 
	{
		return m_SessionCache; 		
	}
	
	map<int, ref EditorDeletedObjectData> GetDeletedSessionCache() 
	{
		return m_DeletedSessionCache;
	}
	
	EditorObject GetEditorObject(int id) 
	{
		return m_ObjectManager.GetEditorObject(id); 	
	}
	
	EditorObject GetEditorObjectByUuid(string uuid)
	{
		return m_EditorObjectsByUuid.Get(uuid);
	}
	
	EditorObject GetEditorObject(notnull Object world_object) 
	{
		return m_ObjectManager.GetEditorObject(world_object);	
	}
	
	EditorObject GetPlacedObjectById(int id) 
	{
		return m_ObjectManager.GetPlacedObjectById(id); 	
	}
	
	EditorObjectData GetSessionDataById(int id) 
	{
		return m_SessionCache[id]; 
	}
	
	EditorDeletedObjectData GetDeletedSessionDataById(int id) 
	{
		return m_DeletedSessionCache[id];
	}
	
	EditorPlaceableItem GetPlaceableObject(string type)
	{
		return m_ObjectManager.GetPlaceableObject(type);
	}
	
	array<ref EditorPlaceableItem> GetPlaceableObjects() 
	{
		return m_ObjectManager.GetPlaceableObjects();
	}
	
	/*
	void SetPlacingObject(EditorWorldObject object)
	{
		m_PlacingObject = object;
	}
	
	EditorWorldObject GetPlacingObject()
	{
		return m_PlacingObject;	
	}*/
	
	bool IsPlacing()
	{
		return (m_PlacingObjects && m_PlacingObjects.Count() > 0); 
	}
	
	bool IsPlayerActive()
	{
		return (m_Player && m_Player.IsControlledPlayer() && !m_Active);
	}
	
	bool IsDragging()
	{
		if (m_CurrentGizmo && m_CurrentGizmo.IsInteracting()) {
			return true;
		}
		
		return DragHandler != null && DragHandler.IsDragging();
	}
		
	// Get Selected player in Editor
	PlayerBase GetPlayer()
	{
		return m_Player;
	}
	
	EditorSettings GetSettings()
	{
		return EditorSettings.Cast(g_Game.GetProfileSetting(EditorSettings));
	}
	
	EditorStatistics GetStatistics()
	{
		return EditorStatistics.Cast(g_Game.GetProfileSetting(EditorStatistics));
	}

	EditorCameraSettings GetCameraSettings()
	{
		return EditorCameraSettings.Cast(g_Game.GetProfileSetting(EditorCameraSettings));
	}
	
	EditorWebApi GetWebApi()
	{
		return m_RestApi;
	}
	
	Object GetObjectUnderCursor()
	{
		return m_ObjectUnderCursor;
	}

	int GetComponentIndexUnderCursor()
	{
		return m_ComponentIndexUnderCursor;
	}
	
	EditorGizmo GetGizmo()
	{
		return m_CurrentGizmo;
	}
	
	float GetGridSize()
	{
		if (!GridMode) {
			return 0;
		}
		
		if (KeyState(KeyCode.KC_LMENU)) {
			return 3.0;
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			return 0.1;
		}
		
		return 1;
	}
}