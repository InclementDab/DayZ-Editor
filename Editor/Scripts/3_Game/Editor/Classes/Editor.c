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

class EditorHandData: Managed
{
	EditorPlaceable Placeable;
	vector Matrix[4];
	
	void EditorHandData(notnull EditorPlaceable placeable, vector matrix[4])
	{
		Placeable = placeable;
		copyarray(Matrix, matrix);
	}
}

class EditorFootprint: Managed
{
	protected bool m_IsUndone;
	
	string ID = UUID.Empty;
	string Type;
	ref ScriptReadWriteContext Data = new ScriptReadWriteContext();
	
	bool IsUndone()
	{
		return m_IsUndone;
	}
	
	void Undo()
	{
		m_IsUndone = true;
		
		int tree_depth;
		if (!Data.GetReadContext().Read(tree_depth)) {
			Error("Invalid depth");
			return;
		}

		EditorNode current = GetDayZGame().GetEditor().GetMaster();
		for (int i = 0; i < tree_depth; i++) {
			string uuid;
			Data.GetReadContext().Read(uuid);
			
			string type;
			Data.GetReadContext().Read(type);
			
			EditorNode node = current[uuid];
			if (!node) {
				node = EditorNode.Cast(type.ToType().Spawn());
				if (!node) {
					Error("Invalid node type " + type);
					continue;
				}
				
				current[uuid] = node;
				node.SetParent(current[uuid]);
			}
			
			current = current[uuid];
		}
						
		current.Read(Data.GetReadContext(), 0);
	}
		
	void Redo()
	{
		m_IsUndone = false;
	}
}

class EditorHistory: set<ref EditorFootprint>
{
	static const int MAX_SIZE = 512;
	
	int InsertAction(EditorFootprint value)
	{	
		int count = Count();
		for (int i = 0; i < count; i++) {
			if (!this[i].IsUndone()) {
				break;
			}
			
			Remove(i);
			i--; count--;
		}
		
		if (count >= MAX_SIZE) {
			Remove(count - 1);
		}
		
		// Adds to bottom of stack
		return InsertAt(value, 0);
	}
}

class Editor: SerializableBase
{
	// Handled in DayZGame
	static const int RPC_SYNC = 54365;
	static const int RPC_REQUEST_SYNC = 54366;
	
	static const ref array<int> RPC_ALL = { RPC_SYNC, RPC_REQUEST_SYNC };
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	
	static const int DEFAULT_ENTITY_COUNT = 512;
	
	// m_Editor is only valid on the local instance.
	protected PlayerIdentity m_Identity;
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
	protected ref EditorHud	m_Hud;

	protected EditorCamera m_Camera;
	ref array<ref EditorObject> Placing = {};
	protected Entity m_CurrentControl;
				
	// todo: change this to some EditorFile struct that manages this better
	// bouncing around strings is a PAIN... i think it also breaks directories... maybe not
	protected string EditorSaveFile;
	static const string ROOT_DIRECTORY = "$saves:\\Editor\\";
	
	static const string Version = "2.0";
	
	protected ref array<ref EditorHiddenObject> m_DeletedObjects = {};	
	
	protected ref EditorNode m_Master = new EditorNode("MAIN", "MAIN", Symbols.PENCIL.Regular());			
	
	// Stack of Undo / Redo Actions
	protected ref EditorHistory m_History = new EditorHistory();
	
