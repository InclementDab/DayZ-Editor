/*
                    Euler sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/

class EditorNode: Node
{	
	void LegacyLoad(notnull EditorSaveData save_data)
	{
		m_Camera.SetPosition(save_data.CameraPosition);
		foreach (EditorObjectData data: save_data.EditorObjects) {
			GetPlacingDestination().Add(data.CovertToNode());
		}
	}
	
	static const ref array<string> FORBIDDEN_ITEMS = { 
		"TestObject",
		"ItemOptics", 
		"AKM_TESTBED", 
		"Red9", 
		"QuickieBow", 
		"LargeTentBackPack", 
		"SurvivorMale_Base", 
		"SurvivorFemale_Base", 
		"Land_VASICore", 
		"FlagCarrierCore"
	};
	
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	
	PlayerIdentity Identity;
	// this isnt 'const' because climate change exists in DayZ
	int Climate = ARGB(0 /* todo, low-precision forecast? */, GetGame().GetWeather().GetFog().GetActual() * 255, GetGame().GetWeather().GetRain().GetActual(), GetGame().GetWeather().GetOvercast().GetActual());
	
	protected EditorCamera m_Camera;
	protected ref EditorHud	m_Hud;
		
	static const string MISSION = "Mission";
	static const string COMMANDS = "Commands";
	static const string TOOLS = "Tools";
	static const string DZ = "DZ";
	static const string VEHICLES = "Vehicles";
	static const string WEAPONS = "Weapons";
	static const string SCRIPTED = "Scripted";
	
	//[Node("SERVER\\Layers", "Layers", Symbols.LAYER_GROUP)]
	static const string LAYERS = "Layers";
	
		//[Node("SERVER\\Layers\\Brushed", "Brushed", Symbols.PAINTBRUSH)];
		static const string BRUSHED = "Brushed";
		static const string HIDDEN = "Hidden";
	
	
	static const string MENUS = "Menus";
	static const string HISTORY = "UndoRedo";
	static const string PLACEABLES = "Placeables";
	static const string BRUSHES = "Brushes";
	static const string PLACING = "Placing";
	static const string RECYCLE = "Recycle";
		
	void EditorNode(UUID uuid, PlayerIdentity identity = null) 
	{
		Identity = identity;
		
#ifndef SERVER
		Add(new NamedNode(DZ, "DZ", Symbols.FOLDER, LinearColor.WHITE));
		Add(new NamedNode(VEHICLES, "CfgVehicles", Symbols.FOLDER, LinearColor.WHITE));
		Add(new NamedNode(WEAPONS, "CfgWeapons", Symbols.FOLDER, LinearColor.WHITE));
		Add(new NamedNode(SCRIPTED, "Scripted Entities", Symbols.FOLDER, LinearColor.WHITE));
		
		Add(new NamedNode(COMMANDS, "Commands", Symbols.COMMAND, LinearColor.WHITE));
		Add(new NamedNode(TOOLS, "Tools", Symbols.TOOLBOX, LinearColor.WHITE));
		
		this[COMMANDS].Add(new AfterlifeToggle("Afterlife", "View Hidden", Symbols.GHOST, LinearColor.WHITE));
		this[COMMANDS].Add(new AddLayerCommand("AddLayer", "Add Layer", Symbols.LAYER_PLUS, LinearColor.WHITE));
		this[COMMANDS].Add(new SetLayerActiveCommand("SetLayerActive", "Set Layer Active", string.Empty, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT, LinearColor.WHITE));
		this[COMMANDS].Add(new PianoCommand("Piano", "Drop Piano", Symbols.PIANO, LinearColor.WHITE));
		this[COMMANDS].Add(new BoxCommand("Box", "Box Selection", Symbols.SQUARE_DASHED, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Camera", "Camera", Symbols.CAMERA, LinearColor.WHITE));
		this[COMMANDS].Add(new EllipseCommand("Ellipse", "Ellipse Selection", Symbols.CIRCLE_DASHED, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Copy", "Copy", Symbols.COPY, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS, LinearColor.WHITE));
		this[COMMANDS].Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE, LinearColor.WHITE));
		this[COMMANDS].Add(new LassoCommand("Lasso", "Lasso Select", Symbols.LASSO, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Lock", "Lock", Symbols.LOCK, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("New", "New", Symbols.FILE, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Open", "Open", Symbols.FOLDER_OPEN, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Paste", "Paste", Symbols.PASTE, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT, LinearColor.WHITE));
		this[COMMANDS].Add(new RenameCommand("Rename", "Rename", string.Empty, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN, LinearColor.WHITE));
		this[COMMANDS].Add(new WeatherToggle("Weather", "Weather", Symbols.SUN, LinearColor.WHITE));
		this[COMMANDS].Add(new CursorToggle("Cursor", "Toggle Cursor", Symbols.ARROW_POINTER, LinearColor.WHITE));
		this[COMMANDS].Add(new HudToggle("Hud", "Toggle Hud", Symbols.EYE, LinearColor.WHITE));
		this[COMMANDS].Add(new MapToggle("Map", "Toggle Map", Symbols.MAP, LinearColor.WHITE));
		this[COMMANDS].Add(new ControlToggle("Control", "Toggle Control", Symbols.PERSON_PREGNANT, LinearColor.WHITE));
		this[COMMANDS].Add(new CreateFolder("CreateFolder", "Create Folder", Symbols.FOLDER_PLUS, LinearColor.WHITE));
		this[COMMANDS].Add(new CommandNode("Console", "Toggle Console", Brands.XBOX, LinearColor.WHITE));
		
		this[TOOLS].Add(new TranslateTool("Translate", "Translation Mode", Symbols.UP_DOWN_LEFT_RIGHT, LinearColor.WHITE));
		this[TOOLS].Add(new RotateTool("Rotate", "Rotation Mode", Symbols.ROTATE, LinearColor.WHITE));
		this[TOOLS].Add(new ScaleTool("Scale", "Scale Mode", Symbols.ARROWS_MAXIMIZE, LinearColor.WHITE));
		
#ifndef COMPONENT_SYSTEM
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(CFG_VEHICLESPATH); i++) {
			string name;
	        GetGame().ConfigGetChildName(CFG_VEHICLESPATH, i, name);
			if (FORBIDDEN_ITEMS.Find(name) != -1) {
				continue;
			}
			
			ConfigObjectEntry entry = ConfigObjectEntry(string.Format("%1 %2", CFG_VEHICLESPATH, name));
			//Print(entry.Scope);
			//Print(entry.GetName());
			
			//array<string> full_path = {};
			//GetGame().ConfigGetFullPath(string.Format("%1 %2", CFG_VEHICLESPATH, name), full_path);
			//this[category].Add(new PlaceableNode(name, name, this[category].GetIcon()));
	    }
				
		
		float t = GetGame().GetTime();
		for (int j = 0; j < 473; j++) {
			array<string> p3d_files = Directory.EnumerateFiles("DZ\\" + DayZGame.P3D_DIRECTORIES[j], "*.p3d");
			//p3d_files.Debug();
			foreach (string p3d: p3d_files) {
				Node current = this[DZ];
				array<string> p3d_split = {};
				p3d.Split(Directory.PATH_SEPERATOR, p3d_split);
				for (int k = 1; k < p3d_split.Count() - 1; k++) {
					//Print(p3d_split[k]);
					if (!current[p3d_split[k]]) {
						current.Add(new NamedNode(p3d_split[k], p3d_split[k], Symbols.FOLDER, LinearColor.WHITE));
					}
					
					//Print(string.Format("%1:%2", current.GetUUID(), p3d_split[k]));
					current = current[p3d_split[k]];
				}
				
				//PrintFormat("[%1] registering as %2", p3d, p3d_split[p3d_split.Count() - 1]);
				current.Add(new PlaceableNode(p3d, p3d_split[p3d_split.Count() - 1], Symbols.TRIANGLE, LinearColor.WHITE));
			}
		}
		
		Print(string.Format("%1 nodes/second", (float)j / ((float)GetGame().GetTime() - t) * 1000.0 ));

		foreach (Param3<typename, string, string> scripted_instance: RegisterScriptedEntity.Instances) {
			this[SCRIPTED].Add(new PlaceableNode(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3, LinearColor.WHITE));
		}	
#endif
		
#else
		Add(new FileNode(MISSION, "Mission", Symbols.FOLDER, LinearColor.WHITE, "$mission:"));
		array<string> mission_files = Directory.EnumerateFiles("$mission:");
		foreach (File mission_file: mission_files) {
			switch (mission_file.GetExtension()) {
				case ".xml": {
					this[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE_XML, LinearColor.WHITE, mission_file));
					break;
				}
				
				case ".json": {
					this[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE_CODE, LinearColor.WHITE, mission_file));
					break;
				}
				
				/*
				case string.Format(".%1", GetDayZGame().GetWorldNameEx(false)):
				case string.Empty: {
					array<string> files = Directory.EnumerateFiles(mission_file);
					foreach (string f: files) {
						string file_formatted = f;
						file_formatted.Replace(m_File, "");
						file_formatted.Replace("$mission:\\", "");
						file_formatted.Replace("\\", "");
						this[FILES].Add(new FileNode(file_formatted, file_formatted, Symbols.FILE, f));
					}
					
					break;
				}*/
				
				default: {
					this[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE, LinearColor.WHITE, mission_file));
					break;
				}
			}
		}
#endif
		
		// Load all default categories and placements
		Add(new NamedNode(LAYERS, "Layers", Symbols.LAYER_GROUP, LinearColor.WHITE));
		Add(new NamedNode(MENUS, "Menus", Symbols.SQUARE_LIST, LinearColor.WHITE));
		Add(new NamedNode(HISTORY, "History", Symbols.CLOCK_ROTATE_LEFT, LinearColor.WHITE));
		Add(new NamedNode(PLACEABLES, "Placeable Objects", Symbols.ADDRESS_BOOK, LinearColor.WHITE));
		Add(new NamedNode(BRUSHES, "Brushes", Symbols.BRUSH, LinearColor.WHITE));
		Add(new NamedNode(PLACING, "Placing", Symbols.FIREPLACE, LinearColor.WHITE));
		Add(new NamedNode(RECYCLE, "Recycle Bin", Symbols.BIN_RECYCLE, LinearColor.WHITE));
		
		this[LAYERS].Add(new NamedNode(BRUSHED, "Brushed", Symbols.PAINTBRUSH, LinearColor.WHITE));
		this[LAYERS].Add(new NamedNode(HIDDEN, "Hidden", Symbols.PAINTBRUSH, LinearColor.WHITE));
		// default layer for now
		this[LAYERS].AddState(NodeState.ACTIVE);
		
		/*
		
		this[MENUS].Add(new CommandNode("File", "File", Symbols.FILE_SPREADSHEET));
		this[MENUS].Add(new CommandNode("Edit", "Edit", Symbols.FILE_PEN));
		this[MENUS].Add(new CommandNode("View", "View", Symbols.BINOCULARS));
		this[MENUS].Add(new CommandNode("Mission", "Mission", Symbols.PERSON_RIFLE));
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["New"]);
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["Open"]);
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["Save"]);
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["SaveAs"]);
		*/
		
		this[BRUSHES].Add(new BetulaPendula_Brush("BetulaPendula_Brush", "Betula Pendula", Symbols.TREES, LinearColor.WHITE));
		this[BRUSHES].Add(new LightningBrush("LightningBrush", "Lightning Brush", Symbols.BOLT, LinearColor.WHITE));
		//this[BRUSHES].Add(this[COMMANDS]["Piano"]);
	}

	void ~EditorNode() 
	{		
		delete m_Hud;
		GetGame().ObjectDelete(m_Camera);
	}
	
	override void EUpdate(float dt)
	{
		if (!GetState().IsActive()) {
			return;
		}
		
		Input input = GetGame().GetInput();
		if (input.LocalPress("UAEditorCursor")) {
			GetGame().GetUIManager().ShowCursor(!GetGame().GetUIManager().IsCursorVisible()); 
		}
		
	}
		
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
		
