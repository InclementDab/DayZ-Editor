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
	
	protected ref map<string, TreeNode> m_CommandShortcutMap = new map<string, TreeNode>();
		
	static const string EDITED_OBJECTS = "EditedObjects";
	static const string COMMANDS = "Commands";
	static const string MENUS = "Menus";
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
		commands.Add(new CommandNode("Afterlife", "View Hidden", Symbols.GHOST, ShortcutKeyType.TOGGLE));
		commands.Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT, ShortcutKeyType.TOGGLE));
		commands.Add(new CommandNode("BoxSelect", "Box Selection", Symbols.SQUARE_DASHED, ShortcutKeyType.PRESS));
		commands.Add(new CommandNode("Camera", "Camera", Symbols.CAMERA, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("CircleSelect", "Circle Selection", Symbols.CIRCLE_DASHED, ShortcutKeyType.PRESS));
		commands.Add(new CommandNode("Copy", "Copy", Symbols.COPY, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Delete", "Delete", Symbols.TRASH, ShortcutKeyType.PRESS));
		commands.Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE, ShortcutKeyType.PRESS));
		commands.Add(new CommandNode("LassoSelect", "Lasso Select", Symbols.LASSO, ShortcutKeyType.PRESS));
		commands.Add(new CommandNode("Lock", "Lock", Symbols.LOCK, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("New", "New File", Symbols.FILE, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Open", "Open File", Symbols.FOLDER_OPEN, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Paste", "Paste", Symbols.PASTE, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("Weather", "Weather", Symbols.CLOUD_SUN, ShortcutKeyType.HOLD));
		commands.Add(new CommandNode("CursorToggle", "Toggle Cursor", Symbols.ARROW_POINTER, ShortcutKeyType.TOGGLE));
		Add(commands);
		
		TreeNode menus = new TreeNode(MENUS, "Menus", Symbols.SQUARE_LIST);
		TreeNode file_menu = new TreeNode("File", "File", Symbols.FILE_SPREADSHEET);
		file_menu.Add(commands["New"]);
		file_menu.Add(commands["Open"]);
		file_menu.Add(commands["Save"]);
		file_menu.Add(commands["SaveAs"]);
		menus.Add(file_menu);
		
		TreeNode edit_menu = new TreeNode("Edit", "Edit", Symbols.FILE_PEN);
		menus.Add(edit_menu);
		
		TreeNode view_menu = new TreeNode("View", "View", Symbols.BINOCULARS);
		menus.Add(view_menu);
		
		TreeNode mission_menu = new TreeNode("Mission", "Mission", Symbols.PERSON_RIFLE);
		menus.Add(mission_menu);
		
		Add(menus);
				
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
	
	void InsertHistory(string display_name, Symbols icon, TreeNode node, ScriptReadWriteContext data)
	{
		// Clear the stack first
		foreach (string uuid, TreeNode undo_redo_node: this[UNDO_REDO].Children) {	
			EditorFootprint footprint = EditorFootprint.Cast(undo_redo_node);
			if (!footprint) {
				continue;
			}
						
			if (!footprint.IsUndone()) {
				break;
			}	
			
			delete this[UNDO_REDO][uuid];
		}
		
		string uuid_generated = string.Format("History:%1", this[UNDO_REDO].Children.Count());		
		this[UNDO_REDO][uuid_generated] = new EditorFootprint(uuid_generated, display_name, icon, node, data);
	}
				
	void Update(float timeslice)
	{
		//Synchronize();
		
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
				InsertHistory(string.Format("Undo Place %1", editor_object_to_place.GetUUID()), Symbols.CLOCK_ROTATE_LEFT, editor_object_to_place, null);
				this[EDITED_OBJECTS]["PlacedObjects"].Add(editor_object_to_place);
				this[EDITED_OBJECTS]["PlacedObjects"].Synchronize();
				
				// remove it from placing
				Placing.RemoveItem(editor_object_to_place);
				PlaySound(EditorSounds.PLOP);				
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
		serializer.Write(m_Player);
		serializer.Write(m_Camera);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(m_Identity);	
		serializer.Read(m_Player);
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
		foreach (string uuid, TreeNode node: this[UNDO_REDO].Children) {
			EditorFootprint footprint = EditorFootprint.Cast(node);
			if (!footprint) {
				continue;
			}
			
			if (!footprint.IsUndone()) {
				footprint.Undo();
				return;
			}
		}
	}
	
	void Redo()
	{
		for (int i = this[UNDO_REDO].Children.Count() - 1; i >= 0; i--) {
			EditorFootprint footprint = EditorFootprint.Cast(this[UNDO_REDO].Children.GetElement(i));
			if (!footprint) {
				continue;
			}
			
			if (footprint.IsUndone()) {
				footprint.Redo();
				return;
			}
		}
	}
	
	bool CanUndo() 
	{
		foreach (string uuid, TreeNode node: this[UNDO_REDO].Children) {
			EditorFootprint footprint = EditorFootprint.Cast(node);
			if (footprint && !footprint.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool CanRedo() 
	{
		for (int i = this[UNDO_REDO].Children.Count() - 1; i >= 0; i--) {
			EditorFootprint footprint = EditorFootprint.Cast(this[UNDO_REDO].Children.GetElement(i));
			if (footprint && footprint.IsUndone()) {
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
		return GetGame() && GetGame().GetUserManager() && GetGame().GetUserManager().GetTitleInitiator() && m_UUID == GetGame().GetUserManager().GetTitleInitiator().GetUid();
	}
		
	TreeNode GetObjects()
	{
		return this[EDITED_OBJECTS];
	}
	
	TreeNode GetCommands()
	{
		return this[COMMANDS];
	}
	
	CommandNode GetCommand(string command)
	{
		return CommandNode.Cast(this[COMMANDS][command]);
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
	
	TreeNode GetMenu(string menu)
	{
		return this[MENUS][menu];
	}
}