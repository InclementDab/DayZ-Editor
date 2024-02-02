/*
                    Euler sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/

class EditorNode: TreeNode
{	
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	static const int DEFAULT_ENTITY_COUNT = 512;
	
	PlayerIdentity Identity;
	DayZPlayer Player;
	
	protected vector m_CursorNormal = vector.Aside;
	protected EditorCamera m_Camera;
	protected ref EditorHud	m_Hud;
	
	protected ref map<string, TreeNode> m_CommandShortcutMap = new map<string, TreeNode>();
	
	//[TreeNode("SERVER\\Layers", "Layers", Symbols.LAYER_GROUP)]
	static const string LAYERS = "Layers";
	
		//[TreeNode("SERVER\\Layers\\Brushed", "Brushed", Symbols.PAINTBRUSH)];
		static const string BRUSHED = "Brushed";
		static const string HIDDEN = "Hidden";
	
	static const string COMMANDS = "Commands";
		static const string TOOLS = "Tools";
	
	static const string MENUS = "Menus";
	static const string HISTORY = "UndoRedo";
	static const string PLACEABLES = "Placeables";
	static const string DZ = "DZ";
	static const string BRUSHES = "Brushes";
	static const string PLACING = "Placing";
	static const string RECYCLE = "Recycle";
	
	void EditorNode(string uuid, string display_name, Symbols icon) 
	{				
		// Load all default categories and placements
		Add(new IndestructableFolderNode(LAYERS, "Layers", Symbols.LAYER_GROUP));
		Add(new TreeNode(COMMANDS, "Commands", Symbols.COMMAND));
		Add(new TreeNode(DZ, "DZ", Symbols.FOLDER));
		Add(new TreeNode(MENUS, "Menus", Symbols.SQUARE_LIST));
		Add(new TreeNode(HISTORY, "History", Symbols.CLOCK_ROTATE_LEFT));
		Add(new TreeNode(PLACEABLES, "Placeable Objects", Symbols.ADDRESS_BOOK));
		Add(new TreeNode(BRUSHES, "Brushes", Symbols.BRUSH));
		Add(new TreeNode(PLACING, "Placing", Symbols.FIREPLACE));
		Add(new TreeNode(RECYCLE, "Recycle Bin", Symbols.BIN_RECYCLE));		
		
		this[COMMANDS].Add(new AfterlifeToggle("Afterlife", "View Hidden", Symbols.GHOST));
		this[COMMANDS].Add(new AddLayerCommand("AddLayer", "Add Layer", Symbols.LAYER_PLUS));
		this[COMMANDS].Add(new SetLayerActiveCommand("SetLayerActive", "Set Layer Active", string.Empty));
		this[COMMANDS].Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT));
		this[COMMANDS].Add(new PianoCommand("Piano", "Drop Piano", Symbols.PIANO));
		this[COMMANDS].Add(new BoxCommand("Box", "Box Selection", Symbols.SQUARE_DASHED));
		this[COMMANDS].Add(new CommandNode("Camera", "Camera", Symbols.CAMERA));
		this[COMMANDS].Add(new EllipseCommand("Ellipse", "Ellipse Selection", Symbols.CIRCLE_DASHED));
		this[COMMANDS].Add(new CommandNode("Copy", "Copy", Symbols.COPY));
		this[COMMANDS].Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS));
		this[COMMANDS].Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH));
		this[COMMANDS].Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE));
		this[COMMANDS].Add(new LassoCommand("Lasso", "Lasso Select", Symbols.LASSO));
		this[COMMANDS].Add(new CommandNode("Lock", "Lock", Symbols.LOCK));
		this[COMMANDS].Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET));
		this[COMMANDS].Add(new CommandNode("New", "New", Symbols.FILE));
		this[COMMANDS].Add(new CommandNode("Open", "Open", Symbols.FOLDER_OPEN));
		this[COMMANDS].Add(new CommandNode("Paste", "Paste", Symbols.PASTE));
		this[COMMANDS].Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT));
		this[COMMANDS].Add(new RenameCommand("Rename", "Rename", string.Empty));
		this[COMMANDS].Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN));
		this[COMMANDS].Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK));
		this[COMMANDS].Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK));
		this[COMMANDS].Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT));
		this[COMMANDS].Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN));
		this[COMMANDS].Add(new WeatherToggle("Weather", "Weather", Symbols.SUN));
		this[COMMANDS].Add(new CursorToggle("Cursor", "Toggle Cursor", Symbols.ARROW_POINTER));
		this[COMMANDS].Add(new HudToggle("Hud", "Toggle Hud", Symbols.EYE));
		this[COMMANDS].Add(new ControlToggle("Control", "Toggle Control", Symbols.PERSON_PREGNANT));
		this[COMMANDS].Add(new CreateFolder("CreateFolder", "Create Folder", Symbols.FOLDER_PLUS));
				
		this[LAYERS].Add(new IndestructableFolderNode(BRUSHED, "Brushed", Symbols.PAINTBRUSH));
		this[LAYERS].Add(new IndestructableFolderNode(HIDDEN, "Hidden", Symbols.PAINTBRUSH));
		// default layer for now
		this[LAYERS].AddState(TreeNodeState.ACTIVE);
		
		this[COMMANDS].Add(new TreeNode(TOOLS, "Tools", Symbols.TOOLBOX));
		this[COMMANDS][TOOLS].Add(new TranslateTool("Translate", "Translation Mode", Symbols.UP_DOWN_LEFT_RIGHT));
		this[COMMANDS][TOOLS].Add(new RotateTool("Rotate", "Rotation Mode", Symbols.ROTATE));
		this[COMMANDS][TOOLS].Add(new ScaleTool("Scale", "Scale Mode", Symbols.ARROWS_MAXIMIZE));		
		
		this[MENUS].Add(new CommandNode("File", "File", Symbols.FILE_SPREADSHEET));
		this[MENUS].Add(new CommandNode("Edit", "Edit", Symbols.FILE_PEN));
		this[MENUS].Add(new CommandNode("View", "View", Symbols.BINOCULARS));
		this[MENUS].Add(new CommandNode("Mission", "Mission", Symbols.PERSON_RIFLE));
		this[MENUS]["File"].Add(this[COMMANDS]["New"]);
		this[MENUS]["File"].Add(this[COMMANDS]["Open"]);
		this[MENUS]["File"].Add(this[COMMANDS]["Save"]);
		this[MENUS]["File"].Add(this[COMMANDS]["SaveAs"]);
		
		this[PLACEABLES].Add(new TreeNode("Unknown", "Unknown", Symbols.CHESS_QUEEN));
		this[PLACEABLES].Add(new TreeNode("Plants", "Plants", Symbols.TREE));
		this[PLACEABLES].Add(new TreeNode("Rocks", "Rocks", Symbols.HILL_ROCKSLIDE));
		this[PLACEABLES].Add(new TreeNode("Clutter", "Clutter", Symbols.TRASH));
		this[PLACEABLES].Add(new TreeNode("Structures", "Structures", Symbols.HOUSE));
		this[PLACEABLES].Add(new TreeNode("Wrecks", "Wrecks", Symbols.CAR_BURST));
		this[PLACEABLES].Add(new TreeNode("AI", "AI", Symbols.PERSON));
		this[PLACEABLES].Add(new TreeNode("Water", "Water", Symbols.WATER));
		this[PLACEABLES].Add(new TreeNode("Vehicles", "Vehicles", Symbols.CAR));
		this[PLACEABLES].Add(new TreeNode("StaticObjects", "Static Objects", Symbols.OBJECT_INTERSECT));
		this[PLACEABLES].Add(new TreeNode("DynamicObjects", "Dynamic Objects", Symbols.SHIRT));
		this[PLACEABLES].Add(new TreeNode("ScriptedObjects", "Scripted Objects", Symbols.CODE));
		
		this[BRUSHES].Add(new BetulaPendula_Brush("BetulaPendula_Brush", "Betula Pendula", Symbols.TREES));
		this[BRUSHES].Add(new LightningBrush("LightningBrush", "Lightning Brush", Symbols.BOLT));
		this[BRUSHES].Add(this[COMMANDS]["Piano"]);

		// handle config objects
#ifndef SERVER
#ifndef WORKBENCH
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
		string category = "Unknown";
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
				
				this[PLACEABLES][category].Add(new PlaceableNode(type, type, this[PLACEABLES][category].GetIcon()));
		    }
		}
		//473
		for (int j = 0; j < 5; j++) {
			array<string> p3d_files = Directory.EnumerateFiles("DZ\\" + DayZGame.P3D_DIRECTORIES[j], "*.p3d");
			foreach (string p3d: p3d_files) {
				TreeNode current = this;
				array<string> p3d_split = {};
				p3d.Split(Directory.PATH_SEPERATOR, p3d_split);
				for (int k = 0; k < p3d_split.Count() - 1; k++) {
					Print(p3d_split[k]);
					if (!current[p3d_split[k]]) {
						current[p3d_split[k]] = new TreeNode(p3d_split[k], p3d_split[k], Symbols.FOLDER);
					}
					
					current = current[p3d_split[k]];
				}
				
				//Print(current);
				Print(p3d);
				//Print(p3d_split[p3d_split.Count() - 1]);
				current[p3d_split[p3d_split.Count() - 1]] = new PlaceableNode(p3d, p3d_split[p3d_split.Count() - 1], Symbols.TRIANGLE);
			}
		}
		
		foreach (Param3<typename, string, string> scripted_instance: RegisterScriptedEntity.Instances) {
			this[PLACEABLES]["ScriptedObjects"].Add(new PlaceableNode(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}		
#endif
#endif
	}

	void ~EditorNode() 
	{
		if (GetGame() && GetGame().GetUpdateQueue(CALL_CATEGORY_GUI)) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		}
		
		delete m_Hud;
		GetGame().ObjectDelete(m_Camera);
	}
	
	override void OnSynchronized()
	{
		super.OnSynchronized();
		
#ifndef SERVER
		m_Hud = new EditorHud(this);
		
		// how long until this is a node?? :/
		if (!m_Camera) {
			if (!Player) {
				Player = DayZPlayer.Cast(Identity.GetPlayer());
				if (Player) {
					Error("Could not find player");
					return;
				}
			}
			
			m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", Player.GetPosition() + "0 10 0", ECE_LOCAL));
			m_Camera.SetActive(true);
		}
#endif
	}
		
	void InsertHistory(string display_name, Symbols icon, TreeNode node, ScriptReadWriteContext data)
	{
		// Clear the stack first
		foreach (string uuid, TreeNode undo_redo_node: this[HISTORY].Children) {	
			EditorFootprint footprint = EditorFootprint.Cast(undo_redo_node);
			if (!footprint) {
				continue;
			}
						
			if (!footprint.IsUndone()) {
				break;
			}	
			
			delete this[HISTORY][uuid];
		}
		
		string uuid_generated = string.Format("History:%1", this[HISTORY].Children.Count());		
		this[HISTORY][uuid_generated] = new EditorFootprint(uuid_generated, display_name, icon, node, data);
	}
	
	override void Update(float timeslice)
	{
		Input input = GetGame().GetInput();
		if (!m_Camera) {
			return;
		}
								
		if (input.LocalPress_ID(UAUIBack)) {
			TreeNode.StateMachine.RemoveAllStates(TreeNodeState.CONTEXT);
			TreeNode.StateMachine.RemoveAllStates(TreeNodeState.ACTIVE);
		}
		
		Raycast raycast = m_Camera.PerformCursorRaycast();
		if (input.LocalPress_ID(UAFire) || input.LocalPress_ID(UAUIBack)) {			
			
			if (!GetWidgetUnderCursor()) {
				// reassigning because were gonna fuck with this statemachine array
				array<TreeNode> nodes = {};
				nodes.Copy(TreeNode.StateMachine[TreeNodeState.ACTIVE]);
				foreach (TreeNode node_to_deselect: nodes) {
					if (node_to_deselect && node_to_deselect.GetInteractType() == TreeNodeInteract.PRESS) {
						node_to_deselect.RemoveState(TreeNodeState.ACTIVE);
					}
				}
				
				TreeNode.StateMachine.RemoveAllStates(TreeNodeState.CONTEXT);
			}		
		}
								
		foreach (string uuid, TreeNode node1: Children[PLACING].Children) {
			ObjectNode object_node = ObjectNode.Cast(node1);
			if (!object_node) {
				continue;
			}
			
			raycast = m_Camera.PerformCursorRaycast(object_node.GetObject());
			if (!raycast) {
				continue;
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
			
			//Print(Placing.Count());
			vector transform[4] = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
			object_node.SetBaseTransform(transform);
		}
		
		if (input.LocalPress_ID(UAZoomIn)) {
			if (GetGame().GetUIManager().IsCursorVisible()) {				
				vector camera_position = m_Camera.GetCursorRay().GetPoint(1000.0);
				raycast = m_Camera.PerformCursorRaycast();
				if (raycast) {
					vector current_position = m_Camera.GetPosition();
					float y_height = current_position[1] - GetGame().SurfaceY(current_position[0], current_position[2]);
					camera_position = raycast.Bounce.GetPoint(y_height);
				}
								
				m_Camera.SetPosition(camera_position);
				m_Camera.Update();
			} else {
				m_Camera.FieldOfView = GameConstants.DZPLAYER_CAMERA_FOV_EYEZOOM;
			}
		}
		
		if (input.LocalRelease_ID(UAZoomIn)) { 
			m_Camera.FieldOfView = 1.0;
		}
	}
				
	override void Write(Serializer serializer, int version)
	{		
		super.Write(serializer, version);
		
		serializer.Write(Identity);
		serializer.Write(Player);
		serializer.Write(m_Camera);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(Identity);	
		serializer.Read(Player);
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
			
			GetDayZGame().SelectPlayer(Identity, Player);
			Hud hud = GetDayZGame().GetMission().GetHud();
			hud.ShowHudUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.HUD));
			hud.ShowQuickbarUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.QUICKBAR));
		}
	}
	
	void Undo()
	{
		foreach (string uuid, TreeNode node: this[HISTORY].Children) {
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
		for (int i = this[HISTORY].Children.Count() - 1; i >= 0; i--) {
			EditorFootprint footprint = EditorFootprint.Cast(this[HISTORY].Children.GetElement(i));
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
		foreach (string uuid, TreeNode node: this[HISTORY].Children) {
			EditorFootprint footprint = EditorFootprint.Cast(node);
			if (footprint && !footprint.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool CanRedo() 
	{
		for (int i = this[HISTORY].Children.Count() - 1; i >= 0; i--) {
			EditorFootprint footprint = EditorFootprint.Cast(this[HISTORY].Children.GetElement(i));
			if (footprint && footprint.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	TreeNode GetPlacingDestination()
	{
		foreach (TreeNode node: TreeNode.StateMachine[TreeNodeState.FOCUS]) {
			if (node && node.GetState().IsContext()) {
				return node;
			}
		}
		
		foreach (TreeNode tree_node: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {
			FolderNode folder_node = FolderNode.Cast(tree_node);
			if (folder_node && folder_node.GetState().IsActive()) {
				return folder_node;
			}
		}
		
		return GetNode(EditorNode.LAYERS);
	}
	
    static float GetSolarDeclination(DateTime date) 
	{			
		DateTime delta = (date - DateTime.Create(date.GetYear(), 1, 1));
        int day_of_year = DateTime.GetDays(delta.GetYear(), delta.GetMonth(), delta.GetDay()) + 1;
        float declination = 23.45 * Math.Sin(Math.DEG2RAD * (360 * (day_of_year + 10) / 365));
        return declination;
	}
	
	DateTime SunniestDay(int year, float lattitude)
	{
		DateTime solstice_date = DateTime.Create(year, 6, 21);
        float declination = GetSolarDeclination(solstice_date);

        for (int i = -10; i < 11; i++) {
            if ((GetSolarDeclination(solstice_date + DateTime.Create(i, 0, 0))) > declination) {
                declination = GetSolarDeclination(solstice_date + DateTime.Create(i, 0, 0));
                solstice_date = solstice_date + DateTime.Create(i, 0, 0);
			}
		}
		
        return solstice_date;
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
			object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL | ECE_INITAI | ECE_CREATEPHYSICS | ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_UPDATEPATHGRAPH);
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
		
	EditorProfileSettings GetProfileSettings()
	{
		return EditorProfileSettings.Cast(GetDayZGame().GetProfileSetting(EditorProfileSettings));
	}
	
	bool IsActive()
	{
		return m_Camera && m_Camera.IsActive();
	}
	
	bool IsLocal()
	{
		return GetGame() && GetGame().GetUserManager() && GetGame().GetUserManager().GetTitleInitiator() && m_UUID == GetGame().GetUserManager().GetTitleInitiator().GetUid();
	}
		
	TreeNode FindNodeFromObject(Object object)
	{
		// Do this better! searching!!!
		return ObjectNode.All[object];
	}
		
	TreeNode GetLayers()
	{
		return this[LAYERS];
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
		return this[HISTORY];
	}
	
	TreeNode GetPlaceables()
	{
		return this[PLACEABLES];
	}
	
	TreeNode GetBrushes()
	{
		return this[BRUSHES];
	}
	
	TreeNode GetMenu(string menu)
	{
		return this[MENUS][menu];
	}
	
	TreeNode GetRecycle()
	{
		return this[RECYCLE];
	}
	
	TreeNode GetPlacing()
	{
		return this[PLACING];
	}
}