#ifndef SERVER
		if (node_state.IsActive() && IsLocal()) {
			if (state) {
				m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", GetGame().GetPlayer().GetPosition() + "0 10 0", ECE_LOCAL));
				m_Camera.SetActive(true);
				m_Hud = new EditorHud(this);
				GetGame().GetPlayer().GetInputController().SetDisabled(true);				
				
			} else {
				delete m_Hud;
				GetGame().ObjectDelete(m_Camera);
				
				GetDayZGame().SelectPlayer(Identity, GetGame().GetPlayer());
				GetGame().GetPlayer().GetInputController().SetDisabled(false);
				Hud hud = GetDayZGame().GetMission().GetHud();
				hud.ShowHudUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.HUD));
				hud.ShowQuickbarUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.QUICKBAR));
			}
		}
#endif
	}
		
	void InsertHistory(Node node, ScriptReadWriteContext data)
	{
		// Clear the stack first
		foreach (string uuid, Node undo_redo_node: this[HISTORY].Children) {	
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
		this[HISTORY][uuid_generated] = new EditorFootprint(uuid_generated, node, data);
	}
					
	override void Write(Serializer serializer, int version)
	{		
		super.Write(serializer, version);
		
		serializer.Write(Identity);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(Identity);							
		return true;
	}
						
	// EditorSounds is helpful
	void PlaySound(string sound_set)
	{
		SEffectManager.PlaySoundOnObject(sound_set, m_Camera);
	}
		
	void Undo()
	{
		foreach (string uuid, Node node: this[HISTORY].Children) {
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
		foreach (string uuid, Node node: this[HISTORY].Children) {
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
	
	Node GetPlacingDestination()
	{
		foreach (Node node: Node.States[NodeState.ACTIVE]) {
			if (node) {
				return node;
			}
		}
		
		foreach (Node tree_node: Node.States[NodeState.ACTIVE]) {
			LayerNode folder_node = LayerNode.Cast(tree_node);
			if (folder_node) {
				return folder_node;
			}
		}
		
		return Get(EditorNode.LAYERS);
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
			object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL | ECE_INITAI | ECE_CREATEPHYSICS | ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_UPDATEPATHGRAPH | ECE_OBJECT_SWAP);
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
	
	override NodeState GetStateMask()
	{
		return NodeState.ACTIVE | NodeState.CONTEXT | NodeState.SYNC_DIRTY | NodeState.CLIENT_AUTH;
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
		
	Node FindNodeFromObject(Object object)
	{
		// Do this better! searching!!!
		return ObjectNode.All[object];
	}
		
	Node GetLayers()
	{
		return this[LAYERS];
	}
	
	Node GetUndoRedo()
	{
		return this[HISTORY];
	}
	
	Node GetPlaceables()
	{
		return this[PLACEABLES];
	}
	
	Node GetBrushes()
	{
		return this[BRUSHES];
	}
	
	Node GetMenu(string menu)
	{
		return this[MENUS][menu];
	}
	
	Node GetRecycle()
	{
		return this[RECYCLE];
	}
	
	Node GetPlacing()
	{
		return this[PLACING];
	}
	
	// structure of actual mission files
	Node GetMission()
	{
		return this[MISSION];
	}
		
	Node GetCommand(string uuid)
	{
		return this[COMMANDS][uuid];
	}
	
	Node GetTool(string uuid)
	{
		return this[TOOLS][uuid];
	}
	
	Node GetStaticObjects()
	{
		return this[DZ];
	}
	
	Node GetDynamicObjects()
	{
		return this[VEHICLES];
	}
	
	Node GetWeapons()
	{
		return this[WEAPONS];
	}
	
	Node GetScripted()
	{
		return this[SCRIPTED];
	}
}