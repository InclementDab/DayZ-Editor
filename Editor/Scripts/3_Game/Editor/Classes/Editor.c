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

class EditorSounds: Managed
{
	static const string HIGHLIGHT = "Highlight_SoundSet";
	static const string PLOP = "Plop_SoundSet";
	static const string PLOPLOW = "PlopLow_SoundSet";
}


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

	EditorStatistics Statistics = EditorStatistics.Cast(GetDayZGame().GetProfileSetting(EditorStatistics));
	
	// protected Editor Members
	protected ref EditorHud	m_EditorHud;
	protected ref EditorBrush m_EditorBrush;

	protected EditorCamera m_EditorCamera;
	ref map<Object, ref EditorHandData> Placing = new map<Object, ref EditorHandData>();
	protected Entity m_CurrentControl;
	
	// Stack of Undo / Redo Actions
	protected ref EditorActionStack m_ActionStack = new EditorActionStack();
		
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

	static const string 						Version = "2.0";
	
	protected ref TStringArray					m_RecentlyOpenedFiles = {};
		
	protected ref Timer	m_StatisticsSaveTimer 	= new Timer(CALL_CATEGORY_GAMEPLAY);
	protected ref Timer	m_AutoSaveTimer			= new Timer(CALL_CATEGORY_GAMEPLAY);
	
	protected ScriptCaller 						m_ObjectSelectCallback;
				
	// Stored list of all Placed Objects, indexed by their WorldObject ID
	protected ref map<int, EditorObject> m_WorldObjectIndex = new map<int, EditorObject>();
	
	protected ref array<ref EditorHiddenObject> m_DeletedObjects = {};
		
	// Current Selected PlaceableListItem
	EditorPlaceableObjectData CurrentSelectedItem;
	
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
			
	
	protected ref map<typename, ref Command> m_Commands = new map<typename, ref Command>();
	protected ref map<string, Command> m_CommandShortcutMap = new map<string, Command>();
	
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
					
		// Init Hud
		g_Game.ReportProgress("Initializing Hud");
		m_EditorHud 		= new EditorHud();
		
		m_Mission = GetGame().GetMission();
		
		GetGame().GetProfileStringList("EditorRecentFiles", m_RecentlyOpenedFiles);
				
		// this is terrible but it didnt work in OnMissionLoaded so im forced to reckon with my demons
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PPEffects.ResetAll, 1000);
		
		ControlCamera(m_EditorCamera);
		
		foreach (typename command_type: RegisterCommand.Instances) {		
			Command command = Command.Cast(command_type.Spawn());
			if (!command) {
				Error("Invalid command");
				continue;
			}
			
			m_Commands[command_type] = command;
			
			if (command.GetShortcut() != string.Empty) {
				m_CommandShortcutMap[command.GetShortcut()] = command;
			}
		}
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(Update);
		
		// TODO!!! write a better autosave!!!
		//m_AutoSaveTimer.Run(GeneralSettings.AutoSaveTimer, this, "OnAutoSaveTimer");
	}

	void ~Editor() 
	{
		Statistics.Save();

		GetGame().ObjectDelete(m_EditorCamera);
	}
		
	protected Object m_DragTarget;
	protected vector m_DragOffset;
	protected void CheckForDragging(Object object)
	{
		if ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) == MB_PRESSED_MASK) {
			m_DragTarget = object;
			
			vector position;	
			vector end_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * 3000;
			int interaction_layers = PhxInteractionLayers.BUILDING | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ITEM_LARGE;
			Object hit_object;
			vector normal;
			float fraction;
			DayZPhysics.RayCastBullet(GetGame().GetCurrentCameraPosition(), end_pos, interaction_layers, null, hit_object, position, normal, fraction);
			
			vector transform[4];
			m_DragTarget.GetTransform(transform);
			
			m_DragOffset = position.InvMultiply4(transform);
		}
	}
		
	void Update(float timeslice)
	{		
		if (IsProcessingCommand()) {
			foreach (string input_name, Command command: m_CommandShortcutMap) {		
				if (GetFocus() && GetFocus().IsInherited(EditBoxWidget)) {
					continue;
				}
						
				if (!command || !command.CanExecute()) {
					continue;
				}
				
				switch (command.GetShortcutType()) {
					case ShortcutKeyType.PRESS: {
						if (GetGame().GetInput().LocalPress(input_name)) {
							command.Execute(true);
						}
						
						break;
					}
					
					case ShortcutKeyType.DOUBLE: {
						if (GetGame().GetInput().LocalDbl(input_name)) {
							command.Execute(true);
						}
						
						break;
					}
					
					case ShortcutKeyType.HOLD: {
						if (GetGame().GetInput().LocalHold(input_name)) {
							command.Execute(true);
						}
						
						break;
					}
				}
			}
		}
			
		if ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) != MB_PRESSED_MASK) {
			m_DragTarget = null;
		}
		
		if (m_DragTarget) {
			EditorObjectDragHandler.Drag(m_DragTarget, m_DragOffset);
		}
		
		Ray ray = GetCamera().PerformCursorRaycast();
		ray.Debug();
		
		foreach (Object object, EditorHandData data1: Placing) {
			vector transform[4];
			object.GetTransform(transform);
						
			transform = { "1 0 0", ray.Direction, "1 0 0" * ray.Direction, ray.Position };
			
			object.SetTransform(transform);
			//output_ray.Debug();
		}
		
		if (GetGame().GetInput().LocalPress_ID(UAFire) && GetWidgetUnderCursor() && GetWidgetUnderCursor().GetName() != "Panel") {
			foreach (Object object_to_place, EditorHandData data: Placing) {					
				string uuid = UUID.Generate();				
				m_PlacedObjects[uuid] = new EditorObject(uuid, object_to_place, object_to_place.GetType(), EFE_DEFAULT);
				
				m_WorldObjectIndex[m_PlacedObjects[uuid].GetWorldObject().GetID()] = m_PlacedObjects[uuid];			
				
				OnObjectCreated.Invoke(m_PlacedObjects[uuid]);
				
				Placing.Remove(object_to_place);
			}
			
			PlaySound(EditorSounds.PLOP);
			
			if (m_CurrentOnlineSession) {
				m_CurrentOnlineSession.SetSynchDirty();
			}
		}
	}
		
	// EditorSounds is helpful
	void PlaySound(string sound_set)
	{
		SEffectManager.PlaySoundOnObject(sound_set, m_EditorCamera);
	}
				
	void PromptForObjectSelection(ScriptCaller callback)
	{
		m_ObjectSelectCallback = callback;
		
		if (m_ObjectSelectCallback) {
			m_EditorHud.ShowNotification("Select a world object");
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
	
	//! DayZ doesnt support 3 deep keybinding. LCTRL + SHIFT + S etc... 
	// so all commands must be appended with ctrl - this is standard windows expectation anyway
	bool IsProcessingCommand()
	{
		return (GetDayZGame().IsLeftCtrlDown());
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
		
	bool IsSurfaceWater(vector position)
	{
		return GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]);
	}
		
	EditorHud ReloadHud() 
	{	
		m_EditorHud = new EditorHud();
		return m_EditorHud;
	}
		
	void Undo()
	{
		foreach (EditorAction action: m_ActionStack) {
			if (!action.IsUndone()) {
				action.CallUndo();
				return;
			}
		}
	}
	
	void Redo()
	{
		for (int i = m_ActionStack.Count() - 1; i >= 0; i--) {
			if (m_ActionStack[i] && m_ActionStack[i].IsUndone()) {
				m_ActionStack[i].CallRedo();
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

	void DeleteObject(notnull EditorObject editor_object, bool create_undo = true) 
	{			
		OnObjectDeleted.Invoke(editor_object);
		delete editor_object;
		
		PlaySound(EditorSounds.PLOPLOW);
	}
	
	void DeleteObjects(notnull array<EditorObject> editor_objects, bool create_undo = true)
	{
		foreach (EditorObject editor_object: editor_objects) {		
			OnObjectDeleted.Invoke(editor_object);
		
			m_PlacedObjects.Remove(editor_object.GetUUID());
			delete editor_object;
		}
		
		PlaySound(EditorSounds.PLOPLOW);
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
			//if (CreateObject(data.CreateObject(), false)) {
			//	created_objects++;
			//}			
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
			m_EditorHud.ShowNotification(error_message, COLOR_YELLOW);
			
			// Disable auto save since we loaded a shit file
			//Settings.AutoSaveTimer = -1;
		} else {
			m_EditorHud.ShowNotification(string.Format("Loaded %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count()), COLOR_GREEN);
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
	
	static EditorPlaceableObjectData GetReplaceableItem(notnull Object object)
	{		
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
		
		array<EditorPlaceableObjectData> placeable_items = GetDayZGame().GetReplaceableObjects(split_string[1].Trim());
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {			
			return null;
		}
		
		return placeable_items[0]; // better way to do other than index 0?
	}
				
	void InsertAction(EditorAction action) 
	{
		m_ActionStack.InsertAction(action);
	}
	
	array<string> GetRecentFiles()
	{
		return m_RecentlyOpenedFiles;
	}
		
	EditorHud GetHud() 
	{
		return m_EditorHud;
	}
		
	EditorCamera GetCamera() 
	{
		return m_EditorCamera;
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
	
	map<string, Command> GetCommandShortcutMap()
	{
		return m_CommandShortcutMap;
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
		return Placing.Count() > 0; 
	}
	
	EditorProfileSettings GetProfileSettings()
	{
		return EditorProfileSettings.Cast(GetDayZGame().GetProfileSetting(EditorProfileSettings));
	}
	
	Command GetCommand(typename command)
	{
		return m_Commands[command];
	}
}