/*
                    Lloyd sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/

[RegisterCommand(DeleteCommand)]
class DeleteCommand: Command
{
	override void Execute(bool state) 
	{
		Print("Cringe"); 
		foreach (EditorNode node: EditorNode.SelectedObjects) {
			Print(node);	
			delete node;
		}
	}
}

class EditorHandData
{
	vector PositionOffset;
	vector OrientationOffset;
}			

class Editor: SerializableBase
{
	// Handled in DayZGame
	static const int RPC_SYNC = 54365;
	
	static const ref array<int> RPC_ALL = { RPC_SYNC, EditorNode.RPC_SYNC };
	
	static const int DEFAULT_ENTITY_COUNT = 512;
	
	// m_Editor is only valid on the local instance.
	protected Man m_Player;
	
	// All editors in lobby, including m_Editor
	protected ref array<PlayerIdentity> m_Editors = {};
	
	protected ref map<PlayerIdentity, Object> m_CameraObjects = new map<PlayerIdentity, Object>();
	
	bool Public;
	protected string m_Password; // must be SHA before reaching clients
	protected int m_MaxPlayers = 32;
	protected int m_MaxEntityCount = 30;
	
	ref ScriptInvoker OnSyncRecieved = new ScriptInvoker();
	
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

	protected EditorCamera m_Camera;
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
	
	static const string Version = "2.0";
	
	protected ScriptCaller m_ObjectSelectCallback;
				
	// Stored list of all Placed Objects, indexed by their WorldObject ID
	protected ref map<Object, EditorObject> m_WorldObjects = new map<Object, EditorObject>();
	
	protected ref array<ref EditorHiddenObject> m_DeletedObjects = {};
			
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
	
	// Stored list of all Placed Objects
	protected ref map<string, EditorObject> m_PlacedObjects = new map<string, EditorObject>();
	
	protected ref map<typename, ref Command> m_Commands = new map<typename, ref Command>();
	protected ref map<string, Command> m_CommandShortcutMap = new map<string, Command>();
			
	protected ref EditorNode m_Master;
	
	void Add(notnull EditorNode node)
	{
		m_Master.Add(node);
	}
		
	void Editor(Man player) 
	{		
		m_Player = player;
				
		MakeDirectory(ROOT_DIRECTORY);
		
		if (!m_Player) {
			m_Master = new EditorNode("SERVER", "SERVER", "");
		} else {
			m_Master = new EditorNode(player.GetIdentity().GetId(), player.GetIdentity().GetName(), "");
		}
		
		EditorNode edited_objects = new EditorNode("EditedObjects", "Edited Objects", IconRegular.OBJECT_GROUP);
		edited_objects.Add(new EditorNode("PlacedObjects", "Placed Objects", IconRegular.HAND));
		edited_objects.Add(new EditorNode("BrushedObjects", "Brushed Objects",IconRegular.BRUSH));
		edited_objects.Add(new EditorNode("HiddenObjects", "Hidden Objects", IconRegular.HIPPO));
		m_Master.Add(edited_objects);

		m_Master.Add(new EditorNode("Unknown", "Unknown", IconRegular.CHESS_QUEEN));
		m_Master.Add(new EditorNode("Plants", "Plants", IconRegular.TREE));
		m_Master.Add(new EditorNode("Rocks", "Rocks", IconRegular.HILL_ROCKSLIDE));
		m_Master.Add(new EditorNode("Clutter", "Clutter", IconRegular.TRASH));
		m_Master.Add(new EditorNode("Structures", "Structures", IconRegular.HOUSE));
		m_Master.Add(new EditorNode("Wrecks", "Wrecks", IconRegular.CAR_BURST));
		m_Master.Add(new EditorNode("AI", "AI", IconRegular.PERSON));
		m_Master.Add(new EditorNode("Water", "Water", IconRegular.WATER));
		m_Master.Add(new EditorNode("Vehicles", "Vehicles", IconRegular.CAR));
		m_Master.Add(new EditorNode("StaticObjects", "Static Objects", IconRegular.OBJECT_INTERSECT));
		m_Master.Add(new EditorNode("DynamicObjects", "Dynamic Objects", IconRegular.SHIRT));
		m_Master.Add(new EditorNode("ScriptedObjects", "Scripted Objects", IconRegular.CODE));

		
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
					
		string category = "Unknown";
		// handle config objects
		foreach (string path: config_paths) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type;
		        GetGame().ConfigGetChildName(path, i, type);
				if (GetGame().ConfigGetInt(path + " " + type + " scope") < 2) {
					continue;
				}
				
				if (GetDayZGame().IsForbiddenItem(type)) {
					continue;
				}
				
				array<string> full_path = {};
				GetGame().ConfigGetFullPath(path + " " + type, full_path);
				
				category = "Unknown";
				if ((full_path.Find("Weapon_Base") != -1) || (full_path.Find("Inventory_Base")) != -1) {
					category = "DynamicObjects";
				} else if (full_path.Find("HouseNoDestruct") != -1) {
					category = "Structures";
				} else if (full_path.Find("Car") != -1) {
					category = "Vehicles";
				} else if (full_path.Find("Man") != -1 || (full_path.Find("DZ_LightAI"))) {
					category = "AI";
				}
				
				m_Master[category].Add(new EditorConfigPlaceable(type, type, IconRegular.BUILDING));
		    }
		}
		
		array<string> paths = { "DZ\\plants", "DZ\\plants_bliss", "DZ\\rocks", "DZ\\rocks_bliss" };
		foreach (string model_path: paths) {
			array<ref CF_File> files = {};
			if (!CF_Directory.GetFiles(model_path + "\\*", files, FindFileFlags.ARCHIVES)) {
				continue;
			}
				
			foreach (CF_File file: files) {		
				if (!file || file.GetExtension() != ".p3d") {
					continue;
				}
				
				string model_name;
				array<string> items = {};
				string full_path_p3d = file.GetFullPath();
				full_path_p3d.Replace("/", "\\");
				full_path_p3d.Split("\\", items);
				if (items.Count() != 0) {
					model_name = items[items.Count() - 1];
				}
				
				category = "StaticObjects";
				if ((items.Find("tree") != -1) || (items.Find("bush") != -1)) {
					category = "Plants";
				} else if (items.Find("clutter") != -1) {
					category = "Clutter";
				} else if (items.Find("rocks") != -1) {
					category = "Rocks";
				}
			
				m_Master[category].Add(new EditorStaticPlaceableItem(file.GetFullPath(), model_name, IconRegular.CIRCLE_C));
			}
		}

		foreach (Param3<typename, string, string> scripted_instance: RegisterEditorObject.Instances) {
			m_Master["ScriptedObjects"].Add(new EditorConfigPlaceable(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}
				
		if (GetGame().IsDedicatedServer()) {
			return;
		}
						
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
							
	
		m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + "0 10 0", ECE_LOCAL));
		ControlCamera(m_Camera);	
	
	
		m_EditorHud = new EditorHud();

		array<string> categories = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
		foreach (string category1: categories) {
			m_EditorHud.GetTemplateController().LeftListItems.Insert(m_Master[category1].GetNodeView());
		}
	
		m_EditorHud.GetTemplateController().RightListItems.Insert(m_Master["EditedObjects"].GetNodeView());
	}

	void ~Editor() 
	{
		GetGame().ObjectDelete(m_Camera);
	}
		
	void Update(bool doSim, float timeslice)
	{
		if (GetGame().IsDedicatedServer()) {
			return;
		}
		
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
			//EditorObjectDragHandler.Drag(m_DragTarget, m_DragOffset);
		}
		
		Ray ray1 = m_Camera.PerformCursorRaycast();
		Ray ray2 = m_Camera.GetCursorRay();
		
		vector camera_orthogonal[4] = { ray1.Direction * ray2.Direction, ray2.Direction, ray1.Direction, ray1.Position };
		Math3D.MatrixOrthogonalize4(camera_orthogonal);	
		
		vector rotation_mat[3];
		Math3D.MatrixIdentity3(rotation_mat);
		if (GetGame().GetInput().LocalPress_ID(UAZoomInOptics)) {
			Math3D.YawPitchRollMatrix(Vector(-15, 0, 0), rotation_mat);
		}
		
		if (GetGame().GetInput().LocalPress_ID(UAZoomOutOptics)) {
			Math3D.YawPitchRollMatrix(Vector(15, 0, 0), rotation_mat);
		}
		
		Math3D.MatrixMultiply3(camera_orthogonal, rotation_mat, camera_orthogonal);
		
		//Shape.CreateMatrix(camera_orthogonal);
		
		m_CursorNormal = m_CursorNormal.Multiply3(rotation_mat);
		//Print(m_CursorNormal);
		//ray1.Debug();
		
		foreach (Object object, EditorHandData data1: Placing) {
			vector transform[4];
			object.GetTransform(transform);
						
			vector size = ObjectGetSize(object);
			
			transform = { m_CursorNormal, ray1.Direction, m_CursorNormal * ray1.Direction, ray1.Position + Vector(0, (size[1] / 2) * ray1.Direction.Length(), 0) };
			
			object.SetTransform(transform);
			
			// diag
			transform[3] = ray1.Position;
			//Shape.CreateMatrix(transform);
		}
		
		if (GetGame().GetInput().LocalPress_ID(UAFire) && GetWidgetUnderCursor() && GetWidgetUnderCursor().GetName() != "Panel") {
			foreach (Object object_to_place, EditorHandData data: Placing) {
				EditorObject editor_object = new EditorObject(UUID.Generate(), object_to_place.GetType(), IconSolid.CIRCLE_DOT, object_to_place, EFE_DEFAULT);
				
				m_Master["EditedObjects"]["PlacedObjects"].Add(editor_object);
				
				m_PlacedObjects[editor_object.GetUUID()] = editor_object;
				m_WorldObjects[object_to_place] = editor_object;
				
				OnObjectCreated.Invoke(editor_object);
				
				// Synchronize to this id
				Synchronize(m_Master["EditedObjects"]["PlacedObjects"]);
				//m_Master["EditedObjects"]["PlacedObjects"].Synchronize();
				
				// remove it from placing
				Placing.Remove(object_to_place);
			}
		}
		
		if (GetGame().GetInput().LocalPress("EditorToggleUI")) {
			m_EditorHud.Show(!m_EditorHud.IsVisible());
		}
	}
			
	void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{		
		switch (rpc_type) {
			case EditorNode.RPC_SYNC: {	
				Print("EditorNode.RPC_SYNC");
				
				
				int tree_depth;
				if (!ctx.Read(tree_depth)) {
					Error("Invalid depth");
					break;
				}
								
				EditorNode current = m_Master;
				for (int i = 0; i < tree_depth - 1; i++) {
					string uuid;
					ctx.Read(uuid);
					
					string type;
					ctx.Read(type);
					Print(uuid);
					if (!current[uuid]) {
						current[uuid] = EditorNode.Cast(type.ToType().Spawn());
				
						if (!current[uuid]) {
							Error("Invalid node type " + type);
							continue;
						}
					}
					
					current = current[uuid];
				}
								
				current.Read(ctx, 0);
				
				// Who do we sync back to? - doing predictive placement so not the client that pushed it.. yet
				if (GetGame().IsDedicatedServer()) {
					array<PlayerIdentity> identities = {};
					Print(sender);
					foreach (PlayerIdentity identity: identities) {
						Print(identity);
						//Synchronize(current, identity);
					}
					
				} else {
					current.OnSynchronized();
				}
				
				break;
			}
		}
	}
	
	void Synchronize(notnull EditorNode node, PlayerIdentity identity = null)
	{	
		ScriptRPC rpc = new ScriptRPC();
		
		int tree_depth = node.GetParentDepth();
		rpc.Write(tree_depth);

		for (int i = tree_depth - 1; i >= 0; i--) {
			EditorNode parent = node.GetParentAtDepth(i);
			rpc.Write(parent.GetUUID());
			rpc.Write(parent.Type().ToString());
		}
			
		node.Write(rpc, 0);
		rpc.Send(null, EditorNode.RPC_SYNC, true, identity);
	}
		
	override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);
		
		serializer.Write(m_Player);
		serializer.Write(Public);
		serializer.Write(m_Password);
		serializer.Write(m_MaxPlayers);
		
		serializer.Write(m_Editors.Count());
		foreach (PlayerIdentity identity: m_Editors) {			
			serializer.Write(identity);
		}
				
		m_Master.Write(serializer, version);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(m_Player);
		serializer.Read(Public);
		serializer.Read(m_Password);
		serializer.Read(m_MaxPlayers);
		
		int editor_count;
		serializer.Read(editor_count);	
		for (int i = 0; i < editor_count; i++) {
			PlayerIdentity identity;
			serializer.Read(identity);
			
			if (!m_CameraObjects[identity]) {
				m_CameraObjects[identity] = GetGame().CreateObjectEx("StaticCamera", vector.Zero, ECE_LOCAL);
			}
		}
					
		m_Master.Read(serializer, version);
		return true;
	}
	
	bool IsMember(notnull PlayerIdentity identity)
	{
		return m_Editors.Find(identity) != -1;
	}
	
	array<PlayerIdentity> GetOnlineMembers()
	{
		return m_Editors;
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
	
	protected vector m_CursorNormal = vector.Aside;
				
	// EditorSounds is helpful
	void PlaySound(string sound_set)
	{
		SEffectManager.PlaySoundOnObject(sound_set, m_Camera);
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
			camera = m_Camera;
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
	
	void ClearPlacing()
	{
		foreach (Object object, EditorHandData data: Placing) {
			GetGame().ObjectDelete(object);
		}
		
		Placing.Clear();
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
	
	static int GetAutoSaveValue(float x)
	{
		return (5 * Math.Pow(x, 4) / 8) - (5 * Math.Pow(x, 3) / 12) - (45 * Math.Pow(x, 2) / 8) + (545 * x / 12) - 25;
	}
		
	/*
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
			
			m_Editor = GetDayZGame().GetEditor();
			
		}
		
		if (clear_before) {
			GetDayZGame().GetEditor().Clear();
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
	}*/
	

/*
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
	}*/
				
	void InsertAction(EditorAction action) 
	{
		m_ActionStack.InsertAction(action);
	}
			
	EditorHud GetHud() 
	{
		return m_EditorHud;
	}
		
	EditorCamera GetCamera() 
	{
		return m_Camera;
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
				
	EditorObject FindEditorObject(Object world_object) 
	{
		return m_WorldObjects[world_object];
	}

	map<string, Command> GetCommandShortcutMap()
	{
		return m_CommandShortcutMap;
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