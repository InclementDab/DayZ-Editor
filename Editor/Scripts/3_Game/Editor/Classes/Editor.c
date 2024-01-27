/*
                    Lloyd sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/



class EditorColors
{
	private void EditorColors();
	private void ~EditorColors();
	
	static const int SELECT = ARGB(255, 7, 111, 146);
	static const int BACKGROUND_1 = ARGB(255, 30, 30, 25);
	static const int HIDDEN_1 = ARGB(255, 131, 131, 149);
}

class Editor: TreeNode
{
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	static const int DEFAULT_ENTITY_COUNT = 512;
	
	protected PlayerIdentity m_Identity;
	protected DayZPlayer m_Player;
	
	protected EditorCamera m_Camera;
	protected ref EditorHud	m_Hud;
	ref EditorCursorTool Tool;

	ref array<ref EditorObject> Placing = {};
	
	protected vector m_CursorNormal = vector.Aside;
	
	// Stack of Undo / Redo Actions
	protected ref EditorHistory m_History = new EditorHistory();
	
	protected ref map<string, TreeNode> m_CommandShortcutMap = new map<string, TreeNode>();
		
	static const string EDITED_OBJECTS = "EditedObjects";
	static const string COMMANDS = "Commands";
	static const string UNDO_REDO = "UndoRedo";
	static const string PLACEABLE_OBJECTS = "PlaceableObjects";
	static const string BRUSHES = "Brushes";
		
	void Editor(string uuid, string display_name, Symbols icon, PlayerIdentity identity, DayZPlayer player) 
	{
		m_Identity = identity;
		m_Player = player;	
		
		// Load all default categories and placements
		TreeNode edited_objects = new TreeNode(EDITED_OBJECTS, "Edited Objects", Symbols.OBJECT_GROUP);
		edited_objects.Add(new TreeNode("PlacedObjects", "Placed Objects", Symbols.HAND));
		edited_objects.Add(new TreeNode("BrushedObjects", "Brushed Objects",Symbols.BRUSH));
		edited_objects.Add(new TreeNode("HiddenObjects", "Hidden Objects", Symbols.HIPPO));
		Add(edited_objects);
		
		TreeNode commands = new TreeNode(COMMANDS, "Commands", Symbols.COMMAND);		
		commands.Add(new AfterlifeCommand("Afterlife", "View Hidden", Symbols.GHOST));
		commands.Add(new BoltCommand("Bolt", "Lightning Bolt", Symbols.BOLT));
		commands.Add(new BoxSelectCommand("BoxSelect", "Box Selection", Symbols.SQUARE_DASHED));
		commands.Add(new CameraCommand("Camera", "Camera", Symbols.CAMERA));
		commands.Add(new CircleSelectCommand("CircleSelect", "Circle Selection", Symbols.CIRCLE_DASHED));
		commands.Add(new CopyCommand("Copy", "Copy", Symbols.COPY));
		commands.Add(new CutCommand("Cut", "Cut", Symbols.SCISSORS));
		commands.Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH));
		commands.Add(new GroundCommand("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE));
		commands.Add(new LassoSelectCommand("LassoSelect", "Lasso Select", Symbols.LASSO));
		commands.Add(new LockCommand("Lock", "Lock", Symbols.LOCK));
		commands.Add(new MagnetCommand("Magnet", "Magnet", Symbols.MAGNET));
		commands.Add(new NewCommand("New", "New File", Symbols.FILE));
		commands.Add(new OpenCommand("Open", "Open File", Symbols.FOLDER_OPEN));
		commands.Add(new PasteCommand("Paste", "Paste", Symbols.PASTE));
		commands.Add(new RedoCommand("Redo", "Redo", Symbols.ROTATE_RIGHT));
		commands.Add(new SaveAsCommand("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN));
		commands.Add(new SaveCommand("Save", "Save", Symbols.FLOPPY_DISK));
		commands.Add(new SnappingCommand("Snap", "Snapping Mode", Symbols.THUMBTACK));
		commands.Add(new UndoCommand("Undo", "Undo", Symbols.ROTATE_LEFT));
		commands.Add(new UnlockCommand("Unlock", "Unlock", Symbols.LOCK_OPEN));
		commands.Add(new WeatherCommand("Weather", "Weather", Symbols.CLOUD_SUN));
		
		Add(commands);
		
		TreeNode undo_redo = new TreeNode(UNDO_REDO, "History", Symbols.CLOCK_ROTATE_LEFT);
		Add(undo_redo);
		
		TreeNode placeable_objects = new TreeNode(PLACEABLE_OBJECTS, "Placeable Objects", Symbols.ADDRESS_BOOK);
		placeable_objects.Add(new TreeNode("Unknown", "Unknown", Symbols.CHESS_QUEEN));
		placeable_objects.Add(new TreeNode("Plants", "Plants", Symbols.TREE));
		placeable_objects.Add(new TreeNode("Rocks", "Rocks", Symbols.HILL_ROCKSLIDE));
		placeable_objects.Add(new TreeNode("Clutter", "Clutter", Symbols.TRASH));
		placeable_objects.Add(new TreeNode("Structures", "Structures", Symbols.HOUSE));
		placeable_objects.Add(new TreeNode("Wrecks", "Wrecks", Symbols.CAR_BURST));
		placeable_objects.Add(new TreeNode("AI", "AI", Symbols.PERSON));
		placeable_objects.Add(new TreeNode("Water", "Water", Symbols.WATER));
		placeable_objects.Add(new TreeNode("Vehicles", "Vehicles", Symbols.CAR));
		placeable_objects.Add(new TreeNode("StaticObjects", "Static Objects", Symbols.OBJECT_INTERSECT));
		placeable_objects.Add(new TreeNode("DynamicObjects", "Dynamic Objects", Symbols.SHIRT));
		placeable_objects.Add(new TreeNode("ScriptedObjects", "Scripted Objects", Symbols.CODE));
		Add(placeable_objects);
		
		TreeNode brushes = new TreeNode(BRUSHES, "Brushes", Symbols.BRUSH);
		brushes.Add(new BetulaPendula_Brush("BetulaPendula_Brush", "Betula Pendula", Symbols.TREES));
		brushes.Add(new LightningBrush("LightningBrush", "Lightning Brush", Symbols.BOLT));
		Add(brushes);
		
		
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };

		string category = "Unknown";
		// handle config objects

#ifndef WORKBENCH
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
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
				
				this[PLACEABLE_OBJECTS][category].Add(new EditorPlaceable(type, type, Symbols.BUILDING));
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
			
				this[PLACEABLE_OBJECTS][category].Add(new EditorPlaceable(file.GetFullPath(), model_name, Symbols.CIRCLE_C));
			}
		}

		foreach (Param3<typename, string, string> scripted_instance: RegisterEditorObject.Instances) {
			this[PLACEABLE_OBJECTS]["ScriptedObjects"].Add(new EditorPlaceable(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}		
#endif
	}

	void ~Editor() 
	{
		if (GetGame() && GetGame().GetUpdateQueue(CALL_CATEGORY_GUI)) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		}
		
		delete m_Hud;
		GetGame().ObjectDelete(m_Camera);
	}
			
	override void OnSynchronized()
	{
		delete m_Hud;
		if (!m_Hud) {
			m_Hud = new EditorHud(this);
		}
		
		// how long until this is a node?? :/
		if (!m_Camera) {
			if (!m_Player) {
				m_Player = DayZPlayer.Cast(m_Identity.GetPlayer());
				if (m_Player) {
					Error("Could not find player");
					return;
				}
			}
			
			m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + "0 10 0", ECE_LOCAL));
			m_Camera.SetActive(true);
		}
		
		// What?
		EnScript.SetClassVar(GetDayZGame(), "m_Editor", 0, this);
	}
		
	void Update(float timeslice)
	{
		Input input = GetGame().GetInput();
		if (!m_Camera) {
			return;
		}
			
		if (GetWidgetUnderCursor() && !GetWidgetUnderCursor().GetName().Contains("Panel")) {
			return;
		}
		
		Raycast raycast = m_Camera.PerformCursorRaycast();	
		if (Tool && !Tool.Update(timeslice, raycast)) {
			return; // MAYBE DDONT DO THIS HERE
		}
		
		vector camera_orthogonal[4] = { raycast.Source.Direction * raycast.Bounce.Direction, raycast.Bounce.Direction, raycast.Source.Direction, raycast.Source.Position };
		Math3D.MatrixOrthogonalize4(camera_orthogonal);	
		
		vector rotation_mat[3];
		Math3D.MatrixIdentity3(rotation_mat);
		if (input.LocalPress_ID(UAZoomInOptics)) {
			Math3D.YawPitchRollMatrix(Vector(-15, 0, 0), rotation_mat);
		}
		
		if (input.LocalPress_ID(UAZoomOutOptics)) {
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
		
		if (input.LocalPress_ID(UATempRaiseWeapon)) {
			TreeNode.ClearSelections();			
			return;
		}
		
		if (input.LocalPress_ID(UAFire)) {
			// The magic copy-paste code that handles all your interactive dreams. hasnt changed
			if (!KeyState(KeyCode.KC_LSHIFT) && !GetWidgetUnderCursor() && KeyState(KeyCode.KC_LMENU)) {
				TreeNode.ClearSelections();
			}
			
			if (raycast.Hit && EditorObject.ByObject[raycast.Hit]) {
				EditorObject editor_object = EditorObject.ByObject[raycast.Hit];
				if (KeyState(KeyCode.KC_LCONTROL)) {
					editor_object.SetSelected(!editor_object.IsSelected());
				} else {
					editor_object.SetSelected(true);
				}
			}
			
			foreach (EditorObject editor_object_to_place: Placing) {
				EditorFootprint footprint = new EditorFootprint(UUID.Generate(), "Undo Place", Symbols.CLOCK_ROTATE_LEFT);
				int tree_depth = editor_object_to_place.GetParentDepth();
				footprint.Data.GetWriteContext().Write(tree_depth);
				for (int i = tree_depth - 1; i >= 0; i--) {
					TreeNode parent = editor_object_to_place.GetParentAtDepth(i);
					footprint.Data.GetWriteContext().Write(parent.GetUUID());
					footprint.Data.GetWriteContext().Write(parent.Type().ToString());
				}

				editor_object_to_place.Write(footprint.Data.GetWriteContext(), 0);
				
				this["EditedObjects"]["PlacedObjects"].Add(editor_object_to_place);
	
				// Synchronize to this id
				this["EditedObjects"]["PlacedObjects"].Synchronize();
				
				// remove it from placing
				Placing.RemoveItem(editor_object_to_place);
				PlaySound(EditorSounds.PLOP);
				m_History.InsertAction(footprint);
			}
			
		}
		
		if (input.LocalHold_ID(UAFire)) {
			foreach (TreeNode selected_node: TreeNode.SelectedObjects) {
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

					float d1 = vector.Dot(vector.Up, raycast.Source.Direction);
					vector x = Math.Cos(d1) * (raycast.Source.Position - current_transform[3]);
					
					vector pos = raycast.Source.Position + raycast.Source.Direction * dist_z;
					
					current_transform = { current_transform[0], current_transform[1], current_transform[2], pos };
					editor_object_cast.SetBaseTransform(current_transform);
					
				} 
				

				if (KeyState(KeyCode.KC_LSHIFT)) {
					
					vector new_forward = vector.Direction(current_transform[3], raycast.Bounce.Position).Normalized();
					
					current_transform = { (current_transform[1] * new_forward).Normalized(), current_transform[1], new_forward, current_transform[3] };
					
					editor_object_cast.SetBaseTransform(current_transform);
					/*
					vector p1 = Vector(2, 0, 2).Multiply4(current_transform);
					vector p2 = Vector(-2, 0, -2).Multiply4(current_transform);
					
					Shape.Create(ShapeType.BBOX, COLOR_GREEN, ShapeFlags.WIREFRAME | ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.ADDITIVE, p1, p2);
					
					float d = 1;
					float t = -(vector.Dot(vector.Up, raycast.Source.Position) + current_transform[3][1]) / vector.Dot(vector.Up, raycast.Source.Direction);
					
					vector result = raycast.Source.Position + raycast.Source.Direction * t;
					
					*/
				}
				
				
				// Any distance placing
				else {
					transform = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
					editor_object_cast.SetBaseTransform(transform);
				}
			}
		}
		
		if (input.LocalRelease_ID(UAFire)) {
			foreach (TreeNode selected_node_synch: TreeNode.SelectedObjects) {
				EditorObject editor_object_sync = EditorObject.Cast(selected_node_synch);
				if (!editor_object_sync) {
					continue;
				}
				
				editor_object_sync.Synchronize();
			}
		}
		
		if (input.LocalPress_ID(UAZoomIn)) {
			m_Camera.FieldOfView = GameConstants.DZPLAYER_CAMERA_FOV_EYEZOOM;
		}
		
		if (input.LocalRelease_ID(UAZoomIn)) { 
			m_Camera.FieldOfView = 1.0;
		}
		
		if (GetGame().GetInput().LocalPress("EditorDeleteObject")) {
			foreach (TreeNode editor_node_to_delete: TreeNode.SelectedObjects) {
				if (!editor_node_to_delete.CanDelete()) {
					continue;
				}
				
				EditorFootprint footprint_delete = new EditorFootprint(UUID.Generate(), "Undo Delete", Symbols.CLOCK_ROTATE_LEFT);
				int delete_tree_depth = editor_node_to_delete.GetParentDepth();
				footprint_delete.Data.GetWriteContext().Write(delete_tree_depth);
				for (int i_d = delete_tree_depth - 1; i_d >= 0; i_d--) {
					TreeNode delete_parent = editor_node_to_delete.GetParentAtDepth(i_d);
					footprint_delete.Data.GetWriteContext().Write(delete_parent.GetUUID());
					footprint_delete.Data.GetWriteContext().Write(delete_parent.Type().ToString());
				}

				editor_node_to_delete.Write(footprint_delete.Data.GetWriteContext(), 0);
				
				this["EditedObjects"]["PlacedObjects"].Remove(editor_node_to_delete.GetUUID());
	
				// Synchronize to this id
				this["EditedObjects"]["PlacedObjects"].Synchronize();
				
				// remove it from placing
				TreeNode.SelectedObjects.RemoveItem(editor_node_to_delete);
				
				EditorObject editor_object_to_delete = EditorObject.Cast(editor_node_to_delete);
				if (editor_object_to_delete) {
					GetGame().ObjectDelete(editor_object_to_delete.GetObject());
				}
				
				
				delete editor_node_to_delete;
				PlaySound(EditorSounds.HIGHLIGHT); // must co-exist
				
				
				m_History.InsertAction(footprint_delete);
			}
		}
		
		if (input.LocalPress("EditorToggleCursor")) {
			bool cursor_state = !GetGame().GetUIManager().IsCursorVisible();
			GetGame().GetUIManager().ShowCursor(cursor_state);
			GetGame().GetUIManager().ShowUICursor(cursor_state);
			m_Hud.ClearCursor();
		}
		
		if (input.LocalPress("EditorToggleHudCommand")) {
			if (m_Camera.IsActive()) {
				m_Hud.Show(!m_Hud.IsVisible());
			}
		}
		
		if (GetGame().GetInput().LocalPress("EditorToggleActive")) {
			SetActive(!IsActive());
		}
		
		Widget root_widget = EnScriptVar<Widget>.Get(GetGame().GetMission(), "m_HudRootWidget");
		root_widget.Show(!IsActive());
	}
				
	override void Write(Serializer serializer, int version)
	{		
		super.Write(serializer, version);
		
		serializer.Write(m_Identity);
		Print(m_Identity);
		serializer.Write(m_Player);
		Print(m_Player);
		serializer.Write(m_Camera);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(m_Identity);
		Print(m_Identity);
		serializer.Read(m_Player);
		Print(m_Player);
		serializer.Read(m_Camera);			
		return true;
	}
						
	// EditorSounds is helpful
	void PlaySound(string sound_set)
	{
		SEffectManager.PlaySoundOnObject(sound_set, m_Camera);
	}
			
	void SetActive(bool active)
	{
		if (!IsLocal()) {
			return;
		}
		
		if (active) {
			m_Camera.SetActive(true);
			m_Hud.Show(true);
		} else {
			m_Hud.Show(false);
			
			GetDayZGame().SelectPlayer(m_Identity, m_Player);
			Hud hud = GetDayZGame().GetMission().GetHud();
			hud.ShowHudUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.HUD));
			hud.ShowQuickbarUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.QUICKBAR));
		}
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
		if (GetGame().GetPlayer()) {
			return GetGame().GetPlayer();
		} 

		Man player = Man.Cast(GetGame().CreatePlayer(null, type, position, 0, string.Empty));
		if (!player) {
			Error(string.Format("Failed to create new player, type %1", type));
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
		Math3D.MatrixOrthogonalize4(transform);
		
		Object object;
		if (type.Contains("\\") || type.Contains("/")) {
			object = GetGame().CreateStaticObjectUsingP3D(type, transform[3], transform[2].VectorToAngles(), 1.0, true);
		} else {
			object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL  | ECE_INITAI | ECE_CREATEPHYSICS);
		}
		
		if (!object) {
			return null;
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
					
	bool IsPlacing()
	{
		return Placing.Count() > 0; 
	}
	
	EditorProfileSettings GetProfileSettings()
	{
		return EditorProfileSettings.Cast(GetDayZGame().GetProfileSetting(EditorProfileSettings));
	}
		
	void SetPlayer(DayZPlayer player)
	{
		m_Player = player;
	}
	
	bool IsActive()
	{
		return m_Camera && m_Camera.IsActive();
	}
	
	bool IsLocal()
	{
		return m_UUID == GetGame().GetUserManager().GetTitleInitiator().GetUid();
	}
	
	// Hope you remembered to register it
	TreeNode FindCommandbyType(typename type)
	{
		return this[COMMANDS][type.ToString()];
	}
	
	TreeNode GetObjects()
	{
		return this[EDITED_OBJECTS];
	}
	
	TreeNode GetCommands()
	{
		return this[COMMANDS];
	}
	
	TreeNode GetCommand(string command)
	{
		return this[COMMANDS][command];
	}
	
	TreeNode GetUndoRedo()
	{
		return this[UNDO_REDO];
	}
	
	TreeNode GetPlaceables()
	{
		return this[PLACEABLE_OBJECTS];
	}
	
	TreeNode GetBrushes()
	{
		return this[BRUSHES];
	}
}