	void Editor(PlayerIdentity identity, Man player) 
	{
		m_Identity = identity;
		m_Player = player;
		
		MakeDirectory(ROOT_DIRECTORY);
		
		EditorNode networked = new EditorNode("NetworkedObjects", "Networked Objects", Symbols.NETWORK_WIRED.Regular());
		networked.Add(new EditorNetworkedObject(m_Identity.GetId(), m_Identity.GetName(), Symbols.PERSON.Regular(), m_Player));
		m_Master.Add(networked);
		
		// Load all default categories and placements
		EditorNode edited_objects = new EditorNode("EditedObjects", "Edited Objects", Symbols.OBJECT_GROUP.Regular());
		edited_objects.Add(new EditorNode("PlacedObjects", "Placed Objects", Symbols.HAND.Regular()));
		edited_objects.Add(new EditorNode("BrushedObjects", "Brushed Objects",Symbols.BRUSH.Regular()));
		edited_objects.Add(new EditorNode("HiddenObjects", "Hidden Objects", Symbols.HIPPO.Regular()));
		m_Master.Add(edited_objects);

		EditorNode placeable_objects = new EditorNode("PlaceableObjects", "Placeable Objects", Symbols.ADDRESS_BOOK.Regular());
		placeable_objects.Add(new EditorNode("Unknown", "Unknown", Symbols.CHESS_QUEEN.Regular()));
		placeable_objects.Add(new EditorNode("Plants", "Plants", Symbols.TREE.Regular()));
		placeable_objects.Add(new EditorNode("Rocks", "Rocks", Symbols.HILL_ROCKSLIDE.Regular()));
		placeable_objects.Add(new EditorNode("Clutter", "Clutter", Symbols.TRASH.Regular()));
		placeable_objects.Add(new EditorNode("Structures", "Structures", Symbols.HOUSE.Regular()));
		placeable_objects.Add(new EditorNode("Wrecks", "Wrecks", Symbols.CAR_BURST.Regular()));
		placeable_objects.Add(new EditorNode("AI", "AI", Symbols.PERSON.Regular()));
		placeable_objects.Add(new EditorNode("Water", "Water", Symbols.WATER.Regular()));
		placeable_objects.Add(new EditorNode("Vehicles", "Vehicles", Symbols.CAR.Regular()));
		placeable_objects.Add(new EditorNode("StaticObjects", "Static Objects", Symbols.OBJECT_INTERSECT.Regular()));
		placeable_objects.Add(new EditorNode("DynamicObjects", "Dynamic Objects", Symbols.SHIRT.Regular()));
		placeable_objects.Add(new EditorNode("ScriptedObjects", "Scripted Objects", Symbols.CODE.Regular()));
		m_Master.Add(placeable_objects);
		
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
				
				m_Master["PlaceableObjects"][category].Add(new EditorPlaceable(type, type, Symbols.BUILDING.Regular()));
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
			
				m_Master["PlaceableObjects"][category].Add(new EditorPlaceable(file.GetFullPath(), model_name, Symbols.CIRCLE_C.Regular()));
			}
		}

		foreach (Param3<typename, string, string> scripted_instance: RegisterEditorObject.Instances) {
			m_Master["PlaceableObjects"]["ScriptedObjects"].Add(new EditorPlaceable(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}
			
		if (GetGame().IsDedicatedServer()) {
			return;
		}
		
		m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + "0 10 0", ECE_LOCAL));
		ControlCamera(m_Camera);	
	
		m_Hud = new EditorHud();		
		m_Hud.GetTemplateController().LeftListItems.Insert(m_Master["PlaceableObjects"].GetNodeView());
		m_Hud.GetTemplateController().RightListItems.Insert(m_Master["NetworkedObjects"].GetNodeView());
		m_Hud.GetTemplateController().RightListItems.Insert(m_Master["EditedObjects"].GetNodeView());
		
		// Hud has loaded, request the stuff
		ScriptRPC().Send(null, RPC_REQUEST_SYNC, true);
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
			
		if ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) != MB_PRESSED_MASK) {
			m_DragTarget = null;
		}
		
		if (m_DragTarget) {
			//EditorObjectDragHandler.Drag(m_DragTarget, m_DragOffset);
		}
		
		Raycast raycast = m_Camera.PerformCursorRaycast();		
		vector camera_orthogonal[4] = { raycast.Source.Direction * raycast.Bounce.Direction, raycast.Bounce.Direction, raycast.Source.Direction, raycast.Source.Position };
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
		
		//Print(Placing.Count());
		foreach (EditorObject editor_object_placing: Placing) {
			vector transform[4] = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
			editor_object_placing.SetBaseTransform(transform);
		}
		
		if (GetGame().GetInput().LocalPress_ID(UAFire)) {			
			// Cursed but we ship it
			if (!GetWidgetUnderCursor() || !GetWidgetUnderCursor().GetName().Contains("Panel")) {
				foreach (EditorObject editor_object_to_place: Placing) {
					EditorFootprint footprint = new EditorFootprint();
					int tree_depth = editor_object_to_place.GetParentDepth();
					footprint.Data.GetWriteContext().Write(tree_depth);
					for (int i = tree_depth - 1; i >= 0; i--) {
						EditorNode parent = editor_object_to_place.GetParentAtDepth(i);
						footprint.Data.GetWriteContext().Write(parent.GetUUID());
						footprint.Data.GetWriteContext().Write(parent.Type().ToString());
					}

					editor_object_to_place.Write(footprint.Data.GetWriteContext(), 0);
					
					m_Master["EditedObjects"]["PlacedObjects"].Add(editor_object_to_place);
		
					// Synchronize to this id
					Synchronize(m_Master["EditedObjects"]["PlacedObjects"]);
					
					// remove it from placing
					Placing.RemoveItem(editor_object_to_place);
					
					m_History.InsertAction(footprint);
				}
			}
		}
		
		if (GetGame().GetInput().LocalHold_ID(UAFire)) {
			foreach (EditorNode selected_node: EditorNode.SelectedObjects) {
				EditorObject editor_object_cast = EditorObject.Cast(selected_node);
				if (!editor_object_cast) {
					continue;
				}
								
				vector current_transform[4];
				editor_object_cast.GetBaseTransform(current_transform);
				
				// Held distance placing
				if (KeyState(KeyCode.KC_LMENU)) {
					Debug.DrawSphere(raycast.Source.Position, vector.Distance(raycast.Source.Position, current_transform[3]), COLOR_RED, ShapeFlags.ADDITIVE | ShapeFlags.WIREFRAME | ShapeFlags.ONCE);
					
					vector v3 = current_transform[1] * raycast.Source.Direction;					
					float dist_z = vector.Dot(((raycast.Source.Position - current_transform[3]) * current_transform[1]), v3) / v3.LengthSq();
					vector pos = raycast.Source.Position + raycast.Source.Direction * dist_z;
															
					current_transform = { current_transform[0], current_transform[1], current_transform[2], pos };
					editor_object_cast.SetBaseTransform(current_transform);
				} 
				
				// Any distance placing
				else {
					transform = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
					editor_object_cast.SetBaseTransform(transform);
				}
			}
		}
		
		if (GetGame().GetInput().LocalHold_ID(UAZoomIn)) {
			if (raycast.Hit) {
				m_Master["NetworkedObjects"].Add(new EditorNetworkedObject(raycast.Hit.GetNetworkIDString(), raycast.Hit.GetNetworkIDString(), Symbols.BUILDING.Regular(), raycast.Hit));

			}
		}
		
		if (GetGame().GetInput().LocalPress("EditorToggleUI")) {
			m_Hud.Show(!m_Hud.IsVisible());
		}
	}
			
	void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{		
		switch (rpc_type) {
			case RPC_REQUEST_SYNC: {
				if (!GetGame().IsServer()) {
					return;
				}
				
				GetDayZGame().GetEditor().Synchronize(GetDayZGame().GetEditor().GetMaster(), sender);
				break;
			}
			
			case RPC_SYNC: {	
				Print("EditorNode.RPC_SYNC");
				int tree_depth;
				if (!ctx.Read(tree_depth)) {
					Error("Invalid depth");
					break;
				}

				EditorNode current = m_Master;
				for (int i = 0; i < tree_depth; i++) {
					string uuid;
					ctx.Read(uuid);
					
					string type;
					ctx.Read(type);
					
					EditorNode node = current[uuid];
					if (!node) {
						node = EditorNode.Cast(type.ToType().Spawn());
						if (!node) {
							Error("Invalid node type " + type);
							continue;
						}
						
						current[uuid] = node;
						node.SetParent(current[uuid]);
					}
					
					current = current[uuid];
				}
								
				current.Read(ctx, 0);
				
				// Who do we sync back to? - doing predictive placement so not the client that pushed it.. yet
				if (GetGame().IsDedicatedServer()) {
					array<PlayerIdentity> identities = {};
					GetGame().GetPlayerIndentities(identities);
					foreach (PlayerIdentity identity: identities) {
						//if (sender.GetId() != identity.GetId())
						Synchronize(current, null);
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
		rpc.Send(null, RPC_SYNC, true, identity);
	}
		
	override void Write(Serializer serializer, int version)
	{		
		serializer.Write(m_Player);
		serializer.Write(Public);
		serializer.Write(m_Password);
		serializer.Write(m_MaxPlayers);
		
		serializer.Write(m_Editors.Count());
		foreach (PlayerIdentity identity: m_Editors) {			
			serializer.Write(identity);
		}
				
		m_Master.Write(serializer, version);
		
		super.Write(serializer, version);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
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
		return super.Read(serializer, version);
	}
	
	void Add(notnull EditorNode node)
	{
		m_Master.Add(node);
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
	
	bool DoCursorRaycast(out vector position, float max_distance = 3000, Object ignore_object = null)
	{
		vector raycast_direction;
		if (m_Hud.IsCursorVisible()) {
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
		m_Hud = new EditorHud();
		return m_Hud;
	}
		
	void Undo()
	{
		foreach (EditorFootprint footprint: m_History) {
			if (!footprint.IsUndone()) {
				footprint.Undo();
				return;
			}
		}
	}
	
	void Redo()
	{
		for (int i = m_History.Count() - 1; i >= 0; i--) {
			if (m_History[i] && m_History[i].IsUndone()) {
				m_History[i].Redo();
				return;
			}
		}
	}
	
	bool CanUndo() 
	{
		foreach (EditorFootprint footprint: m_History) {
			if (footprint && !footprint.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool CanRedo() 
	{
		for (int i = m_History.Count() - 1; i >= 0; i--) {
			if (m_History[i] && m_History[i].IsUndone()) {
				return true;
			}
		}
		
		return false;
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
	
	static Object CreateObject(string type, vector transform[4])
	{
		Object object;
		if (type.Contains("\\") || type.Contains("/")) {
			object = GetGame().CreateStaticObjectUsingP3D(type, transform[3], transform[2].VectorToAngles(), 1.0, true);
		} else {
			object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL);
		}
		object.SetTransform(transform);
		object.Update();	
		
		return object;
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
			m_Hud.ShowNotification(error_message, COLOR_YELLOW);
			
			// Disable auto save since we loaded a shit file
			//Settings.AutoSaveTimer = -1;
		} else {
			m_Hud.ShowNotification(string.Format("Loaded %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count()), COLOR_GREEN);
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
				
	EditorHud GetHud() 
	{
		return m_Hud;
	}
		
	EditorCamera GetCamera() 
	{
		return m_Camera;
	}
		
	bool IsObjectHidden(notnull Object object)
	{
		return (CF.ObjectManager.IsMapObjectHidden(object));
	}
			
	array<ref EditorHiddenObject> GetDeletedObjects()
	{
		return m_DeletedObjects;
	}
				
	bool IsPlacing()
	{
		return Placing.Count() > 0; 
	}
	
	EditorProfileSettings GetProfileSettings()
	{
		return EditorProfileSettings.Cast(GetDayZGame().GetProfileSetting(EditorProfileSettings));
	}
		
	EditorNode GetMaster()
	{	
		return m_Master;
	}
}